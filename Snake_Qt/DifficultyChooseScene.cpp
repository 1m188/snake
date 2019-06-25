#include "DifficultyChooseScene.h"
#include "Director.h"
#include "StartScene.h"
#include "GameScene.h"
#include "QLabel"
#include "QPushButton"
#include "QGridLayout"

DifficultyChooseScene::DifficultyChooseScene(Window *parent)
	: Scene(parent)
{
}

DifficultyChooseScene::~DifficultyChooseScene()
{
}

void DifficultyChooseScene::init()
{
	//控件
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"微软雅黑", 40));
	infoLabel->setText(tr(u8"速  度  选  择"));

	QPushButton *slowSpeedButton = new QPushButton(this);
	slowSpeedButton->setObjectName("0");
	slowSpeedButton->setFont(QFont(u8"微软雅黑", 15));
	slowSpeedButton->setText(tr(u8"慢速"));
	connect(slowSpeedButton, &QPushButton::clicked, this, &DifficultyChooseScene::difficultyChooseButtonClicked);

	QPushButton *mediumSpeedButton = new QPushButton(this);
	mediumSpeedButton->setObjectName("1");
	mediumSpeedButton->setFont(QFont(u8"微软雅黑", 15));
	mediumSpeedButton->setText(tr(u8"中速"));
	connect(mediumSpeedButton, &QPushButton::clicked, this, &DifficultyChooseScene::difficultyChooseButtonClicked);

	QPushButton *fastSpeedButton = new QPushButton(this);
	fastSpeedButton->setObjectName("2");
	fastSpeedButton->setFont(QFont(u8"微软雅黑", 15));
	fastSpeedButton->setText(tr(u8"快速"));
	connect(fastSpeedButton, &QPushButton::clicked, this, &DifficultyChooseScene::difficultyChooseButtonClicked);

	QPushButton *returnToStartSceneButton = new QPushButton(this);
	returnToStartSceneButton->setFont(QFont(u8"微软雅黑", 15));
	returnToStartSceneButton->setText(tr(u8"返回开始界面"));
	connect(returnToStartSceneButton, &QPushButton::clicked, this, &DifficultyChooseScene::returnToStartSceneButtonClicked);

	//布局
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(infoLabel, 0, 0, 10, 15);
	layout->addWidget(slowSpeedButton, 10, 5, 2, 5);
	layout->addWidget(mediumSpeedButton, 12, 5, 2, 5);
	layout->addWidget(fastSpeedButton, 14, 5, 2, 5);
	layout->addWidget(returnToStartSceneButton, 16, 5, 2, 5);
}

void DifficultyChooseScene::difficultyChooseButtonClicked()
{
	GameScene *gameScene = new GameScene(Director::getInstance()->getWindow());
	Director::getInstance()->setNowScene(gameScene);
	gameScene->init(sender()->objectName().toInt());
	gameScene->show();
	deleteLater();
}

void DifficultyChooseScene::returnToStartSceneButtonClicked()
{
	StartScene *startScene = new StartScene(Director::getInstance()->getWindow());
	Director::getInstance()->setNowScene(startScene);
	startScene->init();
	startScene->show();
	deleteLater();
}