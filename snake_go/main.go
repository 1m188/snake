package main

import (
	"fmt"
	"snake/console"
)

func main() {
	console.Clear()
	console.HideCursor()
	defer console.ShowCursor()
	console.MoveCursor(1, 5)
	fmt.Println("#")
}
