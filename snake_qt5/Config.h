#ifndef CONFIG_H
#define CONFIG_H

#include "QString"

//设置类
class Config
{
private:
	Config(); //禁止外部自动构造

	static Config *instance; //单例指针

	int highestScore; //最高分
	QString highestScorePlayer; //最高分保持者
	QString themeResourceUrl; //主题资源url

public:
	~Config();
	Config(const Config &) = delete;
	Config &operator=(const Config &) = delete;

	static Config *getInstance(); //获取单例指针

	void init(); //初始化基本设置
	void uninit(); //保存基本设置

	void setHighestScore(int highestScore); //设置最高分
	int getHighestScore(); //获得最高分
	void setHighestScorePlayer(QString highestScorePlayer); //设置最高分保持者
	QString getHighestScorePlayer(); //获取最高分保持者
	void setTheme(QString themePath); //设置当前主题
};

#endif // !CONFIG_H
