import pygame
import config
import scene

pygame.init()

pygame.display.set_mode(config.size)
pygame.display.set_caption("Snake")

config.curScene = scene.StartScene()
config.curScene.run()
