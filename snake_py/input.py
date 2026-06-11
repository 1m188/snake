"""
跨平台键盘输入模块。

本模块提供非阻塞的键盘读取功能，兼容 Windows、Linux 和 macOS。
在 Unix 系统上使用 termios 和 select 实现原始模式下的按键读取；
在 Windows 系统上使用 msvcrt 实现。
"""

import sys
import select
import atexit

import console

_is_raw_enabled = False

if sys.platform == "win32":
    import msvcrt

    def enable_raw_mode():
        """启用原始终端模式。Windows 下无需特殊设置，仅隐藏光标。"""
        global _is_raw_enabled
        if not _is_raw_enabled:
            console.hide_cursor()
            console.flush()
            _is_raw_enabled = True

    def disable_raw_mode():
        """恢复终端设置并显示光标。"""
        global _is_raw_enabled
        if _is_raw_enabled:
            console.show_cursor()
            console.flush()
            _is_raw_enabled = False

    def get_key():
        """
        非阻塞读取单个按键。

        Returns:
            str | None: 按下的字符，无按键时返回 None。
        """
        if msvcrt.kbhit():
            ch = msvcrt.getch()
            if ch == b"\xe0":
                ch2 = msvcrt.getch()
                return "\x1b[" + ch2.decode("ascii", errors="replace")
            return ch.decode("utf-8", errors="replace")
        return None

else:
    import tty
    import termios

    _original_settings = None

    def enable_raw_mode():
        """
        启用原始终端模式，隐藏光标。

        禁用行缓冲和回显，使按键可即时读取。
        通过 atexit 注册恢复函数以防异常退出时终端状态异常。
        """
        global _is_raw_enabled, _original_settings
        if not _is_raw_enabled:
            fd = sys.stdin.fileno()
            _original_settings = termios.tcgetattr(fd)
            tty.setraw(fd)
            console.hide_cursor()
            console.flush()
            atexit.register(_restore_terminal)
            _is_raw_enabled = True

    def disable_raw_mode():
        """恢复终端到原始设置并显示光标。"""
        global _is_raw_enabled, _original_settings
        if _is_raw_enabled:
            console.show_cursor()
            console.flush()
            _restore_terminal()
            _is_raw_enabled = False

    def _restore_terminal():
        """安全恢复终端属性，可重复调用。"""
        global _original_settings
        if _original_settings is not None:
            fd = sys.stdin.fileno()
            try:
                termios.tcsetattr(fd, termios.TCSADRAIN, _original_settings)
            except Exception:
                pass
            _original_settings = None

    def get_key():
        """
        非阻塞读取单个按键。

        使用 select 检测 stdin 可读性。对 ESC 键（\\x1b）做特殊处理：
        若后续 1ms 内到达更多字节则视为转义序列（如箭头键），
        否则视为单独的 ESC 按键。

        Returns:
            str | None: 按下的字符或转义序列，无按键时返回 None。
        """
        if select.select([sys.stdin], [], [], 0)[0]:
            ch = sys.stdin.read(1)
            if ch == "\x1b":
                if select.select([sys.stdin], [], [], 0.001)[0]:
                    seq = "\x1b"
                    while True:
                        r, _, _ = select.select([sys.stdin], [], [], 0)
                        if not r:
                            break
                        seq += sys.stdin.read(1)
                    return seq
                return "\x1b"
            return ch
        return None
