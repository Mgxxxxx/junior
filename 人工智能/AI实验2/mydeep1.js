const { stdout, exit } = require('process');

const optionOfBlock = [
    ['r', 'd'], ['r', 'd', 'l'], ['d', 'l'],
    ['u', 'r', 'd'], ['u', 'r', 'd', 'l'], ['u', 'd', 'l'],
    ['u', 'r'], ['u', 'r', 'l'], ['u', 'l']
];
let successStatu = [1, 2, 3, 8, 0, 4, 7, 6, 5];

class StatuNode {
    constructor(pre, statu) {
        this.pre = pre;
        this.statu = statu;
        this.pos = statu.findIndex(value => value === 0);
    }
}
let initStatu = new StatuNode(-1, [2, 8, 3, 1, 0, 4, 7, 6, 5]);
// let initStatu = new StatuNode(-1, [2, 8, 3, 1, 6, 4, 0, 7, 5]);

let openList = [initStatu];
let openListLevel = [1]
let closeList = [];
let res = [];
let maxLen = 6;
let flag = false;
let curIndex = 0;

function display() {
    for (let item of res) {
        for (let i = 0; i < item.statu.length;) {
            process.stdout.write(item.statu[i] + "  ");
            i++;
            if (i % 3 === 0) console.log('');
        }
        if (item === res[res.length - 1]) console.log('SUCCESS! TOTAL LENGTH IS ' + res.length + '.');
        else console.log('NEXT');
    }
}

function exchange(pos, direction, curStatu) {
    switch (direction) {
        case 'u': [curStatu[pos - 3], curStatu[pos]] = [curStatu[pos], curStatu[pos - 3]]; return pos - 3;
        case 'r': [curStatu[pos + 1], curStatu[pos]] = [curStatu[pos], curStatu[pos + 1]]; return pos + 1;
        case 'd': [curStatu[pos + 3], curStatu[pos]] = [curStatu[pos], curStatu[pos + 3]]; return pos + 3;
        case 'l': [curStatu[pos - 1], curStatu[pos]] = [curStatu[pos], curStatu[pos - 1]]; return pos - 1;
    }
}

function deepFirstSearch() {
    let lastPos, nextPos, expandNode, success, addCount;
    while (!flag && openList.length > 0) {
        //扩展点要在res最后一个元素的下一层
        if (res.length >= maxLen) {
            res.pop();
            for (let i = 0; i < addCount; i++) {
                openList.pop();
                openListLevel.pop();
            }
            while (openListLevel[openListLevel.length - 1] <= res.length)
                res.pop();
            continue;
        }
        expandNode = openList.pop();
        openListLevel.pop();
        closeList.push(expandNode);
        res.push(expandNode);
        success = successStatu.every((value, index) => value === expandNode.statu[index]);
        if (success) {
            display();
            flag = true;
            return;
        };
        lastPos = closeList[expandNode.pre === -1 ? 0 : expandNode.pre].pos;
        addCount = 0;
        for (let direction of optionOfBlock[expandNode.pos]) {
            let tempStatu = [...(expandNode.statu)];
            nextPos = exchange(expandNode.pos, direction, tempStatu);
            if (nextPos === lastPos) continue;
            let tempNode = new StatuNode(curIndex, [...tempStatu]);
            openList.push(tempNode);
            openListLevel.push(res.length + 1);
            addCount++;
        }
        curIndex++;
    }
}

deepFirstSearch();
if (!flag) stdout.write(`深度优先搜索步数在${maxLen}内无结果。`);

exit();