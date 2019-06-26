from PyQt5.QtWidgets import QWidget, QApplication
from Director import Director
from Scene.StartScene import StartScene


#游戏主窗口
class Window(QWidget):
    def __init__(self):
        super().__init__(None)

        #设置窗口标题和固定大小
        self.setWindowTitle(self.tr("贪吃蛇"))
        self.setFixedSize(940, 940)

        #移动到屏幕中央
        rect = self.frameGeometry()
        rect.moveCenter(QApplication.desktop().availableGeometry().center())
        self.move(rect.topLeft())

    #以下两个函数是当有按键事件发生的时候能够将按键事件截取并且传递给当前受响应的场景响应
    def keyPressEvent(self, event):
        Director().nowScene.keyPressEvent(event)

    def keyReleaseEvent(self, event):
        Director().nowScene.keyReleaseEvent(event)

    #重写show函数，显示的时候带出开始场景
    def show(self):
        startScene = StartScene(self)
        Director().nowScene = startScene
        startScene.init()
        startScene.show()
        super().show()
