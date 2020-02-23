import sys
import pygame
import sprite
import config


# 场景
class Scene:
    # @param screen:绘制表面 fps:帧率
    def __init__(self, screen: pygame.surface.Surface, fps: int):
        self.screen = screen
        self.fpsClock = pygame.time.Clock()
        self.fps = fps
        self.isRunning = False

    # 场景开始之前的准备
    def prepare(self, *args, **kwargs):
        pass

    # 事件处理
    def eventHandle(self, event):
        pass

    # 每帧的状态更新
    def update(self, *args, **kwargs):
        pass

    # 每帧渲染
    def render(self, *args, **kwargs):
        pass

    # 场景结束之后的处理
    def end(self, *args, **kwargs):
        pass

    def run(self, *args, **kwargs):
        self.prepare(*args, **kwargs)
        self.isRunning = True
        while self.isRunning:
            self.fpsClock.tick_busy_loop(self.fps)
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    sys.exit()
                else:
                    self.eventHandle(event)
            self.update(*args, **kwargs)
            self.screen.fill((0, 0, 0))
            self.render(*args, **kwargs)
            pygame.display.flip()
        self.end(*args, **kwargs)


# 开始场景
class StartScene(Scene):
    def __init__(self):
        super().__init__(config.screen, config.FPS)

    def prepare(self, *args, **kwargs):
        infoTextFont = pygame.font.Font(None, 60)
        btnTextFont = pygame.font.Font(None, 40)
        textColor = (0, 0, 0)

        # 标题信息
        self.infoLab = sprite.Label("Snake", infoTextFont, textColor)
        self.infoLab.rect.center = (config.width / 2, config.height / 4)

        # 经典模式
        self.classicBtn = sprite.Button("Classic Mode", btnTextFont, textColor)
        self.classicBtn.rect.center = self.infoLab.rect.center
        self.classicBtn.rect.centery = config.height / 2

        intervl = (config.height - self.classicBtn.rect.top) / 4 - self.classicBtn.rect.height

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
                pass
            elif event.signal == self.endlessBtn.clicked:  # 点击无尽模式按钮
                pass
            elif event.signal == self.highestScoreBtn.clicked:  # 点击最高分按钮
                pass
            elif event.signal == self.exitBtn.clicked:  # 点击退出按钮
                sys.exit()
        self.classicBtn.eventHandle(event)
        self.endlessBtn.eventHandle(event)
        self.highestScoreBtn.eventHandle(event)
        self.exitBtn.eventHandle(event)

    def render(self, *args, **kwargs):
        self.screen.fill((255, 255, 255))
        self.infoLab.draw(self.screen)
        self.classicBtn.draw(self.screen)
        self.endlessBtn.draw(self.screen)
        self.highestScoreBtn.draw(self.screen)
        self.exitBtn.draw(self.screen)


# 游戏场景
class GameScene(Scene):
    def __init__(self):
        super().__init__(config.screen, config.FPS)

    def prepare(self, *args, **kwargs):
        self.snake = sprite.Snake()
        self.food = sprite.Food()
        self.food.randGenPos(self.snake)

    def eventHandle(self, event):
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                self.snake.acc(True)
            else:
                d = self.snake.curDir
                if event.key == pygame.K_UP:
                    d = sprite.Direction.UP
                elif event.key == pygame.K_DOWN:
                    d = sprite.Direction.DOWN
                elif event.key == pygame.K_LEFT:
                    d = sprite.Direction.LEFT
                elif event.key == pygame.K_RIGHT:
                    d = sprite.Direction.RIGHT
                self.snake.ctrlDir(d)
        elif event.type == pygame.KEYUP and event.key == pygame.K_SPACE:
            self.snake.acc(False)
        elif event.type == pygame.USEREVENT + config.snakeMoveEventID:
            self.snake.move()

    def update(self, *args, **kwargs):
        if self.snake.isDead():
            self.isRunning = False

        if self.snake.isGetFood(self.food):
            self.snake.grow()
            self.food.randGenPos(self.snake)

    def render(self, *args, **kwargs):
        self.screen.fill((255, 255, 255))
        for i in range(config.vertInrNum):
            pygame.draw.line(self.screen, (0, 0, 0), (0, i * config.vertInr), (config.width, i * config.vertInr))
        for i in range(1, config.horzInrNum):
            pygame.draw.line(self.screen, (0, 0, 0), (i * config.horzInr, 0), (i * config.horzInr, config.height))
        self.snake.draw(self.screen)
        self.food.draw(self.screen)
