import pygame
import config
import scene

pygame.init()

pygame.display.set_mode(config.size)
pygame.display.set_caption("Snake")

config.HighestScore.loadHighestScore()

curScene = scene.StartScene()
while curScene:
    curScene.run()
    curScene = curScene.nextScene

config.HighestScore.saveHighestScore()
