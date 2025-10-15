package scene

import (
	"math/rand"
	"snake/console"
	"snake/food"
	"snake/keyboard"
	"snake/snake"
	"time"

	"github.com/nsf/termbox-go"
)

// 游戏场景
type GameScene struct {
	IScene
}

func NewGameScene() *GameScene {
	return &GameScene{}
}

func (s *GameScene) Run() {
	console.HideCursor()
	defer console.Flush()
	defer console.ShowCursor()
	defer console.Clear()

	keyboard.Listen()
	defer keyboard.Exit()

	timeFrame := 400 // 每帧的时间 ms
	speedCnt := 1    // 加速档位
	MAXSPEEDCNT := 5 // 最大加速档位
	ticker := time.NewTicker(time.Duration(timeFrame/speedCnt) * time.Millisecond)
	defer ticker.Stop()

	snk := snake.New(snake.Right, 1, 5, 5)
	fod := food.New(1, 7, 1)

gameCircle:
	for {
		<-ticker.C

		width, height := termbox.Size()

		if ev, ok := keyboard.GetInputKeyEv(); ok && ev.Ch == 0 {
			switch ev.Key {
			case termbox.KeyEsc:
				break gameCircle
			case termbox.KeySpace:
				ticker.Stop()
				speedCnt = speedCnt%MAXSPEEDCNT + 1
				ticker = time.NewTicker(time.Duration(timeFrame/speedCnt) * time.Millisecond)
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
		if snk.IsKnockSelf() {
			break gameCircle
		}

		if snk.Pos[0].R == fod.Row && snk.Pos[0].C == fod.Col {
			snk.Grow()
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
		fod.Draw('*')

		// 地图边界绘制
		console.MoveCursor(0, 0)
		console.Draw('+')
		console.MoveCursor(height-1, width-1)
		console.Draw('+')
		console.MoveCursor(height-1, 0)
		console.Draw('+')
		console.MoveCursor(0, width-1)
		console.Draw('+')
		for i := 1; i < width-1; i++ {
			console.MoveCursor(0, i)
			console.Draw('-')
			console.MoveCursor(height-1, i)
			console.Draw('-')
		}
		for i := 1; i < height-1; i++ {
			console.MoveCursor(i, 0)
			console.Draw('|')
			console.MoveCursor(i, width-1)
			console.Draw('|')
		}

		console.Flush()
	}
}
