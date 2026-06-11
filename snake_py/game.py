"""
游戏主控模块。

本模块负责管理游戏主循环、状态机和各模块间的协调。
游戏以固定 60FPS 帧率运行；蛇的移动与帧渲染解耦，
通过帧计数控制移动间隔，实现正常速度与三倍速的切换。
"""

import sys
import time
from enum import Enum, auto

import console
from food import Food
from input import disable_raw_mode, enable_raw_mode, get_key
from render import render_game_over, render_playing
from snake import Direction, Snake


class GameState(Enum):
    """游戏状态枚举。"""

    PLAYING = auto()
    GAME_OVER = auto()


class Game:
    """
    贪吃蛇游戏主类。

    管理游戏的主循环、状态切换、输入处理和模块协调。

    Class Attributes:
        TARGET_FPS (int): 目标帧率（60）。
        FRAME_TIME (float): 每帧目标时长（秒）。
        NORMAL_SPEED_INTERVAL (int): 正常速度下蛇每 N 帧移动一次。
        FAST_SPEED_INTERVAL (int): 三倍速下蛇每 N 帧移动一次。
        MIN_COLS (int): 最低终端列数要求。
        MIN_ROWS (int): 最低终端行数要求。
    """

    TARGET_FPS = 60
    FRAME_TIME = 1.0 / TARGET_FPS
    NORMAL_SPEED_INTERVAL = 6
    FAST_SPEED_INTERVAL = 2
    MIN_COLS = 20
    MIN_ROWS = 10

    def __init__(self):
        """初始化游戏对象，创建各子模块实例。"""
        self._state = None
        self._score = 0
        self._snake = None
        self._food = None
        self._move_interval = self.NORMAL_SPEED_INTERVAL
        self._frame_count = 0
        self._is_fast = False
        self._game_width = 0
        self._game_height = 0

    def run(self):
        """
        启动游戏。

        启用原始终端模式后进入主循环；退出时恢复终端设置并清屏。
        """
        enable_raw_mode()
        try:
            self._start_new_round()
            self._main_loop()
        finally:
            disable_raw_mode()
            console.home()
            console.clear()
            console.show_cursor()
            console.flush()

    def _start_new_round(self):
        """初始化新一局游戏：创建蛇、生成食物、重置分数与速度。"""
        cols, rows = console.term_size()

        if cols < self.MIN_COLS or rows < self.MIN_ROWS:
            print(
                f"终端太小（需要至少 {self.MIN_COLS}x{self.MIN_ROWS}），"
                f"当前 {cols}x{rows}",
                file=sys.stderr,
            )
            sys.exit(1)

        self._game_width = cols - 2
        self._game_height = rows - 2

        start_x = self._game_width // 2
        start_y = self._game_height // 2
        self._snake = Snake(start_x, start_y, Direction.RIGHT, length=3)

        self._food = Food()
        self._food.spawn(self._game_width, self._game_height, set(self._snake.body))

        self._score = 0
        self._is_fast = False
        self._move_interval = self.NORMAL_SPEED_INTERVAL
        self._frame_count = 0
        self._state = GameState.PLAYING

    def _main_loop(self):
        """以 60FPS 固定帧率运行的游戏主循环。"""
        while True:
            frame_start = time.perf_counter()

            self._process_input()

            if self._state == GameState.PLAYING:
                self._update()

            self._render()

            elapsed = time.perf_counter() - frame_start
            sleep_time = self.FRAME_TIME - elapsed
            if sleep_time > 0:
                time.sleep(sleep_time)

    def _process_input(self):
        """
        消费 stdin 缓冲区中的所有待处理按键。

        每个按键根据当前游戏状态分发到对应处理方法。
        """
        while True:
            key = get_key()
            if key is None:
                break
            if self._state == GameState.PLAYING:
                self._handle_playing_input(key)
            elif self._state == GameState.GAME_OVER:
                self._handle_game_over_input(key)

    def _handle_playing_input(self, key):
        """
        游戏进行中的按键处理。

        - w / a / s / d：改变蛇的方向（禁止 180° 掉头）。
        - 空格：切换加速 / 正常速度。
        - 其他按键：忽略。

        Args:
            key (str): 按键字符。
        """
        if key == "w":
            self._snake.set_direction(Direction.UP)
        elif key == "s":
            self._snake.set_direction(Direction.DOWN)
        elif key == "a":
            self._snake.set_direction(Direction.LEFT)
        elif key == "d":
            self._snake.set_direction(Direction.RIGHT)
        elif key == " ":
            self._is_fast = not self._is_fast
            self._move_interval = (
                self.FAST_SPEED_INTERVAL
                if self._is_fast
                else self.NORMAL_SPEED_INTERVAL
            )

    def _handle_game_over_input(self, key):
        """
        游戏结束状态下的按键处理。

        - ESC（\\x1b）或 Ctrl+C：退出程序。
        - 其他任意键：开始新一局。

        Args:
            key (str): 按键字符。
        """
        if key in ("\x1b", "\x03"):
            sys.exit(0)
        self._start_new_round()

    def _update(self):
        """
        每帧更新游戏逻辑。

        刷新终端尺寸以响应用户缩放窗口；通过帧计数控制蛇的移动频率；
        移动后检测墙体碰撞、自身碰撞和食物吃取。
        """
        cols, rows = console.term_size()
        self._game_width = cols - 2
        self._game_height = rows - 2

        self._frame_count += 1
        if self._frame_count % self._move_interval != 0:
            return

        self._snake.move()

        head_x, head_y = self._snake.head
        if (
            head_x < 0
            or head_x >= self._game_width
            or head_y < 0
            or head_y >= self._game_height
        ):
            self._state = GameState.GAME_OVER
            return

        if self._snake.check_self_collision():
            self._state = GameState.GAME_OVER
            return

        if self._snake.head == self._food.position:
            self._snake.grow()
            self._score += 1
            self._food.spawn(self._game_width, self._game_height, set(self._snake.body))

    def _render(self):
        """根据当前游戏状态渲染对应画面。"""
        if self._state == GameState.PLAYING:
            render_playing(self._snake, self._food, self._score)
        elif self._state == GameState.GAME_OVER:
            render_game_over(self._score)
