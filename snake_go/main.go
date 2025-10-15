package main

import (
	"snake/scene"

	"github.com/nsf/termbox-go"
)

func main() {

	termbox.Init()
	defer termbox.Close()

	gameScene := scene.NewGameScene()
	gameScene.Run()

}
