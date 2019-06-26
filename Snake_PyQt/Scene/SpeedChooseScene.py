from Scene.Scene import Scene
from Director import Director
from Scene.GameScene import GameScene
from PyQt5.QtWidgets import QLabel, QPushButton, QGridLayout
from PyQt5.QtGui import QFont
from PyQt5.QtCore import Qt


#速度选择场景
class SpeedChooseScene(Scene):
    def __init__(self, parentWindow):
        super().__init__(parentWindow)

    def init(self):
        #控件
        infoLabel = QLabel(self)
        infoLabel.setAlignment(Qt.AlignCenter)
        infoLabel.setFont(QFont("微软雅黑", 40))
        infoLabel.setText(self.tr("速  度  选  择"))

        easyButton = QPushButton(self)
        easyButton.setObjectName("0")
        easyButton.setFont(QFont("微软雅黑", 15))
        easyButton.setText(self.tr("慢速"))
        easyButton.clicked.connect(self.speedChooseButtonClicked)

        mediumButton = QPushButton(self)
        mediumButton.setObjectName("1")
        mediumButton.setFont(QFont("微软雅黑", 15))
        mediumButton.setText(self.tr("中速"))
        mediumButton.clicked.connect(self.speedChooseButtonClicked)

        difficultButton = QPushButton(self)
        difficultButton.setObjectName("2")
        difficultButton.setFont(QFont("微软雅黑", 15))
        difficultButton.setText(self.tr("快速"))
        difficultButton.clicked.connect(self.speedChooseButtonClicked)

        veryDifficultButton = QPushButton(self)
        veryDifficultButton.setObjectName("3")
        veryDifficultButton.setFont(QFont("微软雅黑", 15))
        veryDifficultButton.setText(self.tr("光速"))
        veryDifficultButton.clicked.connect(self.speedChooseButtonClicked)

        returnToStartSceneButton = QPushButton(self)
        returnToStartSceneButton.setFont(QFont("微软雅黑", 15))
        returnToStartSceneButton.setText(self.tr("返回开始界面"))
        returnToStartSceneButton.clicked.connect(
            self.returnToStartSceneButtonClicked)

        #布局
        layout = QGridLayout(self)
        layout.addWidget(infoLabel, 0, 0, 10, 15)
        layout.addWidget(easyButton, 10, 5, 2, 5)
        layout.addWidget(mediumButton, 12, 5, 2, 5)
        layout.addWidget(difficultButton, 14, 5, 2, 5)
        layout.addWidget(veryDifficultButton, 16, 5, 2, 5)
        layout.addWidget(returnToStartSceneButton, 18, 5, 2, 5)

    #单击速度选择按钮
    def speedChooseButtonClicked(self):
        gameScene = GameScene(Director().window)
        Director().nowScene = gameScene
        gameScene.init(int(self.sender().objectName()))
        gameScene.show()
        self.deleteLater()

    #单击返回开始场景按钮
    def returnToStartSceneButtonClicked(self):
        from Scene.StartScene import StartScene
        startScene = StartScene(Director().window)
        Director().nowScene = startScene
        startScene.init()
        startScene.show()
        self.deleteLater()
