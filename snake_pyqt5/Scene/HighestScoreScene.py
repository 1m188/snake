from Scene.Scene import Scene
from Director import Director
from Config import Config
from PyQt5.QtWidgets import QLabel, QPushButton, QGridLayout
from PyQt5.QtGui import QFont
from PyQt5.QtCore import Qt


#最高分场景
class HighestScoreScene(Scene):
    def __init__(self, parentWindow):
        super().__init__(parentWindow)

    #初始化场景
    def init(self):
        #控件
        infoLabel = QLabel(self)
        infoLabel.setAlignment(Qt.AlignCenter)
        infoLabel.setFont(QFont("微软雅黑", 40))
        infoLabel.setText(self.tr("最  高  分"))

        highestScoreLabel = QLabel(self)
        highestScoreLabel.setAlignment(Qt.AlignCenter)
        highestScoreLabel.setFont(QFont("微软雅黑", 25))
        highestScoreLabel.setText("%d%s      %s" %
                                  (Config().highestScore, self.tr("分"),
                                   Config().highestScorePlayer))

        returnToStartSceneButton = QPushButton(self)
        returnToStartSceneButton.setFont(QFont("微软雅黑", 15))
        returnToStartSceneButton.setText(self.tr("返回开始界面"))
        returnToStartSceneButton.clicked.connect(
            self.returnToStartSceneButtonClicked)

        #布局
        layout = QGridLayout(self)
        layout.addWidget(infoLabel, 0, 0, 10, 15)
        layout.addWidget(highestScoreLabel, 10, 5, 3, 5)
        layout.addWidget(returnToStartSceneButton, 13, 5, 3, 5)

    #单击返回开始场景按钮
    def returnToStartSceneButtonClicked(self):
        from Scene.StartScene import StartScene
        startScene = StartScene(Director().window)
        Director().nowScene = startScene
        startScene.init()
        startScene.show()
        self.deleteLater()
