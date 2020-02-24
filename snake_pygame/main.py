import pygame
import config
import scene

pygame.init()  # 初始化pygame

# 初始化游戏窗口
pygame.display.set_mode(config.size)
pygame.display.set_caption("Snake")

# 游戏进入第一个场景前加载最高分
config.HighestScore.loadHighestScore()

# 从开始场景进入游戏循环
curScene = scene.StartScene()
while curScene:
    curScene.run()
    curScene = curScene.nextScene

# 游戏结束之后保存最高分
config.HighestScore.saveHighestScore()
