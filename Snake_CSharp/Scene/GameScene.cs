using System.Threading;
using System;
using Snake.Sprite;

namespace Snake.Scene
{
    /// <summary>
    /// 游戏主场景
    /// </summary>
    class GameScene : Scene
    {
        //字段
        /// <summary>
        /// 蛇
        /// </summary>
        private Snake.Sprite.Snake snake;
        /// <summary>
        /// 食物
        /// </summary>
        private Food food;
        /// <summary>
        /// 每一次画面刷新的间隔时间
        /// </summary>
        private int timeout;
        /// <summary>
        /// 是否正处于加速状态
        /// </summary>
        private bool isAcc;
        //地图的左、右、上、下的边界位置
        /// <summary>
        /// 地图的左边界位置
        /// </summary>
        private int left;
        /// <summary>
        /// 地图的右边界位置
        /// </summary>
        private int right;
        /// <summary>
        /// 地图的上边界位置
        /// </summary>
        private int up;
        /// <summary>
        /// 地图的下边界位置
        /// </summary>
        private int down;
        /// <summary>
        /// 当前分数
        /// </summary>
        private int nowScore;

        /// <summary>
        /// 初始化游戏场景
        /// </summary>
        /// <param name="difficulty">每一次画面刷新的间隔时间</param>
        public void init(int difficulty)
        {
            //初始游戏内容元素
            snake = new Snake.Sprite.Snake(new Tuple<int, int>(10, 10), 2, 3);
            food = new Food(BodyGrowNum: difficulty); //1 2 3 4依次增加分数和节数
            timeout = -100 * difficulty + 400; //1 2 3 4 依次的timeout依次递减，到最后光速为0停顿
            isAcc = false;
            left = 1;
            right = Console.WindowWidth - 2;
            up = 2;
            down = Console.WindowHeight - 3;
            nowScore = 0;

            //初始化场景显示
            //地图上边界
            Console.SetCursorPosition(left, up);
            for (int i = 0; i < right; i++)
            {
                Console.Write('_');
            }
            //地图下边界
            Console.SetCursorPosition(left, down);
            for (int i = 0; i < right; i++)
            {
                Console.Write('-');
            }
            //地图左边界
            for (int i = up + 1; i < down; i++)
            {
                Console.SetCursorPosition(left, i);
                Console.Write('|');
            }
            //地图右边界
            for (int i = up + 1; i < down; i++)
            {
                Console.SetCursorPosition(right, i);
                Console.Write('|');
            }
            showScore();
        }

        /// <summary>
        /// 分数显示
        /// </summary>
        private void showScore()
        {
            Console.SetCursorPosition(left + 2, up - 1);
            Console.Write("分数：{0}", nowScore);
        }

        /// <summary>
        /// 运行场景
        /// </summary>
        /// <returns>返回一个值用作运行结果的表示或者某些需要返回值的情况</returns>
        public override int run()
        {
            food.getNewPos(left, right, up, down); //初始化随机食物坐标在地图边界之内
            food.display(true); //食物显示
            while (true)
            {
                snake.display(true); //蛇显示
                Thread.Sleep(timeout); //暂停一会儿，用作视觉停留
                snake.display(false); //蛇身消失
                //获取按键
                if (Console.KeyAvailable)
                {
                    ConsoleKey key = Console.ReadKey(true).Key;
                    //方向改变按键
                    if (key == ConsoleKey.W || key == ConsoleKey.S || key == ConsoleKey.A || key == ConsoleKey.D)
                    {
                        snake.changeDir(key); //方向改变
                    }
                    //空格键速度改变
                    else if (key == ConsoleKey.Spacebar)
                    {
                        if (isAcc)
                        {
                            timeout *= 3;
                        }
                        else
                        {
                            timeout /= 3;
                        }
                        isAcc = !isAcc;
                    }
                }
                snake.move(); //移动
                //判定是否吃到食物
                if (snake.isGettedFood(food.X, food.Y))
                {
                    //蛇的更新
                    snake.grow(food.BodyGrowNum); //每次吃到食物增长若干节
                    //食物更新
                    food.getNewPos(left, right, up, down);
                    food.display(true);
                    //分数更新
                    nowScore += food.BodyGrowNum; //每次吃到食物的时候分数加上蛇身增长的节数
                    showScore();
                }
                //判定蛇是否死亡
                if (snake.isDead(left, right, up, down))
                {
                    break;
                }
            }
            return nowScore;
        }
    }
}
