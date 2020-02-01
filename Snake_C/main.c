#include "console.h"
#include "scene/game_scene.h"

int main(int argc, char *argv[])
{
    initConsole();

    gameScene();

    return 0;
}