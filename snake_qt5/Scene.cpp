#include "Scene.h"

Scene::Scene(Window *parent)
	: QWidget(parent)
{
	resize(parent->size());
}

Scene::~Scene()
{
}

void Scene::init()
{
}

void Scene::keyPressEvent(QKeyEvent *event)
{
	return QWidget::keyPressEvent(event);
}

void Scene::keyReleaseEvent(QKeyEvent *event)
{
	return QWidget::keyReleaseEvent(event);
}