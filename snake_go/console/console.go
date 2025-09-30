/* 所有控制台相关的输入输出的封装 */

package console

import (
	"bufio"
	"fmt"
	"os"
)

var out *bufio.Writer

func init() {
	out = bufio.NewWriter(os.Stdout)
}

// 清屏并把光标移到左上角
func Clear() { out.WriteString("\033[2J\033[H") }

// 隐藏光标
func HideCursor() { out.WriteString("\033[?25l") }

// 显示光标
func ShowCursor() { out.WriteString("\033[?25h") }

// 设置光标位置
func MoveCursor(rowIndex, colIndex int) { fmt.Fprintf(out, "\033[%d;%dH", rowIndex+1, colIndex+1) }

// 绘制
func Draw(b byte) { out.WriteByte(b) }

// 刷新终端
func Flush() { out.Flush() }
