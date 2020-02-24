from enum import Enum

# 一些配置属性和全局变量

size = (width, height) = (500, 500)  # 窗口大小

curScene = None  # 当前正在进行的场景

FPS = 60  # 帧率

# 水平/垂直方向间隔数目
horzInrNum = 15
vertInrNum = 15
# 水平/垂直方向间隔大小
horzInr = width / horzInrNum
vertInr = height / vertInrNum

# 蛇移动事件
snakeMoveEventID = 1
snakeMoveTimeInr = 200


# 方向
class Direction(Enum):
    UP = (0, -1)
    DOWN = (0, 1)
    LEFT = (-1, 0)
    RIGHT = (1, 0)


# 模式
class Mode(Enum):
    Classic = "Classic"
    Endless = "Endless"


# 最高分
class HighestScore:
    fileName = "data/highest_score"
    score = {}

    @classmethod
    def loadHighestScore(cls):
        try:
            data = open(cls.fileName)
            lines = data.readlines()
            index = 0
            for mode in Mode:
                cls.score[mode.value] = int(lines[index].strip())
                index += 1
        except FileNotFoundError:
            data = open(cls.fileName, "w")
            for mode in Mode:
                cls.score[mode.value] = 0
        finally:
            data.close()

    @classmethod
    def saveHighestScore(cls):
        with open(cls.fileName, "w") as data:
            for mode in Mode:
                data.write(str(cls.score[mode.value]) + "\n")
