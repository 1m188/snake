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

	title := "贪吃蛇"
	chose1 := "1. 经典模式"
	chose2 := "2. 无界模式"
	pressAnyKey := "按对应数字键继续"
	console.MoveCursor(height/2-5, width/2-len(title)/2)
	console.DrawString(title)
	console.MoveCursor(height/2-1, width/2-len(chose1)/2)
	console.DrawString(chose1)
	console.MoveCursor(height/2+1, width/2-len(chose2)/2)
	console.DrawString(chose2)
	console.MoveCursor(height/2+3, width/2-len(pressAnyKey)/2+3)
	console.DrawString(pressAnyKey)
	console.Flush()

	ticker := time.NewTicker(200 * time.Millisecond)
	defer ticker.Stop()

	for range ticker.C {
		if ev, ok := keyboard.GetInputKeyEv(); ok && ev.Ch == '1' {
			break
		}
	}
}
