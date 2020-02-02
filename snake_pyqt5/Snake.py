#蛇类
class Snake:
    #初始化函数，传入头部坐标，节数和初始方向初始化蛇
    def __init__(self, head, sectionNum, direction):
        #蛇的方向改变时的头部按方向变化
        self.dirChange = [(0, -1), (0, 1), (-1, 0), (1, 0)]

        #初始化初始坐标
        self.dir = direction

        #蛇身坐标
        self.pos = [head]
        for i in range(sectionNum):
            last = self.pos[len(self.pos) - 1]
            self.pos.append([
                last[0] - self.dirChange[self.dir][0],
                last[1] - self.dirChange[self.dir][1]
            ])

    #移动
    def move(self):
        #把原来的尾巴弹出来
        self.pos.pop()
        #获取老头
        head = self.pos[0]
        #把新头压入队列头，作为真正的新头，并压入蛇身数组
        self.pos.insert(0, [
            head[0] + self.dirChange[self.dir][0],
            head[1] + self.dirChange[self.dir][1]
        ])

    #是否死亡
    def isDead(self, up, down, left, right):
        head = self.pos[0]
        #撞墙
        if head[0] < left or head[0] > right or head[1] < up or head[1] > down:
            return True
        #撞到蛇身
        else:
            for i in range(1, len(self.pos)):
                if head[0] == self.pos[i][0] and head[1] == self.pos[i][1]:
                    return True
            return False

    #吃到食物
    def eatFood(self, food):
        head = self.pos[0]
        #如果吃到食物
        if head[0] == food.x and head[1] == food.y:
            #按照相应的节数进行增长
            for i in range(food.bodyGrowth):
                last = self.pos[len(self.pos) - 1]
                secondLast = self.pos[len(self.pos) - 2]
                self.pos.append([
                    last[0] + last[0] - secondLast[0],
                    last[1] + last[1] - secondLast[1]
                ])
            return True
        else:
            return False
