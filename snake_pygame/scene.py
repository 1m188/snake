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
