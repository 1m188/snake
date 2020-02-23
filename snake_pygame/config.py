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


# 最高分
class HighestScore:
    fileName = "data"

    @classmethod
    def loadHighestScore(cls):
        try:
            data = open(cls.fileName)
            contents = data.read().rstrip().split('\n')
            cls.classicHighestScore = int(contents[0])
            cls.endlessHighestScore = int(contents[1])
        except FileNotFoundError:
            data = open(cls.fileName, "w")
            cls.classicHighestScore = 0
            cls.endlessHighestScore = 0
        finally:
            data.close()

    @classmethod
    def saveHighestScore(cls):
        with open(cls.fileName, "w") as data:
            data.write(str(cls.classicHighestScore) + "\n")
            data.write(str(cls.endlessHighestScore) + "\n")
