from PyQt5.QtCore import QFile, QJsonDocument
from PyQt5.QtWidgets import qApp


#设置类
class Config:
    instance = None

    #单例模式
    def __new__(cls):
        if not cls.instance:
            cls.instance = super().__new__(cls)
        return cls.instance

    #设置初始化
    def init(self):
        #初始化设置默认值
        self.highestScore = 0  #最高分
        self.highestScorePlayer = ""  #最高分保持者
        self.themePath = "theme\\default.qss"  #主题路径

        f = QFile("config.ini")
        #如果设置文件存在则从中读入设置数据
        if f.exists():
            #读取文件
            f.open(QFile.ReadOnly)
            b = f.readAll()
            f.close()

            #解析json
            jsonDocument = QJsonDocument.fromJson(b)
            json = jsonDocument.object()
            self.highestScore = json["HighestScore"].toInt()  #读取最高分
            self.highestScorePlayer = json["HighestScorePlayer"].toString(
            )  #读取最高分保持者
            self.themePath = json["ThemePath"].toString()  #读取当前主题文件路径
        #否则创建设置文件并用默认设置进行游戏
        else:
            f.open(QFile.WriteOnly)
            f.close()

        #设置主题
        self.setTheme(self.themePath)

    #保存设置
    def uninit(self):
        json = {}
        json["HighestScore"] = self.highestScore  #写入最高分
        json["HighestScorePlayer"] = self.highestScorePlayer  #写入最高分保持者
        json["ThemePath"] = self.themePath  #写入主题文件路径

        jsonDocument = QJsonDocument()
        jsonDocument.setObject(json)
        b = jsonDocument.toJson()

        #写入json
        f = QFile("config.ini")
        f.open(QFile.WriteOnly | QFile.Truncate)
        f.write(b)
        f.close()

    #设置主题
    def setTheme(self, themePath):
        f = QFile(themePath)
        f.open(QFile.ReadOnly)
        qApp.setStyleSheet(bytes(f.readAll()).decode("utf-8"))
        f.close()
        self.themePath = themePath
