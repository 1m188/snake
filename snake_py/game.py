"""
游戏主控模块。

本模块负责管理游戏主循环、状态机和各模块间的协调。
游戏以固定 60FPS 帧率运行；蛇的移动与帧渲染解耦：
方向按键缓冲到 _pending_direction，仅在移动帧应用（取最后一个），
通过帧计数控制移动间隔，实现正常速度与三倍速的切换。
"""

from __future__ import annotations

import sys
import time
from enum import Enum, auto
from typing import TYPE_CHECKING

import console
from food import Food
from input import disable_raw_mode, enable_raw_mode, get_key
from render import render_game_over, render_playing
from snake import Direction, Snake

if TYPE_CHECKING:
    pass


class GameState(Enum):
    """游戏状态枚举。"""

    PLAYING = auto()
    GAME_OVER = auto()


class Game:
    """
    贪吃蛇游戏主类。

    管理游戏的主循环、状态切换、输入处理和模块协调。

    Attributes:
        _state: 当前游戏状态（PLAYING 或 GAME_OVER）。
        _score: 当前局得分。
        _snake: 蛇实体。
        _food: 食物实体。
        _move_interval: 当前移动间隔（帧数）。
        _frame_count: 帧计数器。
        _is_fast: 是否处于三倍速状态。
        _game_width: 游戏区域宽度。
        _game_height: 游戏区域高度。
        _pending_direction: 缓冲的方向按键，移动帧时应用并清空。
    """

    TARGET_FPS: int = 60
    FRAME_TIME: float = 1.0 / TARGET_FPS
    NORMAL_SPEED_INTERVAL: int = 6
    FAST_SPEED_INTERVAL: int = 2
    MIN_COLS: int = 20
    MIN_ROWS: int = 10

    def __init__(self) -> None:
        """初始化游戏对象，创建各子模块实例。"""
        self._state: GameState | None = None
        self._score: int = 0
        self._snake: Snake | None = None
        self._food: Food | None = None
        self._move_interval: int = self.NORMAL_SPEED_INTERVAL
        self._frame_count: int = 0
        self._is_fast: bool = False
        self._game_width: int = 0
        self._game_height: int = 0
        self._pending_direction: Direction | None = None

    def run(self) -> None:
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

    def _start_new_round(self) -> None:
        """初始化新一局游戏：创建蛇、生成食物、重置分数、速度与方向缓冲。"""
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
        self._pending_direction = None

    def _main_loop(self) -> None:
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

    def _process_input(self) -> None:
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

    def _handle_playing_input(self, key: str) -> None:
        """
        游戏进行中的按键处理。

        - w / a / s / d：将目标方向存入 _pending_direction。
          同帧内多次按键只保留最后一个；实际方向在 _update
          移动帧时由 set_direction 应用（含 180° 防反）。
        - 空格：切换加速 / 正常速度。
        - 其他按键：忽略。

        Args:
            key: 按键字符。
        """
        if key == "w":
            self._pending_direction = Direction.UP
        elif key == "s":
            self._pending_direction = Direction.DOWN
        elif key == "a":
            self._pending_direction = Direction.LEFT
        elif key == "d":
            self._pending_direction = Direction.RIGHT
        elif key == " ":
            self._is_fast = not self._is_fast
            self._move_interval = (
                self.FAST_SPEED_INTERVAL
                if self._is_fast
                else self.NORMAL_SPEED_INTERVAL
            )

    def _handle_game_over_input(self, key: str) -> None:
        """
        游戏结束状态下的按键处理。

        - ESC（\\x1b）或 Ctrl+C：退出程序。
        - 其他任意键：开始新一局。

        Args:
            key: 按键字符。
        """
        if key in ("\x1b", "\x03"):
            sys.exit(0)
        self._start_new_round()

    def _update(self) -> None:
        """
        每帧更新游戏逻辑。

        刷新终端尺寸；通过帧计数控制蛇移动频率；
        移动帧时先应用缓冲的方向（取最后按的），再移动，
        随后检测墙体碰撞、自身碰撞和食物吃取。
        """
        cols, rows = console.term_size()
        self._game_width = cols - 2
        self._game_height = rows - 2

        self._frame_count += 1
        if self._frame_count % self._move_interval != 0:
            return

        if self._pending_direction is not None:
            self._snake.set_direction(self._pending_direction)
            self._pending_direction = None

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

    def _render(self) -> None:
        """根据当前游戏状态渲染对应画面。"""
        if self._state == GameState.PLAYING:
            render_playing(self._snake, self._food, self._score)
        elif self._state == GameState.GAME_OVER:
            render_game_over(self._score)
