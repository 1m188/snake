#导演类
class Director:
    instance = None  #引用单例

    #用new方法以生成唯一单例
    def __new__(cls):
        if not cls.instance:
            cls.instance = super().__new__(cls)
            cls.instance.window = None  #游戏主窗口引用实例
            cls.instance.nowScene = None  #当前受响应场景
        return cls.instance