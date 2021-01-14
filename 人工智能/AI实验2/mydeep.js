const { stdout, exit } = require('process');

const r1 = require('readline').createInterface({ input: process.stdin, output: process.stdout });

function readlineSync() {
    return new Promise((resolve, reject) => {
        r1.resume();
        r1.on('line', function (data) {
            r1.pause();
            resolve(data);
        });
    });
}

const optionOfBlock = [
    ['r', 'd'], ['r', 'd', 'l'], ['d', 'l'],
    ['u', 'r', 'd'], ['u', 'r', 'd', 'l'], ['u', 'd', 'l'],
    ['u', 'r'], ['u', 'r', 'l'], ['u', 'l']
];
let successStatu = [1, 2, 3, 8, 0, 4, 7, 6, 5];

let initStatu = [8, 0, 3, 2, 1, 4, 7, 6, 5];
let initPos = initStatu.findIndex(value => value === 0);
let res = [[...initStatu]];
let flag = false;

function exchange(pos, direction, curStatu) {
    switch (direction) {
        case 'u': [curStatu[pos - 3], curStatu[pos]] = [curStatu[pos], curStatu[pos - 3]]; return pos - 3;
        case 'r': [curStatu[pos + 1], curStatu[pos]] = [curStatu[pos], curStatu[pos + 1]]; return pos + 1;
        case 'd': [curStatu[pos + 3], curStatu[pos]] = [curStatu[pos], curStatu[pos + 3]]; return pos + 3;
        case 'l': [curStatu[pos - 1], curStatu[pos]] = [curStatu[pos], curStatu[pos - 1]]; return pos - 1;
    }
}

function display(len) {
    for (let item of res) {
        for (let i = 0; i < item.length;) {
            process.stdout.write(item[i] + "  ");
            i++;
            if (i % 3 === 0) console.log('');
        }
        if (item === res[res.length - 1]) console.log('SUCCESS! TOTAL LENGTH IS ' + len + '.');
        else console.log('NEXT');
    }
}

function deepSearch(curStatu, pos, lastPos, len, maxLen) {
    if (!flag) {
        if (len >= maxLen) return;
        for (let direction of optionOfBlock[pos]) {
            let tempStatu = [...curStatu];
            let nextPos = exchange(pos, direction, tempStatu);
            if (nextPos === lastPos) continue;
            res.push(tempStatu);
            let nextLen = len + 1;
            // console.log(res)
            let success = successStatu.every((value, index) => value === tempStatu[index]);
            if (success) {
                display(nextLen);
                res.pop();
                flag = true;
                return;
            }
            deepSearch(tempStatu, nextPos, pos, nextLen, maxLen);
            res.pop();
        }
    }
}

const length = 6;
deepSearch(initStatu, initPos, initPos, 1, length);
if (!flag) stdout.write(`深度优先搜索步数在${length}内无结果。`);
exit();
