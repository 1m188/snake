import sys
import pygame


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
