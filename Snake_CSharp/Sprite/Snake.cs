using System;
using System.Collections.Generic;
using System.Linq;

namespace Snake.Sprite
{
    /// <summary>
    /// 蛇类
    /// </summary>
    class Snake
    {
        /// <summary>
        /// 蛇身坐标数组
        /// </summary>
        private List<Tuple<int, int>> pos;
        /// <summary>
        /// 蛇头方向枚举类型
        /// </summary>
        private enum Direction
        {
            up,
            down,
            left,
            right,
        }
        /// <summary>
        /// 蛇头当前方向
        /// </summary>
        private Direction dir;
        /// <summary>
        /// 蛇根据方向的坐标变化数组
        /// </summary>
        private Tuple<int, int>[] dirChange = new Tuple<int, int>[4] { new Tuple<int, int>(0, -1), new Tuple<int, int>(0, 1), new Tuple<int, int>(-1, 0), new Tuple<int, int>(1, 0) };

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="headPos">蛇头坐标</param>
        /// <param name="bodyNum">蛇身除蛇头之外的蛇身节数</param>
        /// <param name="initDir">初始的蛇头方向，0123分别对应上下左右四个方向，其他的数字能够被转化成这四个数字</param>
        public Snake(Tuple<int, int> headPos, int bodyNum, int initDir)
        {
            pos = new List<Tuple<int, int>> { headPos }; //初始化蛇头和蛇身列表
            dir = (Direction)((initDir < 0 ? -initDir : initDir) % 4); //初始化蛇头方向

            //初始化蛇身每节的坐标
            for (int i = 0; i < bodyNum; i++)
            {
                Tuple<int, int> last = pos.Last();
                pos.Add(new Tuple<int, int>(last.Item1 - dirChange[(int)dir].Item1, last.Item2 - dirChange[(int)dir].Item2));
            }
        }

        /// <summary>
        /// 蛇移动
        /// </summary>
        public void move()
        {
            //这里是把末尾的一节去掉之后把头按照移动方向加一个单位做成新头从头部压入蛇身列表
            pos.RemoveAt(pos.Count - 1);
            Tuple<int, int> head = pos.First();
            Tuple<int, int> newHead = new Tuple<int, int>(head.Item1 + dirChange[(int)dir].Item1, head.Item2 + dirChange[(int)dir].Item2);
            pos.Insert(0, newHead);
        }

        /// <summary>
        /// 改变蛇的移动方向
        /// </summary>
        /// <param name="key">传入的按下的键盘的键，能够根据键盘的按键看是否应该改变方向</param>
        public void changeDir(ConsoleKey key)
        {
            if (key == ConsoleKey.W && dir != Direction.down)
            {
                dir = Direction.up;
            }
            else if (key == ConsoleKey.S && dir != Direction.up)
            {
                dir = Direction.down;
            }
            else if (key == ConsoleKey.A && dir != Direction.right)
            {
                dir = Direction.left;
            }
            else if (key == ConsoleKey.D && dir != Direction.left)
            {
                dir = Direction.right;
            }
        }

        /// <summary>
        /// 显示蛇身或者隐藏蛇身
        /// </summary>
        /// <param name="isDisplay">是否要显示蛇身</param>
        public void display(bool isDisplay)
        {
            foreach (Tuple<int, int> i in pos)
            {
                try
                {
                    Console.SetCursorPosition(i.Item1, i.Item2);
                }
                catch (ArgumentOutOfRangeException) //有参数超出界限异常的时候就直接跳过本次循环
                {
                    continue;
                }
                if (isDisplay)
                {
                    Console.Write('#'); //否则画蛇身
                }
                else
                {
                    Console.Write(' '); //隐藏蛇身
                }
            }
        }

        /// <summary>
        /// 判定是否死亡
        /// </summary>
        /// <param name="left">蛇移动边界左边</param>
        /// <param name="right">蛇移动边界右边</param>
        /// <param name="up">蛇移动边界上面</param>
        /// <param name="down">蛇移动边界下面</param>
        /// <returns>是否死亡</returns>
        public bool isDead(int left, int right, int up, int down)
        {
            Tuple<int, int> head = pos[0];
            if (head.Item1 <= left || head.Item1 >= right || head.Item2 <= up || head.Item2 >= down) //判断是否撞墙
            {
                return true;
            }
            for (int i = 1; i < pos.Count; i++) //判断是否自我相撞
            {
                if (head.Equals(pos[i]))
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// 蛇吃了食物之后的蛇身增长
        /// </summary>
        /// <param name="growNum">每次吃到食物之后增长的节数</param>
        public void grow(int growNum)
        {
            //在蛇身末尾相对于移动方向相反的方向一格添加一个尾巴
            for (int i = 0; i < growNum; i++)
            {
                Tuple<int, int> last = pos.Last();
                pos.Add(new Tuple<int, int>(last.Item1 - dirChange[(int)dir].Item1, last.Item2 - dirChange[(int)dir].Item2));
            }
        }

        /// <summary>
        /// 蛇是否吃到食物
        /// </summary>
        /// <param name="foodX">食物的x坐标</param>
        /// <param name="foodY">食物的y坐标</param>
        /// <returns>蛇是否吃到食物</returns>
        public bool isGettedFood(int foodX, int foodY)
        {
            //蛇头和食物坐标重合了之后就算是吃到食物了
            return pos.First().Equals(new Tuple<int, int>(foodX, foodY));
        }
    }
}
