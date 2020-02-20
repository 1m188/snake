import sys
import pygame

screen = pygame.display.set_mode((500, 500))

clock = pygame.time.Clock()

while True:
    clock.tick_busy_loop(60)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()

    screen.fill((255, 255, 255))
    pygame.display.flip()
