import pygame
import config
import scene

pygame.init()

config.screen = pygame.display.set_mode(config.size)
pygame.display.set_caption("Snake")

while True:
    gameScene = scene.GameScene()
    gameScene.run()
