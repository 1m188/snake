#include "ThemeChooseScene.h"
#include "Director.h"
#include "Config.h"
#include "StartScene.h"
#include "QLabel"
#include "QPushButton"
#include "QGridLayout"

ThemeChooseScene::ThemeChooseScene(Window *parent)
	: Scene(parent)
{

}

ThemeChooseScene::~ThemeChooseScene()
{

}

void ThemeChooseScene::init()
{
	//¿Ø¼þ
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 40));
	infoLabel->setText(tr(u8"Ö÷  Ìâ  Ñ¡  Ôñ"));

	QPushButton *defaultButton = new QPushButton(this);
	defaultButton->setObjectName("default");
	defaultButton->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 15));
	defaultButton->setText(tr(u8"Ä¬ÈÏ"));
	connect(defaultButton, &QPushButton::clicked, this, &ThemeChooseScene::themeChooseButtonClicked);

	QPushButton *blackButton = new QPushButton(this);
	blackButton->setObjectName("black");
	blackButton->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 15));
	blackButton->setText(tr(u8"¿áìÅºÚ"));
	connect(blackButton, &QPushButton::clicked, this, &ThemeChooseScene::themeChooseButtonClicked);

	QPushButton *grayButton = new QPushButton(this);
	grayButton->setObjectName("gray");
	grayButton->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 15));
	grayButton->setText(tr(u8"½ðÊô»Ò"));
	connect(grayButton, &QPushButton::clicked, this, &ThemeChooseScene::themeChooseButtonClicked);

	QPushButton *yellowButton = new QPushButton(this);
	yellowButton->setObjectName("yellow");
	yellowButton->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 15));
	yellowButton->setText(tr(u8"Å¯ÐÄ»Æ"));
	connect(yellowButton, &QPushButton::clicked, this, &ThemeChooseScene::themeChooseButtonClicked);

	QPushButton *returnToStartSceneButton = new QPushButton(this);
	returnToStartSceneButton->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 15));
	returnToStartSceneButton->setText(tr(u8"·µ»Ø¿ªÊ¼½çÃæ"));
	connect(returnToStartSceneButton, &QPushButton::clicked, this, &ThemeChooseScene::returnToStartSceneButtonClicked);

	//²¼¾Ö
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(infoLabel, 0, 0, 10, 15);
	layout->addWidget(defaultButton, 10, 5, 2, 5);
	layout->addWidget(blackButton, 12, 5, 2, 5);
	layout->addWidget(grayButton, 14, 5, 2, 5);
	layout->addWidget(yellowButton, 16, 5, 2, 5);
	layout->addWidget(returnToStartSceneButton, 18, 5, 2, 5);
}

void ThemeChooseScene::themeChooseButtonClicked()
{
	Config::getInstance()->setTheme(QString(":/theme/Resoueces/theme/%1.qss").arg(sender()->objectName()));
}

void ThemeChooseScene::returnToStartSceneButtonClicked()
{
	StartScene *startScene = new StartScene(Director::getInstance()->getWindow());
	Director::getInstance()->setNowScene(startScene);
	startScene->init();
	startScene->show();
	deleteLater();
}