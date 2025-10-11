package food

import (
	"snake/console"
)

type Food struct {
	Row, Col int // 食物的行列坐标
	Score    int // 食物的得分
}

func New(row, col, score int) *Food {
	return &Food{Row: row, Col: col, Score: score}
}

func (food *Food) Draw(b byte) {
	console.MoveCursor(food.Row, food.Col)
	console.Draw(b)
}
