import random


#食物类
class Food:
    def __init__(self):
        #食物坐标
        self.x = 0
        self.y = 0

        #吃到这个食物蛇身增长几节
        self.bodyGrowth = 1

    #更新食物坐标
    #传入上下左右的限制边界，以及蛇身坐标，使新的食物坐标不要和以上坐标重合
    def updatePos(self, up, down, left, right, snakePos):
        newX = 0
        newY = 0
        ok = True
        while ok:
            #给出随机新的食物坐标
            newX = random.randint(left + 1, right - 1)
            newY = random.randint(up + 1, down - 1)
            #如果新的坐标和原来的坐标重合
            if newX == self.x and newY == self.y:
                continue
            #遍历蛇身坐标
            for i in snakePos:
                #倘若有一个坐标重合
                if newX == i[0] and newY == i[1]:
                    ok = False
                    break
            if not ok:
                ok = True
            else:
                ok = False
        #更新坐标
        self.x = newX
        self.y = newY
