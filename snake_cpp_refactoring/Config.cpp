#include "Config.h"
#include "fstream"
#include "Console.h"

Config *Config::instance = nullptr; //初始化单例指针

Config::Config() :highestScore(0), foreColor(color::BrightWhite), backgroundColor(color::Black)
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
	//如果文件不存在则会创建一个空的配置文件并只读打开
	std::ifstream in;
	in.open("config.ini");
	if (!in)
	{
		std::ofstream out;
		out.open("config.ini");
		out.close();
	}
	else
	{
		//读取文件内容
		in >> highestScore; //读取并初始化最高分
		//读取前景色和背景色
		int temp = 0;
		in >> temp;
		foreColor = static_cast<color>(temp);
		in >> temp;
		backgroundColor = static_cast<color>(temp);
	}
	Console::getInstance()->setWindowColor(foreColor, backgroundColor); //初始化前景色和背景色
	in.close();
}

void Config::end()
{
	//由于之前调用init之后必定存在配置文件，所以直接打开，并且清空内容
	//之后把相关数据重新输入保存
	std::ofstream out;
	out.open("config.ini", std::ios::trunc);

	out << highestScore; //输入并保存最高分
	out << ' ';
	out << foreColor; //输入并保存前景色
	out << ' ';
	out << backgroundColor; //输入并保存背景色

	out.close();
}