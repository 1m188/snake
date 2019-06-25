#include "SettingScene.h"
#include "Director.h"
#include "StartScene.h"
#include "ThemeChooseScene.h"
#include "QLabel"
#include "QPushButton"
#include "QGridLayout"

SettingScene::SettingScene(Window *parent)
	: Scene(parent)
{

}

SettingScene::~SettingScene()
{

}

void SettingScene::init()
{
	//控件
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"微软雅黑", 40));
	infoLabel->setText(tr(u8"设  置"));

	QPushButton *themeChooseButton = new QPushButton(this);
	themeChooseButton->setFont(QFont(u8"微软雅黑", 15));
	themeChooseButton->setText(tr(u8"主题选择"));
	connect(themeChooseButton, &QPushButton::clicked, this, &SettingScene::themeChooseButtonClicked);

	QPushButton *returnToStartSceneButton = new QPushButton(this);
	returnToStartSceneButton->setFont(QFont(u8"微软雅黑", 15));
	returnToStartSceneButton->setText(tr(u8"返回开始界面"));
	connect(returnToStartSceneButton, &QPushButton::clicked, this, &SettingScene::returnToStartSceneButtonClicked);

	//布局
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(infoLabel, 0, 0, 10, 15);
	layout->addWidget(themeChooseButton, 10, 5, 3, 5);
	layout->addWidget(returnToStartSceneButton, 13, 5, 3, 5);
}

void SettingScene::themeChooseButtonClicked()
{
	ThemeChooseScene *themeChooseScene = new ThemeChooseScene(Director::getInstance()->getWindow());
	Director::getInstance()->setNowScene(themeChooseScene);
	themeChooseScene->init();
	themeChooseScene->show();
	deleteLater();
}

void SettingScene::returnToStartSceneButtonClicked()
{
	StartScene *startScene = new StartScene(Director::getInstance()->getWindow());
	Director::getInstance()->setNowScene(startScene);
	startScene->init();
	startScene->show();
	deleteLater();
}