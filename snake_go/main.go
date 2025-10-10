package main

import (
	"snake/console"
	"snake/keyboard"
	"snake/snake"
	"time"

	"github.com/nsf/termbox-go"
)

func main() {

	termbox.Init()
	defer termbox.Close()

	console.HideCursor()
	defer console.Flush()
	defer console.ShowCursor()
	defer console.Clear()

	keyboard.Listen()
	defer keyboard.Exit()

	ticker := time.NewTicker(500 * time.Millisecond)
	defer ticker.Stop()

	snk := snake.New(snake.Right, 1, 5, 5)

gameCircle:
	for range ticker.C {

		if key, ok := keyboard.GetInputKey(); ok {
			switch key {
			case termbox.KeyEsc:
				break gameCircle
			case termbox.KeyArrowUp:
				snk.Dir = snake.Up
			case termbox.KeyArrowDown:
				snk.Dir = snake.Down
			case termbox.KeyArrowLeft:
				snk.Dir = snake.Left
			case termbox.KeyArrowRight:
				snk.Dir = snake.Right
			}
		}

		snk.Move()

		console.Clear()
		snk.Draw('#')
		console.Flush()
	}

}
