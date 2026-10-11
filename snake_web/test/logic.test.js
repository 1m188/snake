/**
 * 核心逻辑冒烟测试。
 *
 * 运行方式：node test/logic.test.js
 * 不依赖任何测试框架，仅使用 node 内置 assert，逐条输出结果，
 * 任一失败则进程退出码为 1。
 *
 * 覆盖范围：蛇实体（snake.js）、食物生成（food.js）、
 * 游戏主控状态机与加速曲线（game.js）。
 */

'use strict';

const assert = require('assert');
const Snake = require('../js/snake.js');
const Food = require('../js/food.js');
const { Game, moveIntervalForScore } = require('../js/game.js');

const COLS = 20;
const ROWS = 20;
const STEP = 1000 / 60; // 一个逻辑帧的时长（毫秒）

let passed = 0;
const failures = [];

function test(name, fn) {
    try {
        fn();
        passed++;
        console.log('  \u2713 ' + name);
    } catch (err) {
        failures.push(name);
        console.log('  \u2717 ' + name);
        console.log('      ' + err.message);
    }
}

function section(title) {
    console.log('\n' + title);
}

/** 创建已完成开局、随机源固定为 () => 0 的游戏。 */
function freshGame() {
    const game = new Game({ cols: COLS, rows: ROWS, random: () => 0 });
    game.startNewRound();
    return game;
}

/** 把食物精确放到指定空格（按生成算法的行主序自由格列表选位）。 */
function placeFoodAt(game, x, y) {
    const occupied = new Set(
        game.snake.body.map(function (p) { return p.x + ',' + p.y; })
    );
    let freeBefore = 0;
    for (let cy = 0; cy < ROWS; cy++) {
        for (let cx = 0; cx < COLS; cx++) {
            if (cx === x && cy === y) {
                game.food.spawn(COLS, ROWS, occupied, function () {
                    return freeBefore / ((COLS * ROWS) - occupied.size - 1);
                });
                return;
            }
            if (!occupied.has(cx + ',' + cy)) {
                freeBefore++;
            }
        }
    }
    throw new Error('placeFoodAt: 目标格 (' + x + ', ' + y + ') 被蛇身占用');
}

/** 推进若干逻辑帧，返回期间蛇是否吃到过食物。 */
function advance(game, frames) {
    const before = game.score;
    for (let i = 0; i < frames; i++) {
        game.update(STEP);
    }
    return game.score > before;
}

/** 在蛇头前方一格放置食物并推进一个移动间隔，让蛇吃掉它。 */
function eatOnce(game) {
    const dir = game.snake.direction;
    const dx = dir === 'left' ? -1 : dir === 'right' ? 1 : 0;
    const dy = dir === 'up' ? -1 : dir === 'down' ? 1 : 0;
    placeFoodAt(game, game.snake.head.x + dx, game.snake.head.y + dy);
    const eaten = advance(game, game.moveInterval);
    assert.ok(eaten, 'eatOnce: 蛇应在移动后吃到食物');
}

// ---------------------------------------------------------------------------
section('Snake 蛇实体');

test('初始 3 节：头在起点，身体向反方向延伸', function () {
    const s = new Snake(10, 10, 'right', 3);
    assert.deepStrictEqual(s.head, { x: 10, y: 10 });
    assert.deepStrictEqual(s.body, [
        { x: 10, y: 10 },
        { x: 9, y: 10 },
        { x: 8, y: 10 },
    ]);
    assert.strictEqual(s.length, 3);
    assert.strictEqual(s.direction, 'right');
});

test('move()：沿方向前进一格，长度不变，尾部跟进', function () {
    const s = new Snake(10, 10, 'right', 3);
    s.move();
    assert.deepStrictEqual(s.head, { x: 11, y: 10 });
    assert.strictEqual(s.length, 3);
    assert.deepStrictEqual(s.body, [
        { x: 11, y: 10 },
        { x: 10, y: 10 },
        { x: 9, y: 10 },
    ]);
});

test('setDirection()：忽略 180 度反向，接受垂直方向', function () {
    const s = new Snake(10, 10, 'right', 3);
    s.setDirection('left');
    assert.strictEqual(s.direction, 'right');
    s.setDirection('up');
    assert.strictEqual(s.direction, 'up');
    s.setDirection('down');
    assert.strictEqual(s.direction, 'up');
});

test('grow()：下一次移动时增长一节（尾部保留）', function () {
    const s = new Snake(10, 10, 'right', 3);
    s.grow();
    s.move();
    assert.strictEqual(s.length, 4);
    assert.deepStrictEqual(s.body, [
        { x: 11, y: 10 },
        { x: 10, y: 10 },
        { x: 9, y: 10 },
        { x: 8, y: 10 },
    ]);
});

test('checkSelfCollision()：5 节蛇绕 2x2 方块回咬自己时为 true', function () {
    const s = new Snake(10, 10, 'right', 3);
    s.grow();
    s.move();
    s.grow();
    s.move(); // 两次各带一次移动，长度达到 5
    assert.strictEqual(s.length, 5);
    assert.strictEqual(s.checkSelfCollision(), false);
    s.setDirection('up');
    s.move();
    s.setDirection('left');
    s.move();
    s.setDirection('down');
    s.move();
    assert.strictEqual(s.checkSelfCollision(), true);
});

test('occupies()：正确判断坐标是否被蛇身占据', function () {
    const s = new Snake(10, 10, 'right', 3);
    assert.strictEqual(s.occupies(10, 10), true);
    assert.strictEqual(s.occupies(9, 10), true);
    assert.strictEqual(s.occupies(0, 0), false);
});

// ---------------------------------------------------------------------------
section('Food 食物生成');

test('spawn()：生成的食物坐标在场地边界内', function () {
    const f = new Food();
    f.spawn(COLS, ROWS, new Set(), function () {
        return 0.99;
    });
    const p = f.position;
    assert.ok(p.x >= 0 && p.x < COLS, 'x 应在 [0, cols) 内，实际 ' + p.x);
    assert.ok(p.y >= 0 && p.y < ROWS, 'y 应在 [0, rows) 内，实际 ' + p.y);
});

test('spawn()：不落在被占用格子上（首个自由格）', function () {
    const f = new Food();
    const occupied = new Set(['0,0', '1,0', '2,0', '0,1']);
    f.spawn(COLS, ROWS, occupied, function () {
        return 0;
    });
    assert.deepStrictEqual(f.position, { x: 3, y: 0 });
    assert.ok(!occupied.has(f.position.x + ',' + f.position.y));
});

test('spawn()：注入同一随机值时选取确定的自由格', function () {
    const occupied = new Set(['0,0', '1,0', '2,0', '0,1']);
    const a = new Food();
    a.spawn(COLS, ROWS, occupied, function () {
        return 0;
    });
    const b = new Food();
    b.spawn(COLS, ROWS, occupied, function () {
        return 1 / ((COLS * ROWS) - occupied.size - 1);
    });
    assert.deepStrictEqual(a.position, { x: 3, y: 0 });
    assert.deepStrictEqual(b.position, { x: 4, y: 0 });
});

// ---------------------------------------------------------------------------
section('Game 游戏主控');

test('startNewRound()：进入 playing，蛇居中 3 节，分数与速度重置，食物已生成', function () {
    const g = freshGame();
    assert.strictEqual(g.state, 'playing');
    assert.strictEqual(g.score, 0);
    assert.strictEqual(g.moveInterval, 10);
    assert.deepStrictEqual(g.snake.head, { x: 10, y: 10 });
    assert.strictEqual(g.snake.length, 3);
    assert.strictEqual(g.snake.direction, 'right');
    assert.ok(g.food.position !== null, '开局应已生成食物');
});

test('update()：按帧计数控制移动，每 10 个逻辑帧移动一格', function () {
    const g = freshGame();
    advance(g, 9);
    assert.deepStrictEqual(g.snake.head, { x: 10, y: 10 }, '前 9 帧不应移动');
    advance(g, 1);
    assert.deepStrictEqual(g.snake.head, { x: 11, y: 10 }, '第 10 帧应移动一格');
    advance(g, 9);
    assert.deepStrictEqual(g.snake.head, { x: 11, y: 10 }, '第 19 帧不应移动');
    advance(g, 1);
    assert.deepStrictEqual(g.snake.head, { x: 12, y: 10 }, '第 20 帧应再移动一格');
});

test('撞墙：蛇头越过边界即进入 game_over', function () {
    const g = freshGame();
    advance(g, 105); // 从 x=10 向右移动 10 次，第 10 次到达 x=20
    assert.strictEqual(g.state, 'game_over');
});

test('game_over 后 update() 不再推进蛇', function () {
    const g = freshGame();
    advance(g, 105);
    const head = { x: g.snake.head.x, y: g.snake.head.y };
    advance(g, 12);
    assert.strictEqual(g.state, 'game_over');
    assert.deepStrictEqual(g.snake.head, head);
});

test('吃到食物：得分 +1，增长延迟到下次移动体现，食物在蛇身之外重生', function () {
    const g = freshGame();
    eatOnce(g);
    assert.strictEqual(g.score, 1);
    assert.strictEqual(g.snake.length, 3, '吃到瞬间不立即变长（增长延迟一次移动）');
    advance(g, 10);
    assert.strictEqual(g.snake.length, 4, '下次移动后长度 +1');
    assert.deepStrictEqual(g.snake.head, { x: 12, y: 10 });
    assert.strictEqual(g.state, 'playing');
    const occupied = new Set(
        g.snake.body.map(function (p) { return p.x + ',' + p.y; })
    );
    assert.ok(
        !occupied.has(g.food.position.x + ',' + g.food.position.y),
        '重生食物不应落在蛇身'
    );
});

test('加速：吃满 3 个食物后移动间隔从 10 帧降到 9 帧', function () {
    const g = freshGame();
    assert.strictEqual(g.moveInterval, 10);
    eatOnce(g);
    eatOnce(g);
    assert.strictEqual(g.moveInterval, 10);
    eatOnce(g);
    assert.strictEqual(g.moveInterval, 9);
});

test('加速曲线：moveIntervalForScore 每超过 3 分减 1 帧，下限 2 帧', function () {
    const cases = [[0, 10], [1, 10], [2, 10], [3, 9], [6, 8], [12, 6], [21, 3], [24, 2], [100, 2]];
    for (const pair of cases) {
        assert.strictEqual(
            moveIntervalForScore(pair[0]),
            pair[1],
            'score=' + pair[0] + ' 时间隔应为 ' + pair[1]
        );
    }
});

test('方向缓冲：仅在移动帧应用缓冲的方向', function () {
    const g = freshGame();
    g.setPendingDirection('up');
    advance(g, 1);
    assert.strictEqual(g.snake.direction, 'right', '非移动帧不应应用方向');
    advance(g, 9); // 到达第 10 帧
    assert.strictEqual(g.snake.direction, 'up');
    assert.deepStrictEqual(g.snake.head, { x: 10, y: 9 });
});

test('方向缓冲：同一间隔内多次按键取最后一个', function () {
    const g = freshGame();
    g.setPendingDirection('up');
    g.setPendingDirection('down');
    advance(g, 10);
    assert.strictEqual(g.snake.direction, 'down');
    assert.deepStrictEqual(g.snake.head, { x: 10, y: 11 });
});

test('方向缓冲：与当前方向相反的缓冲在应用时被忽略', function () {
    const g = freshGame();
    g.setPendingDirection('left');
    advance(g, 10);
    assert.strictEqual(g.snake.direction, 'right');
    assert.deepStrictEqual(g.snake.head, { x: 11, y: 10 });
});

test('togglePause()：playing 与 paused 互切，暂停时逻辑帧停走', function () {
    const g = freshGame();
    g.togglePause();
    assert.strictEqual(g.state, 'paused');
    const head = { x: g.snake.head.x, y: g.snake.head.y };
    advance(g, 12);
    assert.deepStrictEqual(g.snake.head, head, '暂停时蛇不应移动');
    g.togglePause();
    assert.strictEqual(g.state, 'playing');
    advance(g, 10);
    assert.deepStrictEqual(g.snake.head, { x: head.x + 1, y: head.y }, '恢复后应继续移动');
});

test('backToStart()：返回开始界面后逻辑帧冻结，可再次开局', function () {
    const g = freshGame();
    g.backToStart();
    assert.strictEqual(g.state, 'start');
    const head = { x: g.snake.head.x, y: g.snake.head.y };
    advance(g, 12);
    assert.deepStrictEqual(g.snake.head, head, 'start 状态下蛇不应移动');
    g.startNewRound();
    assert.strictEqual(g.state, 'playing');
    assert.strictEqual(g.score, 0);
    assert.deepStrictEqual(g.snake.head, { x: 10, y: 10 }, '重新开局应重置到场地中央');
});

// ---------------------------------------------------------------------------
console.log('\n结果：' + passed + ' 通过，' + failures.length + ' 失败');
if (failures.length > 0) {
    console.log('失败用例：\n  - ' + failures.join('\n  - '));
    process.exitCode = 1;
}
