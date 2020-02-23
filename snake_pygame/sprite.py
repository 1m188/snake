from enum import Enum
from random import randint
import pygame
import config


# 方向
class Direction(Enum):
    UP = (0, -1)
    DOWN = (0, 1)
    LEFT = (-1, 0)
    RIGHT = (1, 0)


# 蛇
class Snake:
    def __init__(self):
        self.pos = [(2, 2), (1, 2)]
        self.curDir = Direction.RIGHT
        pygame.time.set_timer(pygame.USEREVENT + config.snakeMoveEventID, config.snakeMoveTimeInr)

    def acc(self, isAcc: bool):
        pygame.time.set_timer(pygame.USEREVENT + config.snakeMoveEventID, int(config.snakeMoveTimeInr / 3) if isAcc else config.snakeMoveTimeInr)

    # 蛇的移动
    def move(self):
        self.pos.pop(-1)
        head = self.pos[0]
        newHead = (head[0] + self.curDir.value[0], head[1] + self.curDir.value[1])
        self.pos.insert(0, newHead)

    # 控制方向
    def ctrlDir(self, d: Direction):
        curDir = Direction((self.pos[0][0] - self.pos[1][0], self.pos[0][1] - self.pos[1][1]))  # 这里使用的方向是用当前坐标计算的实际方向，而不是保存的应该的方向。防止在尚未移动的时候就转换了方向导致绘制的时候出现反常情况。
        if (d == Direction.UP and curDir != Direction.DOWN) or (d == Direction.DOWN and curDir != Direction.UP) or (d == Direction.LEFT and curDir != Direction.RIGHT) or (d == Direction.RIGHT and curDir != Direction.LEFT):
            self.curDir = d

    # 判断是否死亡
    def isDead(self) -> bool:
        head = self.pos[0]
        if head[0] < 0 or head[0] > config.horzInrNum - 1 or head[1] < 0 or head[1] > config.vertInrNum - 1:
            return True
        for pos in self.pos[1:]:
            if head == pos:
                return True
        return False

    # 判断是否吃到食物
    def isGetFood(self, food) -> bool:
        return self.pos[0] == food.pos

    # 吃到食物之后的增长
    def grow(self):
        tail = self.pos[-1]
        secondTail = self.pos[-2]
        newTail = (tail[0] + tail[0] - secondTail[0], tail[1] + tail[1] - secondTail[1])
        self.pos.append(newTail)

    # 绘制
    def draw(self, screen: pygame.surface.Surface):
        for pos in self.pos:
            rect = pygame.rect.Rect(pos[0] * config.horzInr, pos[1] * config.vertInr, config.horzInr + 2, config.vertInr + 2)
            color = (0, 0, 255)
            screen.fill(color, rect)


# 食物
class Food:
    def __init__(self):
        self.pos = None

    # 随机生成食物，确保食物的坐标和之前的坐标以及蛇身的坐标不重合
    def randGenPos(self, snake: Snake):
        pos = None
        while True:
            pos = (randint(0, config.horzInrNum - 1), randint(0, config.vertInrNum - 1))
            if pos == self.pos:
                continue
            isContinue = False
            for p in snake.pos:
                if pos == p:
                    isContinue = True
                    break
            if isContinue:
                continue
            break
        self.pos = pos

    # 绘制
    def draw(self, screen: pygame.surface.Surface):
        color = (255, 0, 0)
        rect = pygame.rect.Rect(self.pos[0] * config.horzInr, self.pos[1] * config.vertInr, config.horzInr + 1, config.vertInr + 1)
        pygame.draw.ellipse(screen, color, rect)
