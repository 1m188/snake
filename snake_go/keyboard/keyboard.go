package keyboard

import (
	"github.com/nsf/termbox-go"
)

var inputKeyEv chan termbox.Event // 按键事件
var listen_start bool             // 是否开始监听键盘事件

func listen() {
	for listen_start {
		switch ev := termbox.PollEvent(); ev.Type {
		case termbox.EventKey:
			inputKeyEv <- ev
		}
	}
}

// 开始监听键盘事件
func Listen() {
	inputKeyEv = make(chan termbox.Event)
	listen_start = true
	go listen()
}

// 退出监听键盘事件
func Exit() {
	listen_start = false
	close(inputKeyEv)
}

// 获取按键事件
func GetInputKeyEv() (termbox.Event, bool) {
	select {
	case key, ok := <-inputKeyEv:
		return key, ok
	default:
		return termbox.Event{}, false
	}
}
