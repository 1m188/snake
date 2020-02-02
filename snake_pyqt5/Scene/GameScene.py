from Scene.Scene import Scene
from Director import Director
from Config import Config
from Scene.StartScene import StartScene
from PyQt5.QtWidgets import QMessageBox, QInputDialog
from PyQt5.QtGui import QPalette, QColor, QPainter, QBrush, QFont
from PyQt5.QtCore import Qt, QTimer
from Sprite.Snake import Snake
from Sprite.Food import Food


#游戏主场景
class GameScene(Scene):
    def __init__(self, parentWindow):
        super().__init__(parentWindow)

    #初始化场景
    #传入从0开始，1为递增量的难度系数
    def init(self, difficulty):
        #获取宽高方向地图线的数目
        self.spaceNum = 20

        #获取宽和高方向的间隔
        self.widthSpace = int(self.width() / self.spaceNum)
        self.heightSpace = int(self.height() / self.spaceNum)

        #设定FPS值
        #平常的按阶梯递增的难度
        if difficulty == 3:
            self.fps = 1000
        #光速难度
        else:
            self.fps = difficulty * 5 + 5

        #初始化蛇
        self.snake = Snake([5, 5], 2, 3)
        #初始化食物
        self.food = Food()
        self.food.bodyGrowth = difficulty + 1
        self.food.updatePos(-1, self.spaceNum, -1, self.spaceNum,
                            self.snake.pos)

        #初始化当前分数
        self.score = 0
        #设置分数增长
        self.scoreGrowth = difficulty + 1

        #设置背景颜色
        self.setAutoFillBackground(True)
        self.setPalette(QPalette(QColor(255, 255, 255)))

        #绘制画面定时器
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.gameCycle)
        self.timer.setInterval(1000 / self.fps)
        self.timer.start()

    #重写按键事件
    def keyPressEvent(self, event):
        if not event.isAutoRepeat():
            if event.key() == Qt.Key_Up and abs(
                    self.snake.pos[0][1] - self.snake.pos[1][1]) != 1:
                self.snake.dir = 0
            elif event.key() == Qt.Key_Down and abs(
                    self.snake.pos[0][1] - self.snake.pos[1][1]) != 1:
                self.snake.dir = 1
            elif event.key() == Qt.Key_Left and abs(
                    self.snake.pos[0][0] - self.snake.pos[1][0]) != 1:
                self.snake.dir = 2
            elif event.key() == Qt.Key_Right and abs(
                    self.snake.pos[0][0] - self.snake.pos[1][0]) != 1:
                self.snake.dir = 3
            elif event.key() == Qt.Key_Space:
                self.timer.setInterval(self.timer.interval() / 3)
        return super().keyPressEvent(event)

    #按键释放的时候的事件
    def keyReleaseEvent(self, event):
        if not event.isAutoRepeat():
            if event.key() == Qt.Key_Space:
                self.timer.setInterval(self.timer.interval() * 3)
        return super().keyReleaseEvent(event)

    #游戏循环
    def gameCycle(self):
        #蛇的移动
        self.snake.move()

        #如果吃到了食物
        if self.snake.eatFood(self.food):
            #刷新分数
            self.score += self.scoreGrowth
            #刷新食物坐标
            self.food.updatePos(-1, self.spaceNum, -1, self.spaceNum,
                                self.snake.pos)

        #判断游戏是否结束
        if self.snake.isDead(0, self.spaceNum - 1, 0, self.spaceNum - 1):
            self.gameEnd()

        #更新界面，触发paintEvent事件重绘界面
        self.update()

    #绘制事件
    def paintEvent(self, event):
        painter = QPainter(self)

        #绘制背景线
        for i in range(self.spaceNum):
            painter.drawLine(i * self.widthSpace, 0, i * self.widthSpace,
                             self.height())
        for i in range(self.spaceNum):
            painter.drawLine(0, i * self.heightSpace, self.width(),
                             i * self.heightSpace)

        #绘制蛇身
        painter.setBrush(QBrush(QColor(0, 0, 255)))
        for i, j in self.snake.pos:
            painter.drawRect(i * self.widthSpace, j * self.heightSpace,
                             self.widthSpace, self.heightSpace)

        #绘制食物
        painter.setBrush(QBrush(QColor(255, 0, 0)))
        painter.drawRect(self.food.x * self.widthSpace,
                         self.food.y * self.heightSpace, self.widthSpace,
                         self.heightSpace)

        #绘制分数
        painter.setPen(Qt.red)
        painter.setFont(QFont("微软雅黑", 13, 5))
        painter.drawText(0, 0, self.width(), 25, Qt.AlignCenter,
                         "分数：%d" % self.score)

        painter.end()
        return super().paintEvent(event)

    #游戏结束
    def gameEnd(self):
        #定时器停止
        self.timer.stop()

        #判断是否是最高分
        #如果是的话，则保存最高分并且录入保持者名字
        if self.score > Config().highestScore:
            Config().highestScore = self.score
            highestScorePlayer = ""
            #必须有纪录保持者
            while highestScorePlayer == "":
                highestScorePlayer = QInputDialog.getText(
                    self, self.tr("最高分！"),
                    self.tr("恭喜你的分数 %d 成为最高分，请留下你的尊姓大名。") % self.score)[0]
            #录入最高分纪录保持者
            Config().highestScorePlayer = highestScorePlayer
        #判断是否要再来一局
        #如果不要的话回到开始场景
        if QMessageBox.information(
                self, self.tr("游戏结束"),
                self.tr("您的得分是：%d，是否要以当前难度再来一局？") % self.score,
                QMessageBox.Yes, QMessageBox.No) == QMessageBox.No:
            startScene = StartScene(Director().window)
            Director().nowScene = startScene
            startScene.init()
            startScene.show()
            self.deleteLater()
        #否则将游戏的各项数据设置复原
        else:
            #重置蛇和食物
            self.snake = Snake([5, 5], 2, 3)
            self.food.updatePos(-1, self.spaceNum, -1, self.spaceNum,
                                self.snake.pos)
            #重置分数
            self.score = 0
            #重置绘制定时器
            self.timer.setInterval(1000 / self.fps)
            self.timer.start()
