package main

import (
	"snake/console"
	"time"
)

func main() {

	console.HideCursor()
	defer console.ShowCursor()

	row, col := 1, 5

	ticker := time.NewTicker(500 * time.Millisecond)
	defer ticker.Stop()
	for range ticker.C {
		col++

		console.Clear()
		console.MoveCursor(row, col)
		console.Draw('#')
		console.Flush()
	}

}
