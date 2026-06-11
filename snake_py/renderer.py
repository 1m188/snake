"""
双缓冲渲染模块。

本模块负责将游戏状态渲染到终端。采用双缓冲策略：
所有内容先在内存中拼接为完整输出字符串，然后一次性写入 stdout，
配合 ANSI 转义序列实现光标复位，避免画面闪烁与撕裂。

对于 CJK（中文）文本，使用 ANSI 光标定位直接写入目标位置，
而非放入由单字节字符组成的画布中，以此消除因 CJK 字符
占 2 列终端宽度而导致的换行级联错位。

地图边界使用 '-'（横）、'|'（竖）、'+'（角）绘制。
"""

import shutil
import sys
import unicodedata


class Renderer:
    """
    终端渲染器。

    使用双缓冲策略：内存中构建完整的输出字符串，然后一次性
    write 到 stdout。CJK 文本通过 ANSI 光标定位直接输出，
    避免与纯 ASCII 画布混合导致的宽度计算错误。

    Class Attributes:
        _CURSOR_HOME (str): ANSI 光标复位序列。
    """

    _CURSOR_HOME = "\033[H"

    def __init__(self):
        """初始化渲染器。"""

    @staticmethod
    def _term_size():
        """
        获取当前终端尺寸。

        Returns:
            tuple: (列数, 行数)。
        """
        size = shutil.get_terminal_size()
        return size.columns, size.lines

    @staticmethod
    def _visual_width(s):
        """
        计算字符串在终端中的视觉宽度。

        CJK 字符（East Asian Width 类别为 W 或 F）占 2 列，
        其余字符占 1 列。

        Args:
            s (str): 输入字符串。

        Returns:
            int: 终端列数宽度。
        """
        return sum(
            2 if unicodedata.east_asian_width(ch) in ("W", "F") else 1 for ch in s
        )

    # ─── 公共渲染入口 ───────────────────────────────────────────

    def render_playing(self, snake, food, score):
        """
        渲染游戏进行中画面。

        边界、蛇和食物通过纯 ASCII 画布绘制；分数（含 CJK）
        通过 ANSI 光标定位单独输出，避免混入画布引发换行。

        Args:
            snake (Snake): 蛇实体。
            food (Food): 食物实体。
            score (int): 当前得分。
        """
        cols, rows = self._term_size()
        game_w = cols - 2
        game_h = rows - 2

        # ── 构建纯 ASCII 帧（边界 + 游戏对象） ──
        lines = self._build_ascii_frame(cols, rows, game_w, game_h, snake, food)

        # ── 拼接输出：边界帧 → 光标定位写分数 → 光标复位 ──
        output = self._CURSOR_HOME
        output += self._join_lines(lines)
        output += self._format_score_at(score, cols)
        output += self._CURSOR_HOME

        sys.stdout.write(output)
        sys.stdout.flush()

    def render_game_over(self, score):
        """
        渲染游戏结束画面。

        边界用纯 ASCII 绘制；居中文本（含 CJK）通过光标定位写入，
        避免宽度计算错误。

        Args:
            score (int): 最终得分。
        """
        cols, rows = self._term_size()

        # ── 边界画布 ──
        border_lines = self._build_border_only(cols, rows)

        # ── 拼接输出 ──
        output = self._CURSOR_HOME
        output += self._join_lines(border_lines)
        output += self._format_game_over_text(score, cols, rows)
        output += self._CURSOR_HOME

        sys.stdout.write(output)
        sys.stdout.flush()

    # ─── 帧构建 ─────────────────────────────────────────────────

    def _build_ascii_frame(self, cols, rows, game_w, game_h, snake, food):
        """
        构建纯 ASCII 游戏帧（边界 + 蛇 + 食物）。

        Returns:
            list[str]: 每行一个字符串，不含 CJK 文本。
        """
        lines = []
        lines.append("+" + "-" * (cols - 2) + "+")
        for gy in range(game_h):
            row = ["|"]
            for gx in range(game_w):
                pos = (gx, gy)
                if pos == snake.head:
                    row.append(snake.HEAD_SYMBOL)
                elif snake.occupies(pos):
                    row.append(snake.BODY_SYMBOL)
                elif pos == food.position:
                    row.append(food.SYMBOL)
                else:
                    row.append(" ")
            row.append("|")
            lines.append("".join(row))
        lines.append("+" + "-" * (cols - 2) + "+")
        return lines

    def _build_border_only(self, cols, rows):
        """构建仅包含边界的画布线。"""
        lines = ["+" + "-" * (cols - 2) + "+"]
        for _ in range(rows - 2):
            lines.append("|" + " " * (cols - 2) + "|")
        lines.append("+" + "-" * (cols - 2) + "+")
        return lines

    @staticmethod
    def _join_lines(lines):
        """
        将行列表拼接为输出片段。

        除最后一行外，每行后追加 \\r\\n 以将光标移至下一行的
        列 0，避免列偏移累积。
        """
        result = ""
        for i, line in enumerate(lines):
            result += line
            if i < len(lines) - 1:
                result += "\r\n"
        return result

    # ─── CJK 文本光标定位 ──────────────────────────────────────

    def _format_score_at(self, score, cols):
        """
        生成在右上角绘制得分的光标定位字符串。

        分数放置在第 2 行（第一行内部）右侧，紧邻右边界的 '|'。

        Args:
            score (int): 当前得分。
            cols (int): 终端列数。

        Returns:
            str: 包含 ANSI 光标定位和得分文本的字符串。
        """
        text = f"分数: {score}"
        sw = self._visual_width(text)
        # ANSI 坐标为 1-indexed，score 右对齐至右边界内侧
        col = max(2, cols - sw)
        return f"\033[2;{col}H{text}"

    def _format_game_over_text(self, score, cols, rows):
        """
        生成游戏结束居中文本的光标定位字符串。

        依次输出四条居中文本，在垂直方向也居中排列。

        Args:
            score (int): 最终得分。
            cols (int): 终端列数。
            rows (int): 终端行数。

        Returns:
            str: ANSI 光标定位片段。
        """
        text_lines = [
            "游戏结束！",
            f"最终得分: {score}",
            "",  # 空行用于垂直间距
            "按下任意键继续",
            "按下Esc键退出游戏",
        ]
        start_row = rows // 2 - len(text_lines) // 2 + 1  # 1-indexed

        parts = []
        for i, text in enumerate(text_lines):
            if not text:
                continue
            sw = self._visual_width(text)
            start_col = max(2, (cols - sw) // 2 + 1)
            row = start_row + i
            parts.append(f"\033[{row};{start_col}H{text}")
        return "".join(parts)
