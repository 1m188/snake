from Scene.Scene import Scene
from Director import Director
from PyQt5.QtWidgets import QLabel, QPushButton, QGridLayout, QApplication
from PyQt5.QtGui import QFont
from PyQt5.QtCore import Qt


#开始场景
class StartScene(Scene):
    def __init__(self, parentWindow):
        super().__init__(parentWindow)

    #初始化开始场景
    def init(self):
        #控件
        infoLabel = QLabel(self)
        infoLabel.setAlignment(Qt.AlignCenter)
        infoLabel.setFont(QFont("微软雅黑", 40))
        infoLabel.setText(self.tr("贪  吃  蛇"))

        newGameButton = QPushButton(self)
        newGameButton.setFont(QFont("微软雅黑", 15))
        newGameButton.setText(self.tr("开始新游戏"))
        newGameButton.clicked.connect(self.newGameButtonClicked)

        highestScoreButton = QPushButton(self)
        highestScoreButton.setFont(QFont("微软雅黑", 15))
        highestScoreButton.setText(self.tr("最高分"))
        highestScoreButton.clicked.connect(self.highestScoreButtonClicked)

        settingButton = QPushButton(self)
        settingButton.setFont(QFont("微软雅黑", 15))
        settingButton.setText(self.tr("设置"))
        settingButton.clicked.connect(self.settingButtonClicked)

        exitButton = QPushButton(self)
        exitButton.setFont(QFont("微软雅黑", 15))
        exitButton.setText(self.tr("退出"))
        exitButton.clicked.connect(QApplication.quit)

        #布局
        layout = QGridLayout(self)
        layout.addWidget(infoLabel, 0, 0, 10, 15)
        layout.addWidget(newGameButton, 10, 5, 2, 5)
        layout.addWidget(highestScoreButton, 12, 5, 2, 5)
        layout.addWidget(settingButton, 14, 5, 2, 5)
        layout.addWidget(exitButton, 16, 5, 2, 5)

    #单击开始新游戏按钮
    def newGameButtonClicked(self):
        from Scene.SpeedChooseScene import SpeedChooseScene
        speedChooseScene = SpeedChooseScene(Director().window)
        Director().nowScene = speedChooseScene
        speedChooseScene.init()
        speedChooseScene.show()
        self.deleteLater()

    #单击最高分按钮
    def highestScoreButtonClicked(self):
        from Scene.HighestScoreScene import HighestScoreScene
        highestScoreScene = HighestScoreScene(Director().window)
        Director().nowScene = highestScoreScene
        highestScoreScene.init()
        highestScoreScene.show()
        self.deleteLater()

    #单击设置按钮
    def settingButtonClicked(self):
        from Scene.SettingScene import SettingScene
        settingScene = SettingScene(Director().window)
        Director().nowScene = settingScene
        settingScene.init()
        settingScene.show()
        self.deleteLater()
