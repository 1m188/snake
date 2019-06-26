from Scene.Scene import Scene
from Director import Director
from Config import Config
from Scene.StartScene import StartScene
from PyQt5.QtWidgets import QLabel, QPushButton, QGridLayout
from PyQt5.QtGui import QFont
from PyQt5.QtCore import Qt


#主题选择场景
class ThemeChooseScene(Scene):
    def __init__(self, parentWindow):
        super().__init__(parentWindow)

    #初始化场景
    def init(self):
        #控件
        infoLabel = QLabel(self)
        infoLabel.setAlignment(Qt.AlignCenter)
        infoLabel.setFont(QFont("微软雅黑", 40))
        infoLabel.setText(self.tr("主  题  选  择"))

        defaultButton = QPushButton(self)
        defaultButton.setObjectName("default")
        defaultButton.setFont(QFont("微软雅黑", 15))
        defaultButton.setText(self.tr("默认"))
        defaultButton.clicked.connect(self.themeChooseButtonClicked)

        blackButton = QPushButton(self)
        blackButton.setObjectName("black")
        blackButton.setFont(QFont("微软雅黑", 15))
        blackButton.setText(self.tr("酷炫黑"))
        blackButton.clicked.connect(self.themeChooseButtonClicked)

        grayButton = QPushButton(self)
        grayButton.setObjectName("gray")
        grayButton.setFont(QFont("微软雅黑", 15))
        grayButton.setText(self.tr("金属灰"))
        grayButton.clicked.connect(self.themeChooseButtonClicked)

        yellowButton = QPushButton(self)
        yellowButton.setObjectName("yellow")
        yellowButton.setFont(QFont("微软雅黑", 15))
        yellowButton.setText(self.tr("暖心黄"))
        yellowButton.clicked.connect(self.themeChooseButtonClicked)

        returnToStartSceneButton = QPushButton(self)
        returnToStartSceneButton.setFont(QFont("微软雅黑", 15))
        returnToStartSceneButton.setText(self.tr("返回开始界面"))
        returnToStartSceneButton.clicked.connect(
            self.returnToStartSceneButtonClicked)

        #布局
        layout = QGridLayout(self)
        layout.addWidget(infoLabel, 0, 0, 10, 15)
        layout.addWidget(defaultButton, 10, 5, 2, 5)
        layout.addWidget(blackButton, 12, 5, 2, 5)
        layout.addWidget(grayButton, 14, 5, 2, 5)
        layout.addWidget(yellowButton, 16, 5, 2, 5)
        layout.addWidget(returnToStartSceneButton, 18, 5, 2, 5)

    #单击主题选择按钮
    def themeChooseButtonClicked(self):
        Config().setTheme("theme\\%s.qss" % self.sender().objectName())

    #单击返回开始场景按钮
    def returnToStartSceneButtonClicked(self):
        startScene = StartScene(Director().window)
        Director().nowScene = startScene
        startScene.init()
        startScene.show()
        self.deleteLater()
