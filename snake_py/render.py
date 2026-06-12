"""
双缓冲渲染模块。

本模块负责将游戏状态渲染到终端，**完全自管渲染符号**：
蛇头、蛇身、食物的符号均定义为模块级常量，通过参数传入渲染函数，
不与游戏对象耦合。

采用双缓冲策略：每帧先在内存中通过 console 模块往输出缓冲区追加内容，
最后一次性 flush，避免画面闪烁与撕裂。

CJK（中文）文本通过光标定位直接写入目标位置，不与
纯 ASCII 画布混合，以此消除宽度计算错误导致的换行错位。

所有渲染函数均以 render_ 为前缀，按调用顺序编排：
  清屏 → 画地图 → 画蛇 → 画食物 → 画 UI 文字 → 刷新
"""

from __future__ import annotations

import unicodedata
from typing import TYPE_CHECKING

import console

if TYPE_CHECKING:
    from food import Food
    from snake import Snake

# ─── 渲染符号常量 ───────────────────────────────────────────────

char_head = "@"
char_body = "#"
char_food = "*"

# ─── 工具函数 ───────────────────────────────────────────────────


def _visual_width(s: str) -> int:
    """
    计算字符串在终端中的视觉宽度。

    CJK 字符（East Asian Width 为 W 或 F）占 2 列，其余占 1 列。

    Args:
        s: 输入字符串。

    Returns:
        终端列数。
    """
    return sum(2 if unicodedata.east_asian_width(ch) in ("W", "F") else 1 for ch in s)


# ─── 总编排函数 ─────────────────────────────────────────────────


def render_playing(snake: Snake, food: Food, score: int) -> None:
    """
    渲染游戏进行中的完整一帧。

    Args:
        snake: 蛇实体。
        food: 食物实体。
        score: 当前得分。
    """
    cols, rows = console.term_size()

    console.home()
    console.clear()
    render_map(cols, rows)
    render_snake(snake, char_head, char_body, cols, rows)
    render_food(food, char_food, cols, rows)
    render_score(score, cols)
    console.home()
    console.flush()


def render_game_over(score: int) -> None:
    """
    渲染游戏结束画面。

    Args:
        score: 最终得分。
    """
    cols, rows = console.term_size()

    console.home()
    console.clear()
    render_map(cols, rows)
    render_game_over_text(score, cols, rows)
    console.home()
    console.flush()


# ─── 游戏对象渲染函数 ────────────────────────────────────────────


def render_map(cols: int, rows: int) -> None:
    """
    绘制地图边界。

    上下边界使用水平长字符串一次 write；左右边界逐行定位写 '|'。

    Args:
        cols: 终端列数。
        rows: 终端行数。
    """
    console.move(1, 1)
    console.write("+" + "-" * (cols - 2) + "+")

    console.move(rows, 1)
    console.write("+" + "-" * (cols - 2) + "+")

    for r in range(2, rows):
        console.move(r, 1)
        console.write("|")
        console.move(r, cols)
        console.write("|")


def render_snake(
    snake: Snake, char_head: str, char_body: str, cols: int, rows: int
) -> None:
    """
    逐节绘制蛇身。

    蛇头与蛇身的显示符号由调用方通过参数传入，
    确保渲染内容与游戏对象解耦。超出边界的节不绘制。

    Args:
        snake: 蛇实体。
        char_head: 蛇头显示符号。
        char_body: 蛇身显示符号。
        cols: 终端列数。
        rows: 终端行数。
    """
    for i, (gx, gy) in enumerate(snake.body):
        tx = gx + 2
        ty = gy + 2
        if 2 <= tx < cols and 2 <= ty < rows:
            console.move(ty, tx)
            console.write(char_head if i == 0 else char_body)


def render_food(food: Food, char_food: str, cols: int, rows: int) -> None:
    """
    绘制食物。

    若食物未生成（position 为 None）则跳过。
    显示符号由调用方通过参数传入，确保渲染与对象解耦。

    Args:
        food: 食物实体。
        char_food: 食物显示符号。
        cols: 终端列数。
        rows: 终端行数。
    """
    if food.position is None:
        return
    gx, gy = food.position
    tx = gx + 2
    ty = gy + 2
    if 2 <= tx < cols and 2 <= ty < rows:
        console.move(ty, tx)
        console.write(char_food)


# ─── UI 文本渲染函数 ─────────────────────────────────────────────


def render_score(score: int, cols: int) -> None:
    """
    在右上角绘制得分（含 CJK 文字）。

    分数位于第 2 行，右对齐至右边界内侧。

    Args:
        score: 当前得分。
        cols: 终端列数。
    """
    text = f"分数: {score}"
    sw = _visual_width(text)
    col = max(2, cols - sw)
    console.move(2, col)
    console.write(text)


def render_game_over_text(score: int, cols: int, rows: int) -> None:
    """
    在画面中央绘制游戏结束文本组（含 CJK）。

    垂直与水平方向均居中排列。

    Args:
        score: 最终得分。
        cols: 终端列数。
        rows: 终端行数。
    """
    text_lines = [
        "游戏结束！",
        f"最终得分: {score}",
        "",
        "按下任意键继续",
        "按下Esc键退出游戏",
    ]
    start_row = rows // 2 - len(text_lines) // 2 + 1

    for i, text in enumerate(text_lines):
        if not text:
            continue
        sw = _visual_width(text)
        start_col = max(2, (cols - sw) // 2 + 1)
        console.move(start_row + i, start_col)
        console.write(text)
