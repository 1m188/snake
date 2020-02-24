from enum import Enum
import json

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
    Classic = "Classic"  # 经典模式
    Endless = "Endless"  # 无尽模式


# 最高分
class HighestScore:
    fileName = "data/highest_score"  # 最高分文件名
    score = {}

    @classmethod
    def loadHighestScore(cls):
        try:
            data = open(cls.fileName)
            cls.score = json.loads(data.read())
        except FileNotFoundError:
            data = open(cls.fileName, "w")
            for mode in Mode:
                cls.score[mode.value] = 0
        finally:
            data.close()

    @classmethod
    def saveHighestScore(cls):
        with open(cls.fileName, "w") as data:
            data.write(json.dumps(cls.score))
