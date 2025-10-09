package main

import (
	"snake/console"
	"snake/snake"
	"time"
)

func main() {

	console.HideCursor()
	defer console.ShowCursor()

	snk := snake.New(snake.Right, 1, 5, 5)

	ticker := time.NewTicker(500 * time.Millisecond)
	defer ticker.Stop()
	for range ticker.C {
		snk.Move()

		console.Clear()
		snk.Draw('#')
		console.Flush()
	}

}
