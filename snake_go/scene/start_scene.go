package scene

import (
	"snake/console"
	"snake/keyboard"
	"time"

	"github.com/nsf/termbox-go"
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

	width, height := termbox.Size()
	console.MoveCursor(height/2-5, width/2-2)
	console.DrawString("贪吃蛇\n\n")
	console.MoveCursor(height/2, width/2-6)
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
