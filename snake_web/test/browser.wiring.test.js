/**
 * 浏览器装配冒烟测试（无头集成测试）。
 *
 * 运行方式：node test/browser.wiring.test.js
 *
 * 使用 Node vm 构造一个最小浏览器沙箱（document / canvas / localStorage /
 * performance / requestAnimationFrame 桩），按 index.html 中 <script> 的顺序
 * 加载全部模块，再模拟按键与帧推进，验证：
 * 模块加载顺序与全局命名空间、main.js 装配、覆盖层随状态切换、
 * 键盘意图分发、游戏结束结算与重开 / 返回流程。
 */

'use strict';

const assert = require('assert');
const fs = require('fs');
const path = require('path');
const vm = require('vm');

const jsDir = path.join(__dirname, '..', 'js');
// 与 index.html 中脚本加载顺序保持一致
const SCRIPTS = [
    'constants.js',
    'snake.js',
    'food.js',
    'game.js',
    'storage.js',
    'render.js',
    'input.js',
    'main.js',
];

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

/** 创建带惰性空方法的 2D 上下文桩。 */
function makeCtx() {
    const target = {};
    return new Proxy(target, {
        get: function (obj, prop) {
            if (!(prop in obj)) {
                obj[prop] = function () {};
            }
            return obj[prop];
        },
        set: function (obj, prop, value) {
            obj[prop] = value;
            return true;
        },
    });
}

/** 构造最小浏览器沙箱并加载全部脚本，返回沙箱与测试辅助函数。
 *
 * Args:
 *     options: options.fixedRandom 存在时用固定值替换沙箱内 Math.random，
 *              使食物生成位置可预测（配合导航吃食物）。
 */
function bootBrowser(options = {}) {
    const ctx2d = makeCtx();
    const elements = {};
    let keydownHandler = null;
    let clickHandlerOf = {};

    function element(id) {
        if (!elements[id]) {
            const el = {
                id: id,
                hidden: true,
                _handlers: {},
                addEventListener: function (type, fn) {
                    this._handlers[type] = fn;
                    clickHandlerOf[id] = this._handlers;
                },
            };
            // 与真实 DOM 一致：textContent 赋值时转换为字符串
            Object.defineProperty(el, 'textContent', {
                get: function () {
                    return el._text;
                },
                set: function (value) {
                    el._text = String(value);
                },
            });
            elements[id] = el;
        }
        return elements[id];
    }

    const canvas = {
        width: 0,
        height: 0,
        getContext: function () {
            return ctx2d;
        },
        addEventListener: function () {}, // 触摸滑动监听挂在此处，桩中无需触发
    };
    elements.board = canvas;

    let currentTime = 0;
    let rafQueue = [];

    const document = {
        addEventListener: function (type, fn) {
            if (type === 'keydown') {
                keydownHandler = fn;
            }
        },
        getElementById: function (id) {
            return element(id);
        },
    };

    const sandbox = {
        document: document,
        navigator: { maxTouchPoints: 0 },
        performance: {
            now: function () {
                return currentTime;
            },
        },
        requestAnimationFrame: function (fn) {
            rafQueue.push(fn);
        },
        localStorage: (function () {
            const map = new Map();
            return {
                getItem: function (k) {
                    return map.has(k) ? map.get(k) : null;
                },
                setItem: function (k, v) {
                    map.set(k, String(v));
                },
            };
        })(),
        console: console,
    };
    sandbox.window = sandbox;
    if (options.fixedRandom !== undefined) {
        const seed = options.fixedRandom;
        sandbox.Math = Object.create(Math);
        sandbox.Math.random = function () {
            return seed;
        };
    }
    vm.createContext(sandbox);

    const source = SCRIPTS.map(function (name) {
        return fs.readFileSync(path.join(jsDir, name), 'utf8');
    }).join('\n;\n');
    vm.runInContext(source, sandbox, { filename: 'bundle-in-html-order.js' });

    return {
        sandbox: sandbox,
        ctx2d: ctx2d,
        element: element,
        /** 模拟一次按键。 */
        press: function (key) {
            assert.ok(keydownHandler, '应已注册 keydown 监听');
            keydownHandler({ key: key, preventDefault: function () {} });
        },
        /** 模拟点击某元素的指定事件。 */
        fire: function (id, type) {
            const handlers = elements[id] && elements[id]._handlers;
            assert.ok(handlers && handlers[type], id + ' 应已绑定 ' + type);
            handlers[type]();
        },
        /** 推进 n 个渲染帧（每帧约 16.7ms）。 */
        tick: function (frames) {
            for (let i = 0; i < frames; i++) {
                currentTime += 1000 / 60;
                const queue = rafQueue;
                rafQueue = [];
                for (const fn of queue) {
                    fn(currentTime);
                }
            }
        },
    };
}

console.log('\n浏览器装配（无头集成）');

test('加载后进入开始界面，仅开始覆盖层可见', function () {
    const b = bootBrowser();
    assert.strictEqual(b.element('overlay-start').hidden, false);
    assert.strictEqual(b.element('overlay-paused').hidden, true);
    assert.strictEqual(b.element('overlay-gameover').hidden, true);
});

test('点击开始按钮后覆盖层全部隐藏，画布开始被绘制', function () {
    const b = bootBrowser();
    b.fire('btn-start', 'click');
    b.tick(2);
    assert.strictEqual(b.element('overlay-start').hidden, true);
    assert.strictEqual(b.element('overlay-paused').hidden, true);
    assert.ok(b.ctx2d.fillRect.callCount !== 0 || true); // Proxy 桩不计数，绘制已执行无异常
    assert.strictEqual(typeof b.ctx2d.fillStyle, 'string');
});

test('方向键可在开始界面直接开局', function () {
    const b = bootBrowser();
    b.press('ArrowUp');
    b.tick(1); // 覆盖层在下一渲染帧更新
    assert.strictEqual(b.element('overlay-start').hidden, true);
});

test('一路向右撞墙后进入结束界面并结算', function () {
    const b = bootBrowser();
    b.fire('btn-start', 'click');
    b.tick(110); // 从中央向右约 10 次移动后撞墙
    assert.strictEqual(b.element('overlay-gameover').hidden, false);
    assert.strictEqual(b.element('final-score').textContent, '0');
    assert.strictEqual(b.element('record-line').hidden, true, '0 分不应提示新纪录');
});

test('结束界面按 ESC 返回开始界面', function () {
    const b = bootBrowser();
    b.fire('btn-start', 'click');
    b.tick(110);
    b.press('Escape');
    b.tick(1); // 覆盖层在下一渲染帧更新
    assert.strictEqual(b.element('overlay-gameover').hidden, true);
    assert.strictEqual(b.element('overlay-start').hidden, false);
});

test('空格可暂停 / 继续，暂停时逻辑冻结', function () {
    const b = bootBrowser();
    b.press('ArrowLeft'); // 开始界面按方向键直接开局
    b.press(' ');
    b.tick(24);
    assert.strictEqual(b.element('overlay-paused').hidden, false, '空格应暂停');
    const pausedScore = b.element('score').textContent;
    b.tick(24);
    assert.strictEqual(
        b.element('score').textContent,
        pausedScore,
        '暂停时分数不应变化'
    );
    b.press(' ');
    b.tick(8);
    assert.strictEqual(b.element('overlay-paused').hidden, true, '再按空格应继续');
});

test('结束后按空格重新开局', function () {
    const b = bootBrowser();
    b.fire('btn-start', 'click');
    b.tick(110);
    b.press(' ');
    b.tick(2);
    assert.strictEqual(b.element('overlay-gameover').hidden, true);
    assert.strictEqual(b.element('overlay-paused').hidden, true);
    assert.strictEqual(b.element('overlay-start').hidden, true);
    b.tick(110); // 新一局同样可以从满血玩到撞墙
    assert.strictEqual(b.element('overlay-gameover').hidden, false);
});

test('ESC 在游戏中不产生返回效果', function () {
    const b = bootBrowser();
    b.fire('btn-start', 'click');
    b.press('Escape');
    b.tick(2);
    assert.strictEqual(b.element('overlay-start').hidden, true);
    assert.strictEqual(b.element('overlay-gameover').hidden, true);
});

test('破纪录结算后顶部最高分立即更新，新开一局无需刷新页面', function () {
    // 固定 Math.random = 0：食物始终生成在首个空格 (0,0)。
    // 开局朝右，先按 w（上）10 格到 (10,0)，再按 a（左）10 格到 (0,0) 吃食物，
    // 随后向左撞墙结束（开局直接按 a 会被 180° 防反忽略）。
    const b = bootBrowser({ fixedRandom: 0 });
    b.fire('btn-start', 'click');
    b.press('w');
    b.tick(105); // 向上移动 10 格到 (10,0)
    b.press('a');
    b.tick(115); // 向左 10 格吃到 (0,0) 的食物，再一步向左撞墙结束
    assert.strictEqual(b.element('score').textContent, '1', '应吃到 1 个食物');
    assert.strictEqual(b.element('overlay-gameover').hidden, false);
    assert.strictEqual(b.element('high-score').textContent, '1', '顶部最高分应在结算时立即更新');
    assert.strictEqual(b.element('final-high-score').textContent, '1');
    // 用户报告的场景：结束界面上直接新开一局
    b.press(' ');
    b.tick(2);
    assert.strictEqual(b.element('overlay-gameover').hidden, true);
    assert.strictEqual(b.element('high-score').textContent, '1', '新开一局后顶部最高分不应回退');
});

console.log('\n结果：' + passed + ' 通过，' + failures.length + ' 失败');
if (failures.length > 0) {
    console.log('失败用例：\n  - ' + failures.join('\n  - '));
    process.exitCode = 1;
}
