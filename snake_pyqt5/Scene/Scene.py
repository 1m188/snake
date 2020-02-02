from PyQt5.QtWidgets import QWidget


#场景类
class Scene(QWidget):
    def __init__(self, parentWindow):
        super().__init__(parentWindow)
        self.resize(parentWindow.size())

    def init(self):
        pass