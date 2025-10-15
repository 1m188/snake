package scene

import (
	"snake/console"
	"snake/keyboard"
	"time"
)

// 开始场景
type StartScene struct {
	IScene
}

func NewStartScene() *StartScene {
	return &StartScene{}
}

func (s *StartScene) Run() {
	console.HideCursor()
	defer console.Flush()
	defer console.ShowCursor()
	defer console.Clear()

	keyboard.Listen()
	defer keyboard.Exit()

	console.MoveCursor(0, 0)
	console.DrawString("贪吃蛇\n\n")
	console.DrawString("按任意键开始游戏\n")
	console.Flush()

	ticker := time.NewTicker(200 * time.Millisecond)
	defer ticker.Stop()

	for range ticker.C {
		if _, ok := keyboard.GetInputKey(); ok {
			break
		}
	}
}
