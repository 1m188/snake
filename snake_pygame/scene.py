import pygame
import sprite
import config
from config import HighestScore, Mode, Direction


# 场景
class Scene:
    # @param screen:绘制表面 fps:帧率
    def __init__(self, screen: pygame.surface.Surface, fps: int):
        self.screen = screen
        self.fpsClock = pygame.time.Clock()
        self.fps = fps
        self.isRunning = False
        self.nextScene = None  # 下一个场景，设置了这个变量之后，当前场景结束之后会进入下一个场景

        # 禁止该抽象基类被实例化
        # 不用abc是因为所有的方法都可以被选择实现，但是该类不能够被实例化
        if type(self) == Scene:
            raise TypeError("can not create instance of the type Scene")

    # 场景开始之前的准备
    def prepare(self):
        pass

    # 事件处理
    def eventHandle(self, event):
        pass

    # 每帧的状态更新
    def update(self):
        pass

    # 每帧渲染
    def render(self):
        pass

    # 场景结束之后的处理
    def end(self):
        pass

    def run(self):
        self.prepare()  # 首先进行场景开始前的准备
        self.isRunning = True  # 标志场景是否在事件-更新-渲染的循环之中
        while self.isRunning:
            self.fpsClock.tick_busy_loop(self.fps)  # 确保帧率
            # 事件处理
            for event in pygame.event.get():
                if event.type == pygame.QUIT:  # 退出事件统一处理，其他的场景特异性事件另外处理
                    self.isRunning = False
                else:
                    self.eventHandle(event)
            self.update()  # 场景之中的状态更新
            self.screen.fill((0, 0, 0))  # 在新的渲染开始之前抹去之前渲染的所有东西
            self.render()  # 渲染
            pygame.display.flip()  # pygame自带的双缓冲
        self.end()  # 场景结束之后的资源回收场景整理之类的功能，但是不可以写场景切换的代码


# 开始场景
class StartScene(Scene):
    def __init__(self):
        super().__init__(pygame.display.get_surface(), config.FPS)

    def prepare(self):
        width = self.screen.get_width()
        height = self.screen.get_height()

        # 标题信息
        self.infoLab = sprite.Label("Snake", pygame.font.Font(None, 60), (0, 0, 0))
        self.infoLab.center = (width / 2, height / 4)

        # 经典模式
        self.classicBtn = sprite.Button("Classic Mode", pygame.font.Font(None, 40), (0, 0, 0))
        self.classicBtn.centerx = width / 2
        self.classicBtn.top = height / 2

        preCtrl = self.classicBtn
        intervl = (height - self.classicBtn.top) / 4

        # 无尽模式
        self.endlessBtn = sprite.Button("Endless Mode", pygame.font.Font(None, 40), (0, 0, 0))
        self.endlessBtn.centerx = width / 2
        self.endlessBtn.top = preCtrl.top + intervl
        preCtrl = self.endlessBtn

        # 最高分
        self.highestScoreBtn = sprite.Button("Highest Score", pygame.font.Font(None, 40), (0, 0, 0))
        self.highestScoreBtn.centerx = width / 2
        self.highestScoreBtn.top = preCtrl.top + intervl
        preCtrl = self.highestScoreBtn

        # 退出
        self.exitBtn = sprite.Button("Exit", pygame.font.Font(None, 40), (0, 0, 0))
        self.exitBtn.centerx = width / 2
        self.exitBtn.top = preCtrl.top + intervl
        preCtrl = self.exitBtn

    def eventHandle(self, event):
        if event.type == pygame.USEREVENT:
            if event.signal == self.classicBtn.clicked:  # 点击经典模式按钮
                self.isRunning = False
                self.nextScene = GameScene(Mode.Classic)
            elif event.signal == self.endlessBtn.clicked:  # 点击无尽模式按钮
                self.isRunning = False
                self.nextScene = GameScene(Mode.Endless)
            elif event.signal == self.highestScoreBtn.clicked:  # 点击最高分按钮
                self.isRunning = False
                self.nextScene = HighestScoreScene()
            elif event.signal == self.exitBtn.clicked:  # 点击退出按钮
                self.isRunning = False
        self.classicBtn.eventHandle(event)
        self.endlessBtn.eventHandle(event)
        self.highestScoreBtn.eventHandle(event)
        self.exitBtn.eventHandle(event)

    def render(self):
        self.screen.fill((255, 255, 255))
        self.infoLab.draw(self.screen)
        self.classicBtn.draw(self.screen)
        self.endlessBtn.draw(self.screen)
        self.highestScoreBtn.draw(self.screen)
        self.exitBtn.draw(self.screen)


# 游戏场景
class GameScene(Scene):
    def __init__(self, mode: Mode):
        super().__init__(pygame.display.get_surface(), config.FPS)
        self.mode = mode

    def prepare(self):
        self.background = sprite.Background()
        self.snake = sprite.Snake(self.mode)
        self.food = sprite.Food()
        self.food.randGenPos(self.snake)
        self.score = 0

    def eventHandle(self, event):
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                self.snake.acc(True)
            else:
                d = self.snake.curDir
                if event.key == pygame.K_UP:
                    d = Direction.UP
                elif event.key == pygame.K_DOWN:
                    d = Direction.DOWN
                elif event.key == pygame.K_LEFT:
                    d = Direction.LEFT
                elif event.key == pygame.K_RIGHT:
                    d = Direction.RIGHT
                self.snake.ctrlDir(d)
        elif event.type == pygame.KEYUP and event.key == pygame.K_SPACE:
            self.snake.acc(False)
        elif event.type == pygame.USEREVENT + config.snakeMoveEventID:
            self.snake.move()

    def update(self):
        if self.snake.isDead():
            self.isRunning = False
            self.nextScene = GameoverScene(self.mode, self.score)

        if self.snake.isGetFood(self.food):
            self.snake.grow()
            self.food.randGenPos(self.snake)
            self.score += 1

    def render(self):
        self.screen.fill((255, 255, 255))
        self.background.draw(self.screen)
        self.snake.draw(self.screen)
        self.food.draw(self.screen)


# 游戏结束场景
class GameoverScene(Scene):
    def __init__(self, mode: Mode, score: int):
        super().__init__(pygame.display.get_surface(), config.FPS)

        self.mode = mode
        self.score = score

        # 判断是否最高分
        # 是的话更新最高分并且设置标志位
        self.isNewHighestScore = False
        for m in Mode:
            if m == mode:
                if score > HighestScore.score[m.value]:
                    HighestScore.score[m.value] = score
                    self.isNewHighestScore = True
                break

    def prepare(self):
        width = self.screen.get_width()
        height = self.screen.get_height()

        # 游戏结束标题
        self.gameoverLab = sprite.Label("Game Over", pygame.font.Font(None, 60), (0, 0, 0))
        self.gameoverLab.center = (width / 2, height / 4)

        # 分数
        self.scoreLab = sprite.Label(f"Your score is: {self.score}", pygame.font.Font(None, 40), (0, 0, 0))
        self.scoreLab.centerx = width / 2
        self.scoreLab.top = height / 2

        preCtrl = self.scoreLab
        interval = (height - self.scoreLab.top) / (4 if self.isNewHighestScore else 3)

        # 当前模式
        self.modeLab = sprite.Label(f"Your mode is: {self.mode.value}", pygame.font.Font(None, 40), (0, 0, 0))
        self.modeLab.centerx = width / 2
        self.modeLab.top = preCtrl.top + interval
        preCtrl = self.modeLab

        # 最高分提醒
        if self.isNewHighestScore:
            self.newHighestScoreLab = sprite.Label("New Highest Score!", pygame.font.Font(None, 40), (0, 0, 0))
            self.newHighestScoreLab.centerx = width / 2
            self.newHighestScoreLab.top = preCtrl.top + interval
            preCtrl = self.newHighestScoreLab

        # 返回按钮
        self.backBtn = sprite.Button("Back To Start", pygame.font.Font(None, 40), (0, 0, 0))
        self.backBtn.centerx = width / 2
        self.backBtn.top = preCtrl.top + interval
        preCtrl = self.modeLab

    def eventHandle(self, event):
        self.backBtn.eventHandle(event)
        if event.type == pygame.USEREVENT:
            if event.signal == self.backBtn.clicked:
                self.isRunning = False
                self.nextScene = StartScene()

    def render(self):
        self.screen.fill((255, 255, 255))
        self.gameoverLab.draw(self.screen)
        self.scoreLab.draw(self.screen)
        self.modeLab.draw(self.screen)
        if self.isNewHighestScore:
            self.newHighestScoreLab.draw(self.screen)
        self.backBtn.draw(self.screen)


# 最高分场景
class HighestScoreScene(Scene):
    def __init__(self):
        super().__init__(pygame.display.get_surface(), config.FPS)

    def prepare(self):
        width = self.screen.get_width()
        height = self.screen.get_height()

        # 最高分标题
        self.highestScoreLab = sprite.Label("Highest Score", pygame.font.Font(None, 60), (0, 0, 0))
        self.highestScoreLab.center = (width / 2, height / 4)

        interval = height / 2 / (len(Mode) + 1)
        preCtrl = pygame.rect.Rect(0, height / 2 - interval, 1, 1)

        # 按照模式的种类一行一行往下展示
        self.scoreLabList = []
        for mode in Mode:
            scoreLab = sprite.Label(f"{mode.value} Mode: {HighestScore.score[mode.value]}", pygame.font.Font(None, 40), (0, 0, 0))
            scoreLab.centerx = width / 2
            scoreLab.top = preCtrl.top + interval
            preCtrl = scoreLab
            self.scoreLabList.append(scoreLab)

        # 返回按钮
        self.backBtn = sprite.Button("Back To Start", pygame.font.Font(None, 40), (0, 0, 0))
        self.backBtn.centerx = width / 2
        self.backBtn.top = preCtrl.top + interval
        preCtrl = self.backBtn

    def eventHandle(self, event):
        self.backBtn.eventHandle(event)
        if event.type == pygame.USEREVENT:
            if event.signal == self.backBtn.clicked:
                self.isRunning = False
                self.nextScene = StartScene()

    def render(self):
        self.screen.fill((255, 255, 255))
        self.highestScoreLab.draw(self.screen)
        for scoreLab in self.scoreLabList:
            scoreLab.draw(self.screen)
        self.backBtn.draw(self.screen)
