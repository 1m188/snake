package snake

import (
	"snake/console"
)

type Direction int

// 方向
const (
	Up Direction = iota
	Down
	Left
	Right
)

// 移动
var moveMap map[Direction]SnakePos = map[Direction]SnakePos{
	Up:    {R: -1, C: 0},
	Down:  {R: 1, C: 0},
	Left:  {R: 0, C: -1},
	Right: {R: 0, C: 1},
}

// 坐标
type SnakePos struct {
	R, C int // 行 列
}

func (pos *SnakePos) Add(pos2 *SnakePos) {
	pos.R += pos2.R
	pos.C += pos2.C
}
func (pos *SnakePos) Sub(pos2 *SnakePos) {
	pos.R -= pos2.R
	pos.C -= pos2.C
}

// 蛇
type Snake struct {
	Dir Direction  // 方向
	Pos []SnakePos // 蛇身坐标
}

func New(dir Direction, R, C, length int) *Snake {
	snake := &Snake{Dir: dir}
	snake.Pos = make([]SnakePos, length)
	switch dir {
	case Up:
		for i := range length {
			snake.Pos[i] = SnakePos{R + i, C}
		}
	case Down:
		for i := range length {
			snake.Pos[i] = SnakePos{R - i, C}
		}
	case Left:
		for i := range length {
			snake.Pos[i] = SnakePos{R, C + i}
		}
	default:
		for i := range length {
			snake.Pos[i] = SnakePos{R, C - i}
		}
	}
	return snake
}

func (snake *Snake) Move() {
	for i := len(snake.Pos) - 1; i > 0; i-- {
		snake.Pos[i] = snake.Pos[i-1]
	}
	v := moveMap[snake.Dir]
	snake.Pos[0].Add(&v)
}

func (snake *Snake) Draw(b byte) {
	for _, pos := range snake.Pos {
		console.MoveCursor(pos.R, pos.C)
		console.Draw(b)
	}
}

func (snake *Snake) Grow() {
	if len(snake.Pos) == 1 {
		pos := SnakePos{snake.Pos[0].R, snake.Pos[0].C}
		v := moveMap[snake.Dir]
		pos.Sub(&v)
		snake.Pos = append(snake.Pos, pos)
	} else {
		last := snake.Pos[len(snake.Pos)-1]
		pre := snake.Pos[len(snake.Pos)-2]
		v := SnakePos{last.R*2 - pre.R, last.C*2 - pre.C}
		snake.Pos = append(snake.Pos, v)
	}
}

// 蛇是否撞上自己
func (s *Snake) IsKnockSelf() bool {
	for i := 1; i < len(s.Pos); i++ {
		if s.Pos[0] == s.Pos[i] {
			return true
		}
	}
	return false
}
