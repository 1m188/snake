from PyQt5.QtWidgets import QApplication
import sys
from Director import Director
from Window import Window
from Config import Config

app = QApplication(sys.argv)
Config().init()
Director().window = Window()
Director().window.show()
app.exec()
Config().uninit()
