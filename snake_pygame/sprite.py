from enum import Enum
import pygame
import config


class Direction(Enum):
    UP = (0, -1)
    DOWN = (0, 1)
    LEFT = (-1, 0)
    RIGHT = (1, 0)


class Snake:
    def __init__(self):
        self.pos = [(2, 2), (1, 2)]
        self.curDir = Direction.RIGHT
        pygame.time.set_timer(pygame.USEREVENT + config.snakeMoveEventID, config.snakeMoveTimeInr)

    def move(self):
        self.pos.pop(-1)
        head = self.pos[0]
        newHead = (head[0] + self.curDir.value[0], head[1] + self.curDir.value[1])
        self.pos.insert(0, newHead)

    def ctrlDir(self, d: Direction):
        if (d == Direction.UP and self.curDir != Direction.DOWN) or (d == Direction.DOWN and self.curDir != Direction.UP) or (d == Direction.LEFT and self.curDir != Direction.RIGHT) or (d == Direction.RIGHT and self.curDir != Direction.LEFT):
            self.curDir = d

    def isDead(self) -> bool:
        head = self.pos[0]
        if head[0] < 0 or head[0] > config.horzInrNum - 1 or head[1] < 0 or head[1] > config.vertInrNum - 1:
            return True
        for pos in self.pos[1:]:
            if head == pos:
                return True
        return False

    def draw(self, screen: pygame.surface.Surface):
        for pos in self.pos:
            rect = pygame.rect.Rect(pos[0] * config.horzInr, pos[1] * config.vertInr, config.horzInr + 2, config.vertInr + 2)
            color = (0, 0, 255)
            screen.fill(color, rect)
