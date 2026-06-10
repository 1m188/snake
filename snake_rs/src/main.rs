/// 程序入口与游戏主循环
///
/// 负责终端环境的初始化与恢复、键盘输入的监听、
/// 游戏主循环的调度（输入 → 更新 → 渲染 → 计时）。
mod game;
mod render;

use crossterm::{
    ExecutableCommand, cursor,
    event::{self, Event, KeyCode},
    terminal,
};
use std::io::{self, stdout};
use std::time::{Duration, Instant};

use game::{Direction, Game};

/// 每帧的时间间隔（毫秒），控制蛇的移动速度
const TICK_MS: u64 = 150;

fn main() -> io::Result<()> {
    // ── 终端初始化 ──
    let mut stdout = stdout();

    // 启用原始模式（禁用行缓冲和回显，逐键读取）
    terminal::enable_raw_mode()?;
    // 切换到备用屏幕，避免游戏界面污染终端滚动历史
    stdout.execute(terminal::EnterAlternateScreen)?;
    // 隐藏光标
    stdout.execute(cursor::Hide)?;

    // 获取终端尺寸，棋盘需在终端内部各留 1 格给边框
    let (term_w, term_h) = terminal::size()?;
    let game_w = term_w.saturating_sub(2).max(10);
    let game_h = term_h.saturating_sub(2).max(10);

    // 创建初始游戏状态
    let mut game = Game::new(game_w, game_h);
    let tick_duration = Duration::from_millis(TICK_MS);
    let mut last_tick = Instant::now();
    let mut running = true;

    // ── 游戏主循环 ──
    while running {
        if game.game_over {
            // ===== 游戏结束状态 =====
            render::render_game_over(&mut stdout, &game)?;

            // 阻塞等待任意按键以重新开始
            loop {
                if event::poll(Duration::from_millis(100))?
                    && let Event::Key(_) = event::read()?
                {
                    // 重新获取终端尺寸（窗口大小可能已变化）
                    let (tw, th) = terminal::size()?;
                    let gw = tw.saturating_sub(2).max(10);
                    let gh = th.saturating_sub(2).max(10);
                    game = Game::new(gw, gh);
                    last_tick = Instant::now();
                    break;
                }
            }
        } else {
            // ===== 游戏进行状态 =====
            render::render(&mut stdout, &game)?;

            // 计算距下一次 tick 还需等待的时间
            let elapsed = last_tick.elapsed();
            let remaining = tick_duration.saturating_sub(elapsed);

            // 等待输入事件，超时后自动推进游戏帧
            if event::poll(remaining)?
                && let Event::Key(key_event) = event::read()?
            {
                match key_event.code {
                    // WASD 方向控制
                    KeyCode::Char('w') | KeyCode::Char('W') => {
                        game.change_direction(Direction::Up);
                    }
                    KeyCode::Char('s') | KeyCode::Char('S') => {
                        game.change_direction(Direction::Down);
                    }
                    KeyCode::Char('a') | KeyCode::Char('A') => {
                        game.change_direction(Direction::Left);
                    }
                    KeyCode::Char('d') | KeyCode::Char('D') => {
                        game.change_direction(Direction::Right);
                    }
                    // Esc / Q 退出游戏
                    KeyCode::Esc | KeyCode::Char('q') | KeyCode::Char('Q') => {
                        running = false;
                    }
                    _ => {} // 忽略其他按键
                }
            }

            // 若距上次 tick 已过去设定时长，推进游戏一帧
            if last_tick.elapsed() >= tick_duration {
                game.update();
                last_tick = Instant::now();
            }
        }
    }

    // ── 终端恢复 ──
    stdout.execute(cursor::Show)?;
    stdout.execute(terminal::LeaveAlternateScreen)?;
    terminal::disable_raw_mode()?;

    Ok(())
}
