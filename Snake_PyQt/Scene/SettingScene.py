from Scene.Scene import Scene
from Director import Director
from Scene.ThemeChooseScene import ThemeChooseScene
from PyQt5.QtWidgets import QLabel, QPushButton, QGridLayout
from PyQt5.QtGui import QFont
from PyQt5.QtCore import Qt


#设置场景
class SettingScene(Scene):
    def __init__(self, parentWindow):
        super().__init__(parentWindow)

    #初始化场景
    def init(self):
        #控件
        infoLabel = QLabel(self)
        infoLabel.setAlignment(Qt.AlignCenter)
        infoLabel.setFont(QFont("微软雅黑", 40))
        infoLabel.setText(self.tr("设  置"))

        themeChooseButton = QPushButton(self)
        themeChooseButton.setFont(QFont("微软雅黑", 15))
        themeChooseButton.setText(self.tr("主题选择"))
        themeChooseButton.clicked.connect(self.themeChooseButtonClicked)

        returnToStartSceneButton = QPushButton(self)
        returnToStartSceneButton.setFont(QFont("微软雅黑", 15))
        returnToStartSceneButton.setText(self.tr("返回开始界面"))
        returnToStartSceneButton.clicked.connect(
            self.returnToStartSceneButtonClicked)

        #布局
        layout = QGridLayout(self)
        layout.addWidget(infoLabel, 0, 0, 10, 15)
        layout.addWidget(themeChooseButton, 10, 5, 3, 5)
        layout.addWidget(returnToStartSceneButton, 13, 5, 3, 5)

    #单击主题选择按钮
    def themeChooseButtonClicked(self):
        themeChooseScene = ThemeChooseScene(Director().window)
        Director().nowScene = themeChooseScene
        themeChooseScene.init()
        themeChooseScene.show()
        self.deleteLater()

    #单击返回开始场景按钮
    def returnToStartSceneButtonClicked(self):
        from Scene.StartScene import StartScene
        startScene = StartScene(Director().window)
        Director().nowScene = startScene
        startScene.init()
        startScene.show()
        self.deleteLater()
