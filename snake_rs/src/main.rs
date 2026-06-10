/// 程序入口与游戏主循环
///
/// 负责终端环境的初始化与恢复、键盘输入的监听、
/// 游戏主循环的调度。采用 60 FPS 帧率驱动，每帧独立处理输入、
/// 游戏逻辑和渲染，蛇的移动节拍由独立的计时器控制。
///
/// # 双缓冲渲染
/// 每帧在内存中构建完整的画面字符串，通过一次 `Print` + `flush`
/// 整体渲染到终端，避免画面闪烁或撕裂。
mod game;
mod render;

use crossterm::{
    ExecutableCommand, cursor,
    event::{self, Event, KeyCode, KeyEventKind},
    terminal,
};
use std::io::{self, stdout};
use std::thread;
use std::time::{Duration, Instant};

use game::{Direction, Game};

/// 渲染帧间隔 —— 60 FPS（约 16.67ms，取整 16ms）
const FRAME_MS: u64 = 16;
/// 蛇基础移动间隔（毫秒）
const BASE_TICK_MS: u64 = 150;
/// 加速移动间隔（150 / 3 = 50ms）
const FAST_TICK_MS: u64 = 50;
/// 空格切换冷却时间（毫秒）—— 两次 toggle 之间的最小间隔
///
/// 不依赖终端的 KeyEventKind 区分 Press/Repeat，
/// 长按时频繁的空格事件（~30ms 间隔）全部被冷却过滤，只生效一次。
const SPACE_COOLDOWN_MS: u64 = 200;

fn main() -> io::Result<()> {
    // ── 终端初始化 ──
    let mut stdout = stdout();

    // 启用原始模式，禁用行缓冲和回显
    terminal::enable_raw_mode()?;
    // 切换到备用屏幕
    stdout.execute(terminal::EnterAlternateScreen)?;
    // 隐藏光标
    stdout.execute(cursor::Hide)?;

    // 获取终端尺寸，棋盘留 1 格边框
    let (term_w, term_h) = terminal::size()?;
    let game_w = term_w.saturating_sub(2).max(10);
    let game_h = term_h.saturating_sub(2).max(10);

    // 创建初始游戏状态
    let mut game = Game::new(game_w, game_h);
    // 当前蛇移动节拍（150ms 基础 / 50ms 加速）
    let mut current_tick_ms = BASE_TICK_MS;
    let mut last_game_tick = Instant::now();
    // 上次空格 toggle 的时间（冷却计时器，防止长按时反复切换）
    let mut last_space_toggle = Instant::now();
    let mut running = true;

    // ── 游戏主循环 ──
    while running {
        if game.game_over {
            // ===== 游戏结束状态 =====
            render::render_game_over(&mut stdout, &game)?;

            // 阻塞等待按键：Esc 退出 / 任意键重新开始
            loop {
                if event::poll(Duration::from_millis(100))?
                    && let Event::Key(key) = event::read()?
                {
                    if key.code == KeyCode::Esc {
                        running = false;
                    } else {
                        // 重新开始：获取最新终端尺寸并重建游戏
                        let (tw, th) = terminal::size()?;
                        let gw = tw.saturating_sub(2).max(10);
                        let gh = th.saturating_sub(2).max(10);
                        game = Game::new(gw, gh);
                        last_game_tick = Instant::now();
                        current_tick_ms = BASE_TICK_MS;
                        last_space_toggle = Instant::now();
                    }
                    break;
                }
            }
        } else {
            // ===== 游戏进行状态 =====
            let frame_start = Instant::now();

            // 非阻塞处理所有待处理的输入事件
            while event::poll(Duration::ZERO)? {
                if let Event::Key(key) = event::read()? {
                    match key.code {
                        // WASD 方向控制：按下 / 按住均生效
                        KeyCode::Char('w') | KeyCode::Char('W')
                            if key.kind != KeyEventKind::Release =>
                        {
                            game.change_direction(Direction::Up);
                        }
                        KeyCode::Char('s') | KeyCode::Char('S')
                            if key.kind != KeyEventKind::Release =>
                        {
                            game.change_direction(Direction::Down);
                        }
                        KeyCode::Char('a') | KeyCode::Char('A')
                            if key.kind != KeyEventKind::Release =>
                        {
                            game.change_direction(Direction::Left);
                        }
                        KeyCode::Char('d') | KeyCode::Char('D')
                            if key.kind != KeyEventKind::Release =>
                        {
                            game.change_direction(Direction::Right);
                        }
                        // 空格：每次完整键击切换速度（冷却防长按抖动）
                        KeyCode::Char(' ')
                            if last_space_toggle.elapsed()
                                > Duration::from_millis(SPACE_COOLDOWN_MS) =>
                        {
                            current_tick_ms = if current_tick_ms == BASE_TICK_MS {
                                FAST_TICK_MS
                            } else {
                                BASE_TICK_MS
                            };
                            last_space_toggle = Instant::now();
                        }
                        _ => {}
                    }
                }
            }

            // 游戏逻辑 tick —— 独立于渲染帧率的固定节拍
            if last_game_tick.elapsed() >= Duration::from_millis(current_tick_ms) {
                game.update();
                last_game_tick = Instant::now();
            }

            // 渲染当前帧（双缓冲：内存构建 → 一次 flush）
            render::render(&mut stdout, &game)?;

            // 帧率控制：补足到 16ms（60 FPS）
            let elapsed = frame_start.elapsed();
            let frame_dur = Duration::from_millis(FRAME_MS);
            if elapsed < frame_dur {
                thread::sleep(frame_dur - elapsed);
            }
        }
    }

    // ── 终端恢复 ──
    stdout.execute(cursor::Show)?;
    stdout.execute(terminal::LeaveAlternateScreen)?;
    terminal::disable_raw_mode()?;

    Ok(())
}
