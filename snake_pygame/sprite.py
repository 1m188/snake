from random import randint
import pygame
import config
from config import Mode, Direction


# 蛇
class Snake:
    def __init__(self, mode: Mode):
        self.pos = [(2, 2), (1, 2)]
        self.curDir = Direction.RIGHT
        pygame.time.set_timer(pygame.USEREVENT + config.snakeMoveEventID, config.snakeMoveTimeInr)
        self.mode = mode

    # 加速
    def acc(self, isAcc: bool):
        pygame.time.set_timer(pygame.USEREVENT + config.snakeMoveEventID, int(config.snakeMoveTimeInr / 3) if isAcc else config.snakeMoveTimeInr)

    # 通过坐标位置获取当前实际方向
    def getCurDir(self) -> Direction:
        tmpDir = [self.pos[0][0] - self.pos[1][0], self.pos[0][1] - self.pos[1][1]]
        if self.mode == Mode.Endless:
            if abs(tmpDir[0]) > 1:
                tmpDir[0] %= (-1 if tmpDir[0] > 0 else 1) * config.horzInrNum
            if abs(tmpDir[1]) > 1:
                tmpDir[1] %= (-1 if tmpDir[1] > 0 else 1) * config.vertInrNum
        return Direction(tuple(tmpDir))

    # 蛇的移动
    def move(self):
        self.pos.pop(-1)
        head = self.pos[0]
        newHead = (head[0] + self.curDir.value[0], head[1] + self.curDir.value[1])
        if self.mode == Mode.Endless:
            newHead = ((newHead[0] + config.horzInrNum) % config.horzInrNum, (newHead[1] + config.vertInrNum) % config.vertInrNum)
        self.pos.insert(0, newHead)

    # 控制方向
    def ctrlDir(self, d: Direction):
        curDir = self.getCurDir()
        if (d == Direction.UP and curDir != Direction.DOWN) or (d == Direction.DOWN and curDir != Direction.UP) or (d == Direction.LEFT and curDir != Direction.RIGHT) or (d == Direction.RIGHT and curDir != Direction.LEFT):
            self.curDir = d

    # 判断是否死亡
    def isDead(self) -> bool:
        head = self.pos[0]
        for pos in self.pos[1:]:
            if head == pos:
                return True
        return self.mode == Mode.Classic and (head[0] < 0 or head[0] > config.horzInrNum - 1 or head[1] < 0 or head[1] > config.vertInrNum - 1)

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
            if pos == self.pos[0]:
                rectL = rect
                rectL.width /= 4
                rectL.height /= 4
                rectR = rectL.copy()
                curDir = self.getCurDir()
                if curDir == Direction.UP:
                    rectL.center = (pos[0] * config.horzInr + config.horzInr / 4, pos[1] * config.vertInr + config.vertInr / 4)
                    rectR.center = (pos[0] * config.horzInr + config.horzInr / 4 * 3, pos[1] * config.vertInr + config.vertInr / 4)
                elif curDir == Direction.DOWN:
                    rectL.center = (pos[0] * config.horzInr + config.horzInr / 4 * 3, pos[1] * config.vertInr + config.vertInr / 4 * 3)
                    rectR.center = (pos[0] * config.horzInr + config.horzInr / 4, pos[1] * config.vertInr + config.vertInr / 4 * 3)
                elif curDir == Direction.LEFT:
                    rectL.center = (pos[0] * config.horzInr + config.horzInr / 4, pos[1] * config.vertInr + config.vertInr / 4 * 3)
                    rectR.center = (pos[0] * config.horzInr + config.horzInr / 4, pos[1] * config.vertInr + config.vertInr / 4)
                elif curDir == Direction.RIGHT:
                    rectL.center = (pos[0] * config.horzInr + config.horzInr / 4 * 3, pos[1] * config.vertInr + config.vertInr / 4)
                    rectR.center = (pos[0] * config.horzInr + config.horzInr / 4 * 3, pos[1] * config.vertInr + config.vertInr / 4 * 3)
                color = (250, 235, 215)
                pygame.draw.ellipse(screen, color, rectL)
                pygame.draw.ellipse(screen, color, rectR)


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


# 标签
class Label:
    # @param text:待渲染文本 font:用来渲染文本使用的字体 color:渲染文本时文本的颜色
    def __init__(self, text: str, font: pygame.font.Font, color: pygame.color.Color):
        self.font = font
        self.text = text
        self.color = color
        self.image = font.render(text, True, color)
        self.rect = self.image.get_rect()

    # @param 将要改变的颜色
    def setColor(self, color: pygame.color.Color):
        self.color = color
        self.image = self.font.render(self.text, True, self.color)

    def draw(self, screen: pygame.surface.Surface):
        screen.blit(self.image, self.rect)


# 按钮
class Button(Label):
    def __init__(self, text: str, font: pygame.font.Font, color: pygame.color.Color):
        super().__init__(text, font, color)
        self.isClickedDown = False

    # 测定pos坐标是否在按钮之内
    # @param pos:鼠标坐标
    def isPosIn(self, pos: tuple):
        return pos[0] >= self.rect.left and pos[0] <= self.rect.right and pos[1] >= self.rect.top and pos[1] <= self.rect.bottom

    def eventHandle(self, event):
        pos = pygame.mouse.get_pos()
        # 模拟点击按钮事件
        if event.type == pygame.MOUSEBUTTONDOWN:
            if self.isPosIn(pos):
                self.isClickedDown = True
        elif event.type == pygame.MOUSEBUTTONUP:
            if self.isPosIn(pos):
                if self.isClickedDown:
                    pygame.event.post(pygame.event.Event(pygame.USEREVENT, sender=self, signal=self.clicked))
            self.isClickedDown = False
        # 显示按钮被鼠标所指
        elif event.type == pygame.MOUSEMOTION:
            if self.isPosIn(pos):
                self.setColor((192, 192, 192))
            else:
                self.setColor((0, 0, 0))

    # 被点击信号
    def clicked(self):
        pass
