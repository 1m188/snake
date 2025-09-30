package main

import (
	"snake/console"
)

func main() {
	console.Clear()
	console.HideCursor()
	defer console.ShowCursor()

	console.MoveCursor(1, 5)
	console.Draw('#')
	console.Flush()
}
