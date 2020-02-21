import sys
import pygame
import config

pygame.init()

config.screen = pygame.display.set_mode(config.size)
pygame.display.set_caption("Snake")

clock = pygame.time.Clock()

while True:
    clock.tick_busy_loop(60)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()

    config.screen.fill((255, 255, 255))

    for i in range(config.vertInrNum):
        pygame.draw.line(config.screen, (0, 0, 0), (0, i * config.vertInr), (config.width, i * config.vertInr))
    for i in range(1, config.horzInrNum):
        pygame.draw.line(config.screen, (0, 0, 0), (i * config.horzInr, 0), (i * config.horzInr, config.height))

    pygame.display.flip()
