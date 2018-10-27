#ifndef COLORCHOOSESCENE
#define COLORCHOOSESCENE

#include "Scene.h"

//前景色、背景色的选择场景
class ColorChooseScene :public Scene
{
private:
	//模式，意味着当前场景是前景色选择还是背景色的选择
	//为'1'是前景色选择，为'2'是背景色选择
	int mode;

public:
	ColorChooseScene(int mode);
	~ColorChooseScene();

	void init() override;
	int run() override;
};

#endif // !COLORCHOOSESCENE
