import sys
import pygame

size = (width, height) = (500, 500)
horzInrNum = 15
vertInrNum = 15
horzInr = width / horzInrNum
vertInr = height / vertInrNum

pygame.init()

screen = pygame.display.set_mode(size)
pygame.display.set_caption("Snake")

clock = pygame.time.Clock()

while True:
    clock.tick_busy_loop(60)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()

    screen.fill((255, 255, 255))

    for i in range(vertInrNum):
        pygame.draw.line(screen, (0, 0, 0), (0, i * vertInr), (width, i * vertInr))
    for i in range(1, horzInrNum):
        pygame.draw.line(screen, (0, 0, 0), (i * horzInr, 0), (i * horzInr, height))

    pygame.display.flip()
