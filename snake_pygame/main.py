import sys
import pygame
import config
from sprite import Snake, Direction, Food

pygame.init()

config.screen = pygame.display.set_mode(config.size)
pygame.display.set_caption("Snake")

snake = Snake()
food = Food()
food.randGenPos(snake)

clock = pygame.time.Clock()

while True:
    clock.tick_busy_loop(60)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            d = snake.curDir
            if event.key == pygame.K_w:
                d = Direction.UP
            elif event.key == pygame.K_s:
                d = Direction.DOWN
            elif event.key == pygame.K_a:
                d = Direction.LEFT
            elif event.key == pygame.K_d:
                d = Direction.RIGHT
            snake.ctrlDir(d)
        elif event.type == pygame.USEREVENT + config.snakeMoveEventID:
            snake.move()

    if snake.isDead():
        snake = Snake()
        food.randGenPos(snake)

    if snake.isGetFood(food):
        snake.grow()
        food.randGenPos(snake)

    config.screen.fill((255, 255, 255))

    for i in range(config.vertInrNum):
        pygame.draw.line(config.screen, (0, 0, 0), (0, i * config.vertInr), (config.width, i * config.vertInr))
    for i in range(1, config.horzInrNum):
        pygame.draw.line(config.screen, (0, 0, 0), (i * config.horzInr, 0), (i * config.horzInr, config.height))

    snake.draw(config.screen)

    food.draw(config.screen)

    pygame.display.flip()
