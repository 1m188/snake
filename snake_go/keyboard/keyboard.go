package keyboard

import (
	"github.com/nsf/termbox-go"
)

var inputKey chan termbox.Key // 输入按键
var listen_start bool         // 是否开始监听键盘事件

func listen() {
	for listen_start {
		switch ev := termbox.PollEvent(); ev.Type {
		case termbox.EventKey:
			inputKey <- ev.Key
		}
	}
}

// 开始监听键盘事件
func Listen() {
	inputKey = make(chan termbox.Key)
	listen_start = true
	go listen()
}

// 退出监听键盘事件
func Exit() {
	listen_start = false
	close(inputKey)
}

// 获取按键
func GetInputKey() (termbox.Key, bool) {
	select {
	case key, ok := <-inputKey:
		return key, ok
	default:
		return 0, false
	}
}
