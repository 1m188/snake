#include "Config.h"
#include "QFile"
#include "QApplication"
#include "QJsonObject"
#include "QJsonDocument"

Config *Config::instance = nullptr; //初始化单例指针

Config::Config()
{

}

Config::~Config()
{
}

Config * Config::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Config();
	}
	return instance;
}

void Config::init()
{
	highestScore = 0; //初始化最高分
	highestScorePlayer = "None"; //...最高分保持者
	themeResourceUrl = ":/theme/Resoueces/theme/default.qss"; //...主题资源url

	QFile configFile("config.ini");
	//如果文件不存在则创建一个并使用默认设置开始游戏，否则从文件中读取相关的设置数据
	if (!configFile.open(QIODevice::ReadOnly))
	{
		QFile newConfigFile("config.ini");
		newConfigFile.open(QIODevice::WriteOnly);
		newConfigFile.close();
	}
	else
	{
		//从文件读出json
		QByteArray byteArray = configFile.readAll();
		configFile.close();

		//json格式解析
		QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray);
		QJsonObject json = jsonDocument.object();
		highestScore = json.value("HighestScore").toInt(); //从文件读入最高分
		highestScorePlayer = json.value("HighestScorePlayer").toString(); //...最高分保持者
		themeResourceUrl = json.value("ThemeResourceUrl").toString(); //...主题文件路径
	}
	setTheme(themeResourceUrl); //设置主题
}

void Config::uninit()
{
	QJsonObject json;
	json.insert("HighestScore", highestScore); //保存最高分
	json.insert("HighestScorePlayer", highestScorePlayer); //...最高分保持者
	json.insert("ThemeResourceUrl", themeResourceUrl); //...主题文件路径

	QJsonDocument jsonDocument;
	jsonDocument.setObject(json);
	QByteArray byteArray = jsonDocument.toJson();

	//对文件以写打开，并且清空文件内容
	QFile configFile("config.ini");
	configFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
	//写入json
	configFile.write(byteArray);
	//关闭文件
	configFile.close();
}

void Config::setHighestScore(int highestScore)
{
	this->highestScore = highestScore;
}

int Config::getHighestScore()
{
	return highestScore;
}

void Config::setHighestScorePlayer(QString highestScorePlayer)
{
	this->highestScorePlayer = highestScorePlayer;
}

QString Config::getHighestScorePlayer()
{
	return highestScorePlayer;
}

void Config::setTheme(QString themeResourceUrl)
{
	this->themeResourceUrl = themeResourceUrl;
	QFile qss(themeResourceUrl);
	if (!qss.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet("");
	}
	else
	{
		qApp->setStyleSheet(qss.readAll());
		qss.close();
	}
}