#ifndef SCENE_H
#define SCENE_H

//场景类
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void init(); //场景初始化
	virtual int run(); //运行场景
};

#endif // !SCENE_H
