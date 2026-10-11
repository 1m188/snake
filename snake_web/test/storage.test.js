/**
 * 最高分存储模块的冒烟测试。
 *
 * 运行方式：node test/storage.test.js
 * 通过注入内存版 storage 桩隔离 localStorage，覆盖读写、
 * 只升不降与存储异常降级四类行为。
 */

'use strict';

const assert = require('assert');
const HighScoreStore = require('../js/storage.js');

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

/** 创建内存版 storage 桩，接口与 localStorage 一致。 */
function fakeStorage() {
    const map = new Map();
    return {
        getItem: function (key) {
            return map.has(key) ? map.get(key) : null;
        },
        setItem: function (key, value) {
            map.set(key, String(value));
        },
    };
}

/** 创建读写总是抛异常的 storage 桩，用于验证降级路径。 */
function brokenStorage() {
    return {
        getItem: function () {
            throw new Error('storage unavailable');
        },
        setItem: function () {
            throw new Error('storage unavailable');
        },
    };
}

console.log('\nHighScoreStore 最高分存储');

test('无记录时 get() 返回 0', function () {
    const store = new HighScoreStore(fakeStorage());
    assert.strictEqual(store.get(), 0);
});

test('save() 后 get() 返回已保存的分数', function () {
    const store = new HighScoreStore(fakeStorage());
    store.save(7);
    assert.strictEqual(store.get(), 7);
});

test('save() 更低分数不会覆盖历史最高', function () {
    const store = new HighScoreStore(fakeStorage());
    store.save(10);
    store.save(4);
    assert.strictEqual(store.get(), 10);
});

test('存储不可用时 get() 安全返回 0，save() 不抛异常', function () {
    const store = new HighScoreStore(brokenStorage());
    assert.strictEqual(store.get(), 0);
    assert.doesNotThrow(function () {
        store.save(3);
    });
});

test('存储值为非数字时 get() 返回 0', function () {
    const storage = fakeStorage();
    storage.setItem('snake_web_high_score', 'not-a-number');
    const store = new HighScoreStore(storage);
    assert.strictEqual(store.get(), 0);
});

console.log('\n结果：' + passed + ' 通过，' + failures.length + ' 失败');
if (failures.length > 0) {
    console.log('失败用例：\n  - ' + failures.join('\n  - '));
    process.exitCode = 1;
}
