/**
 * 入口装配模块。
 *
 * 本模块负责创建游戏、渲染器、存储与输入控制器等实例并完成装配：
 * - 以 requestAnimationFrame 驱动主循环：推进逻辑帧 → 整帧重绘；
 * - 把输入意图按当前游戏状态分发给游戏对象；
 * - 监听状态切换，维护覆盖层（开始 / 暂停 / 结束）与分数的 DOM 更新，
 *   并在游戏结束时结算最高分。
 */

(function (global) {
    'use strict';

    const C = global.SnakeConstants;

    const canvas = document.getElementById('board');
    const scoreEl = document.getElementById('score');
    const highScoreEl = document.getElementById('high-score');
    const overlays = {
        start: document.getElementById('overlay-start'),
        paused: document.getElementById('overlay-paused'),
        game_over: document.getElementById('overlay-gameover'),
    };
    const finalScoreEl = document.getElementById('final-score');
    const finalHighScoreEl = document.getElementById('final-high-score');
    const recordLineEl = document.getElementById('record-line');

    const game = new Game();
    const renderer = new Renderer(canvas);
    const highScoreStore = new HighScoreStore();

    /** 已展示的最高分（用于结算破纪录判定与 DOM 更新）。 */
    let shownHighScore = highScoreStore.get();
    /** 上一帧的游戏状态，用于检测状态切换。 */
    let lastState = game.state;
    /** 缓存的上次渲染分数，避免每帧改写 DOM。 */
    let shownScore = -1;

    /** 主操作：开始 / 暂停切换 / 结束后重开（含义随状态变化）。 */
    function handlePrimary() {
        if (
            game.state === C.GAME_STATE.START ||
            game.state === C.GAME_STATE.GAME_OVER
        ) {
            game.startNewRound();
        } else {
            game.togglePause();
        }
    }

    /** 方向：开始界面按方向键直接开局，游戏中缓冲转向。 */
    function handleDirection(direction) {
        if (game.state === C.GAME_STATE.START) {
            game.startNewRound();
        }
        if (game.state === C.GAME_STATE.PLAYING) {
            game.setPendingDirection(direction);
        }
    }

    /** 返回：结束界面按 ESC 回到开始界面。 */
    function handleBack() {
        if (game.state === C.GAME_STATE.GAME_OVER) {
            game.backToStart();
        }
    }

    /** 游戏结束结算：保存最高分并同步结束界面与顶部计分栏文案。 */
    function settleGameOver() {
        const isNewRecord = game.score > shownHighScore && game.score > 0;
        highScoreStore.save(game.score);
        shownHighScore = highScoreStore.get();
        finalScoreEl.textContent = game.score;
        finalHighScoreEl.textContent = shownHighScore;
        highScoreEl.textContent = shownHighScore;
        recordLineEl.hidden = !isNewRecord;
    }

    /** 按状态显示对应覆盖层，playing 时全部隐藏。 */
    function showOverlay(state) {
        overlays.start.hidden = state !== C.GAME_STATE.START;
        overlays.paused.hidden = state !== C.GAME_STATE.PAUSED;
        overlays.game_over.hidden = state !== C.GAME_STATE.GAME_OVER;
    }

    /** 分数变化时更新 DOM。 */
    function refreshScore() {
        if (game.score !== shownScore) {
            shownScore = game.score;
            scoreEl.textContent = shownScore;
        }
    }

    /** 状态切换时的副作用处理。 */
    function handleStateChange(state) {
        if (state === C.GAME_STATE.GAME_OVER) {
            settleGameOver();
        }
        showOverlay(state);
    }

    // 装配输入：键盘 + 触屏滑动 + 虚拟方向键
    new InputController(
        {
            onDirection: handleDirection,
            onPrimary: handlePrimary,
            onBack: handleBack,
        },
        canvas,
        document.getElementById('touch-controls')
    );

    // 覆盖层按钮
    document.getElementById('btn-start').addEventListener('click', function () {
        game.startNewRound();
    });
    document.getElementById('btn-resume').addEventListener('click', function () {
        game.togglePause();
    });
    document.getElementById('btn-restart').addEventListener('click', function () {
        game.startNewRound();
    });

    // 初始界面
    highScoreEl.textContent = shownHighScore;
    showOverlay(lastState);
    refreshScore();

    // 主循环：按真实时间推进逻辑帧，每帧整体重绘
    let lastTime = performance.now();
    function loop(now) {
        const deltaMs = now - lastTime;
        lastTime = now;

        game.update(deltaMs);
        if (game.state !== lastState) {
            lastState = game.state;
            handleStateChange(lastState);
        }
        renderer.draw(game);
        refreshScore();

        requestAnimationFrame(loop);
    }
    requestAnimationFrame(loop);
})(typeof window !== 'undefined' ? window : globalThis);
