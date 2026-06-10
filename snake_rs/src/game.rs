/// 游戏状态与逻辑模块
///
/// 定义蛇的移动方向、游戏状态结构体，以及蛇的移动、
/// 碰撞检测、食物生成等核心游戏逻辑。
use rand::Rng;

/// 蛇的移动方向
#[derive(Clone, Copy, PartialEq)]
pub enum Direction {
    Up,
    Down,
    Left,
    Right,
}

impl Direction {
    /// 返回当前方向的反方向，用于阻止蛇 180° 掉头
    pub fn opposite(self) -> Direction {
        match self {
            Direction::Up => Direction::Down,
            Direction::Down => Direction::Up,
            Direction::Left => Direction::Right,
            Direction::Right => Direction::Left,
        }
    }
}

/// 游戏核心状态
pub struct Game {
    /// 蛇身坐标列表，索引 0 始终为蛇头
    pub snake: Vec<(u16, u16)>,
    /// 当前食物的坐标
    pub food: (u16, u16),
    /// 蛇当前移动方向
    pub direction: Direction,
    /// 当前得分（每吃到一个食物加 1 分）
    pub score: u32,
    /// 游戏是否已结束
    pub game_over: bool,
    /// 棋盘内部宽度（不包含左右边框）
    pub width: u16,
    /// 棋盘内部高度（不包含上下边框）
    pub height: u16,
}

impl Game {
    /// 创建新游戏
    ///
    /// 蛇初始 3 节，居中排列，默认向右移动，食物随机放置在空白处。
    pub fn new(width: u16, height: u16) -> Self {
        let center_x = width / 2;
        let center_y = height / 2;

        // 蛇初始 3 节，水平排列于场地中央
        let snake = vec![
            (center_x, center_y),     // 蛇头（索引 0）
            (center_x - 1, center_y), // 身体
            (center_x - 2, center_y), // 尾部
        ];

        // 在空白处随机生成食物
        let food = Self::generate_food(&snake, width, height);

        Game {
            snake,
            food,
            direction: Direction::Right,
            score: 0,
            game_over: false,
            width,
            height,
        }
    }

    /// 在棋盘上随机随机一个未被蛇身占用的坐标作为食物位置
    fn generate_food(snake: &[(u16, u16)], width: u16, height: u16) -> (u16, u16) {
        let mut rng = rand::thread_rng();
        loop {
            let x = rng.gen_range(0..width);
            let y = rng.gen_range(0..height);
            if !snake.contains(&(x, y)) {
                return (x, y);
            }
        }
    }

    /// 尝试改变蛇的移动方向
    ///
    /// 若新方向与当前方向完全相反（180° 掉头），则忽略本次操作。
    pub fn change_direction(&mut self, new_dir: Direction) {
        if new_dir != self.direction.opposite() {
            self.direction = new_dir;
        }
    }

    /// 执行一帧游戏逻辑
    ///
    /// 按当前方向移动蛇头，检测是否撞墙或撞自身。
    /// 若吃到食物则增长一节并加分；否则移除尾部保持长度不变。
    /// 返回 `true` 表示游戏仍在进行，`false` 表示本帧游戏结束。
    pub fn update(&mut self) -> bool {
        if self.game_over {
            return false;
        }

        let (head_x, head_y) = self.snake[0];

        // 根据方向计算新蛇头位置，同时检测撞墙
        let new_head = match self.direction {
            Direction::Up => {
                if head_y == 0 {
                    self.game_over = true;
                    return false;
                }
                (head_x, head_y - 1)
            }
            Direction::Down => {
                if head_y + 1 >= self.height {
                    self.game_over = true;
                    return false;
                }
                (head_x, head_y + 1)
            }
            Direction::Left => {
                if head_x == 0 {
                    self.game_over = true;
                    return false;
                }
                (head_x - 1, head_y)
            }
            Direction::Right => {
                if head_x + 1 >= self.width {
                    self.game_over = true;
                    return false;
                }
                (head_x + 1, head_y)
            }
        };

        // 判断是否吃到食物（新蛇头位置与食物重合）
        let eating = new_head == self.food;

        // 检测是否撞到自身
        // 若吃到食物则尾部不动，需要检查整个身体（含尾部）
        // 若没吃到食物则尾部会移除，只检查除最后一节外的身体
        let collision_range = if eating {
            &self.snake[..]
        } else {
            &self.snake[..self.snake.len() - 1]
        };
        if collision_range.contains(&new_head) {
            self.game_over = true;
            return false;
        }

        // 在蛇身前端插入新蛇头
        self.snake.insert(0, new_head);

        if eating {
            // 吃到食物：尾部不动（蛇增长一节），加分并生成新食物
            self.score += 1;
            self.food = Self::generate_food(&self.snake, self.width, self.height);
        } else {
            // 未吃到食物：移除尾部，保持蛇身长度不变
            self.snake.pop();
        }

        true
    }
}
