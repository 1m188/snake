package main

import (
	"math/rand"
	"snake/console"
	"snake/food"
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
	fod := food.New(1, 7, 1)

gameCircle:
	for range ticker.C {

		width, height := termbox.Size()

		if key, ok := keyboard.GetInputKey(); ok {
			switch key {
			case termbox.KeyEsc:
				break gameCircle
			case termbox.KeyArrowUp:
				if snk.Dir != snake.Down {
					snk.Dir = snake.Up
				}
			case termbox.KeyArrowDown:
				if snk.Dir != snake.Up {
					snk.Dir = snake.Down
				}
			case termbox.KeyArrowLeft:
				if snk.Dir != snake.Right {
					snk.Dir = snake.Left
				}
			case termbox.KeyArrowRight:
				if snk.Dir != snake.Left {
					snk.Dir = snake.Right
				}
			}
		}

		snk.Move()

		if snk.Pos[0].R == fod.Row && snk.Pos[0].C == fod.Col {
			r, c := 0, 0
			f := true
			for f {
				f = false
				r = rand.Intn(height-2) + 1
				c = rand.Intn(width-2) + 1
				for _, v := range snk.Pos {
					if v.R == r && v.C == c {
						f = true
						break
					}
				}
			}
			fod = food.New(r, c, 1)
		}

		console.Clear()
		snk.Draw('#')
		console.Flush()
	}

}
