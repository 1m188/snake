package scene

import (
	"snake/console"
	"snake/keyboard"
	"time"

	"github.com/nsf/termbox-go"
)

// 结束场景
type EndScene struct {
	IScene
}

func NewEndScene() *EndScene {
	return &EndScene{}
}

func (s *EndScene) Run() {
	console.HideCursor()
	defer console.Flush()
	defer console.ShowCursor()
	defer console.Clear()

	keyboard.Listen()
	defer keyboard.Exit()

	width, height := termbox.Size()

	title := "游戏结束！"
	pressAnyKey := "按任意键返回开始界面"
	console.MoveCursor(height/2-5, width/2-len(title)/2)
	console.DrawString(title)
	console.MoveCursor(height/2+5, width/2-len(pressAnyKey)/2+3)
	console.DrawString(pressAnyKey)
	console.Flush()

	ticker := time.NewTicker(200 * time.Millisecond)
	defer ticker.Stop()

	for range ticker.C {
		if _, ok := keyboard.GetInputKeyEv(); ok {
			break
		}
	}
}
