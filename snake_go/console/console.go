package console

import (
	"fmt"
)

// 清屏并把光标移到左上角
func Clear() { fmt.Print("\033[2J\033[H") }

// 隐藏光标
func HideCursor() { fmt.Print("\033[?25l") }

// 显示光标
func ShowCursor() { fmt.Print("\033[?25h") }

// 设置光标位置
func MoveCursor(rowIndex, colIndex int) { fmt.Printf("\033[%d;%dH", rowIndex+1, colIndex+1) }
