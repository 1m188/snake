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
        self.nextScene = None

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
        self.prepare()
        self.isRunning = True
        while self.isRunning:
            self.fpsClock.tick_busy_loop(self.fps)
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.isRunning = False
                else:
                    self.eventHandle(event)
            self.update()
            self.screen.fill((0, 0, 0))
            self.render()
            pygame.display.flip()
        self.end()


# 开始场景
class StartScene(Scene):
    def __init__(self):
        super().__init__(pygame.display.get_surface(), config.FPS)

    def prepare(self):
        width = self.screen.get_width()
        height = self.screen.get_height()

        infoTextFont = pygame.font.Font(None, 60)
        btnTextFont = pygame.font.Font(None, 40)
        textColor = (0, 0, 0)

        # 标题信息
        self.infoLab = sprite.Label("Snake", infoTextFont, textColor)
        self.infoLab.rect.center = (width / 2, height / 4)

        # 经典模式
        self.classicBtn = sprite.Button("Classic Mode", btnTextFont, textColor)
        self.classicBtn.rect.center = self.infoLab.rect.center
        self.classicBtn.rect.centery = height / 2

        intervl = (height - self.classicBtn.rect.top) / 4 - self.classicBtn.rect.height

        # 无尽模式
        self.endlessBtn = sprite.Button("Endless Mode", btnTextFont, textColor)
        self.endlessBtn.rect.center = self.classicBtn.rect.center
        self.endlessBtn.rect.top = self.classicBtn.rect.bottom + intervl

        # 最高分
        self.highestScoreBtn = sprite.Button("Highest Score", btnTextFont, textColor)
        self.highestScoreBtn.rect.center = self.endlessBtn.rect.center
        self.highestScoreBtn.rect.top = self.endlessBtn.rect.bottom + intervl

        # 退出
        self.exitBtn = sprite.Button("Exit", btnTextFont, textColor)
        self.exitBtn.rect.center = self.highestScoreBtn.rect.center
        self.exitBtn.rect.top = self.highestScoreBtn.rect.bottom + intervl

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

        if self.snake.isGetFood(self.food):
            self.snake.grow()
            self.food.randGenPos(self.snake)
            self.score += 1

    def render(self):
        width = self.screen.get_width()
        height = self.screen.get_height()
        self.screen.fill((255, 255, 255))
        for i in range(config.vertInrNum):
            pygame.draw.line(self.screen, (0, 0, 0), (0, i * config.vertInr), (width, i * config.vertInr))
        for i in range(1, config.horzInrNum):
            pygame.draw.line(self.screen, (0, 0, 0), (i * config.horzInr, 0), (i * config.horzInr, height))
        self.snake.draw(self.screen)
        self.food.draw(self.screen)

    def end(self):
        self.nextScene = GameoverScene(self.mode, self.score)


# 游戏结束场景
class GameoverScene(Scene):
    def __init__(self, mode: Mode, score: int):
        super().__init__(pygame.display.get_surface(), config.FPS)

        self.score = score
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

        self.gameoverLab = sprite.Label("Game Over", pygame.font.Font(None, 60), (0, 0, 0))
        self.gameoverLab.rect.center = (width / 2, height / 4)

        self.scoreLab = sprite.Label(f"Your score is: {self.score}", pygame.font.Font(None, 40), (0, 0, 0))
        self.scoreLab.rect.center = self.gameoverLab.rect.center
        self.scoreLab.rect.centery = height / 2

        interval = (height - self.scoreLab.rect.top) / (3 if self.isNewHighestScore else 2) - self.scoreLab.rect.height

        preLab = self.scoreLab
        if self.isNewHighestScore:
            self.newHighestScoreLab = sprite.Label("New Highest Score!", pygame.font.Font(None, 40), (0, 0, 0))
            self.newHighestScoreLab.rect.center = preLab.rect.center
            self.newHighestScoreLab.rect.centery = preLab.rect.bottom + interval
            preLab = self.newHighestScoreLab

        self.backBtn = sprite.Button("Back To Start", pygame.font.Font(None, 40), (0, 0, 0))
        self.backBtn.rect.center = preLab.rect.center
        self.backBtn.rect.centery = preLab.rect.bottom + interval

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

        self.highestScoreLab = sprite.Label("Highest Score", pygame.font.Font(None, 60), (0, 0, 0))
        self.highestScoreLab.rect.center = (width / 2, height / 4)

        self.classicHighestScoreLab = sprite.Label(f"Classic Mode: {HighestScore.score[Mode.Classic.value]}", pygame.font.Font(None, 40), (0, 0, 0))
        self.classicHighestScoreLab.rect.center = self.highestScoreLab.rect.center
        self.classicHighestScoreLab.rect.centery = height / 2

        interval = (height - self.classicHighestScoreLab.rect.top) / 3 - self.classicHighestScoreLab.rect.height

        self.endlessHighestScoreLab = sprite.Label(f"Endless Mode: {HighestScore.score[Mode.Endless.value]}", pygame.font.Font(None, 40), (0, 0, 0))
        self.endlessHighestScoreLab.rect.center = self.highestScoreLab.rect.center
        self.endlessHighestScoreLab.rect.centery = self.classicHighestScoreLab.rect.bottom + interval

        self.backBtn = sprite.Button("Back To Start", pygame.font.Font(None, 40), (0, 0, 0))
        self.backBtn.rect.center = self.endlessHighestScoreLab.rect.center
        self.backBtn.rect.centery = self.endlessHighestScoreLab.rect.bottom + interval

    def eventHandle(self, event):
        self.backBtn.eventHandle(event)
        if event.type == pygame.USEREVENT:
            if event.signal == self.backBtn.clicked:
                self.isRunning = False
                self.nextScene = StartScene()

    def render(self):
        self.screen.fill((255, 255, 255))
        self.highestScoreLab.draw(self.screen)
        self.classicHighestScoreLab.draw(self.screen)
        self.endlessHighestScoreLab.draw(self.screen)
        self.backBtn.draw(self.screen)
