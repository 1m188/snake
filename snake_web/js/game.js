/**
 * 游戏主控模块。
 *
 * 本模块负责管理游戏状态机、帧循环推进和各模块间的协调。
 * 游戏以固定 60FPS 的逻辑帧率运行；蛇的移动与帧渲染解耦：
 * 方向按键缓冲到 pendingDirection，仅在移动帧应用（取最后一个），
 * 通过帧计数控制移动间隔，并随得分逐渐加速。
 *
 * 本模块不依赖 DOM，渲染与输入由 render / input / main 模块驱动。
 */

(function (global) {
    'use strict';

    const C = global.SnakeConstants || require('./constants.js');
    const Snake = global.Snake || require('./snake.js');
    const Food = global.Food || require('./food.js');

    /** 单个逻辑帧的时长（毫秒）。 */
    const LOGIC_FRAME_MS = 1000 / C.LOGIC_FPS;
    /** 单次 update 允许累积的最大真实时长（毫秒），防止切后台后补帧过多。 */
    const MAX_DELTA_MS = 250;

    class Game {
        /**
         * 创建游戏对象。
         *
         * Args:
         *     options: 可选配置。
         *     options.cols: 场地列数，默认取常量 GRID_COLS。
         *     options.rows: 场地行数，默认取常量 GRID_ROWS。
         *     options.random: 随机函数注入（供食物生成），默认 Math.random。
         */
        constructor(options = {}) {
            this._cols = options.cols ?? C.GRID_COLS;
            this._rows = options.rows ?? C.GRID_ROWS;
            this._random = options.random ?? Math.random;
            this._state = C.GAME_STATE.START;
            this._score = 0;
            this._snake = null;
            this._food = null;
            this._moveInterval = C.INITIAL_MOVE_INTERVAL;
            this._frameCount = 0;
            this._accumulator = 0;
            this._pendingDirection = null;
        }

        /** 当前游戏状态（GAME_STATE 中的取值）。 */
        get state() {
            return this._state;
        }

        /** 当前局得分。 */
        get score() {
            return this._score;
        }

        /** 蛇实体。 */
        get snake() {
            return this._snake;
        }

        /** 食物实体。 */
        get food() {
            return this._food;
        }

        /** 当前移动间隔（逻辑帧数），值越小蛇越快。 */
        get moveInterval() {
            return this._moveInterval;
        }

        /**
         * 初始化新一局：重置分数、速度与方向缓冲，创建蛇并生成食物。
         *
         * 蛇初始化到场地中央，长度为初始节数，方向向右。
         */
        startNewRound() {
            this._score = 0;
            this._snake = new Snake(
                Math.floor(this._cols / 2),
                Math.floor(this._rows / 2),
                C.DIRECTION.RIGHT
            );
            this._food = new Food();
            this._respawnFood();
            this._moveInterval = C.INITIAL_MOVE_INTERVAL;
            this._frameCount = 0;
            this._accumulator = 0;
            this._pendingDirection = null;
            this._state = C.GAME_STATE.PLAYING;
        }

        /**
         * 缓冲一个方向输入，移动帧时应用（同一间隔内取最后一次）。
         *
         * Args:
         *     direction: 目标方向（'up' | 'down' | 'left' | 'right'）。
         */
        setPendingDirection(direction) {
            this._pendingDirection = direction;
        }

        /** 暂停 / 恢复游戏（仅在 playing 与 paused 之间切换）。 */
        togglePause() {
            if (this._state === C.GAME_STATE.PLAYING) {
                this._state = C.GAME_STATE.PAUSED;
            } else if (this._state === C.GAME_STATE.PAUSED) {
                this._state = C.GAME_STATE.PLAYING;
            }
        }

        /**
         * 返回开始界面：冻结逻辑帧并清空方向缓冲，
         * 保留当前局面作为开始界面的背景画面。
         */
        backToStart() {
            this._state = C.GAME_STATE.START;
            this._pendingDirection = null;
        }

        /**
         * 推进游戏：按真实时间累积逻辑帧并逐帧更新。
         *
         * 以固定 60FPS 逻辑帧率驱动，与实际刷新率无关；
         * 仅在 playing 状态下推进，暂停或结束时冻结。
         *
         * Args:
         *     deltaMs: 距上次调用的真实时间（毫秒）。
         */
        update(deltaMs) {
            if (this._state !== C.GAME_STATE.PLAYING) {
                return;
            }
            this._accumulator += Math.min(deltaMs, MAX_DELTA_MS);
            while (this._accumulator >= LOGIC_FRAME_MS) {
                this._accumulator -= LOGIC_FRAME_MS;
                this._tick();
            }
        }

        /**
         * 执行单个逻辑帧的更新。
         *
         * 通过帧计数控制蛇的移动频率；移动帧时先应用缓冲的方向
         * （180 度防反），再移动，随后依次检测墙体碰撞、自身碰撞
         * 和食物吃取。
         */
        _tick() {
            this._frameCount++;
            if (this._frameCount % this._moveInterval !== 0) {
                return;
            }

            if (this._pendingDirection !== null) {
                this._snake.setDirection(this._pendingDirection);
                this._pendingDirection = null;
            }

            this._snake.move();

            const head = this._snake.head;
            if (
                head.x < 0 ||
                head.x >= this._cols ||
                head.y < 0 ||
                head.y >= this._rows
            ) {
                this._state = C.GAME_STATE.GAME_OVER;
                return;
            }

            if (this._snake.checkSelfCollision()) {
                this._state = C.GAME_STATE.GAME_OVER;
                return;
            }

            if (
                head.x === this._food.position.x &&
                head.y === this._food.position.y
            ) {
                this._snake.grow();
                this._score++;
                this._moveInterval = moveIntervalForScore(this._score);
                this._respawnFood();
            }
        }

        /** 让食物在未被蛇身占用的格子中重新生成。 */
        _respawnFood() {
            this._food.spawn(this._cols, this._rows, this._occupiedCells(), this._random);
        }

        /** 返回蛇身所有格子的占用集合（键为 "x,y"）。 */
        _occupiedCells() {
            return new Set(
                this._snake.body.map(function (p) {
                    return p.x + ',' + p.y;
                })
            );
        }
    }

    /**
     * 根据得分计算移动间隔（加速曲线）。
     *
     * 每吃 FOODS_PER_SPEEDUP 个食物，间隔减 1 帧，
     * 最快不低于 FASTEST_MOVE_INTERVAL。
     *
     * Args:
     *     score: 当前得分。
     *
     * Returns:
     *     移动间隔（逻辑帧数）。
     */
    function moveIntervalForScore(score) {
        const speedups = Math.floor(score / C.FOODS_PER_SPEEDUP);
        return Math.max(
            C.FASTEST_MOVE_INTERVAL,
            C.INITIAL_MOVE_INTERVAL - speedups
        );
    }

    if (typeof module !== 'undefined' && module.exports) {
        module.exports = { Game: Game, moveIntervalForScore: moveIntervalForScore };
    } else {
        global.Game = Game;
        global.moveIntervalForScore = moveIntervalForScore;
    }
})(typeof window !== 'undefined' ? window : globalThis);
