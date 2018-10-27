#ifndef CONFIG_H
#define CONFIG_H

//用来储存游戏的某些全局数据的单例类
class Config
{
private:
	Config(); //禁止外部自动构造

	static Config *instance; //唯一实例化句柄

public:
	Config(const Config &) = delete; //禁止复制构造
	Config &operator=(const Config &) = delete; //禁止赋值构造
	~Config();

	static Config *getInstance(); //获取单例
	void init(); //从文件中初始化相关数据
	void end(); //游戏结束之后的数据处理

	//可设置为背景色或者前景色的颜色枚举
	const enum color
	{
		Black,
		Blue,
		Green,
		LakeBlue,
		Red,
		Purple,
		Yellow,
		White,
		Gray,
		LightBlue,
		LightGreen,
		PaleLightGreen,
		LightRed,
		LightPurple,
		LightYellow,
		BrightWhite
	};
	color foreColor; //前景色
	color backgroundColor; //背景色
	int highestScore; //最高分
};

#endif // !CONFIG_H
