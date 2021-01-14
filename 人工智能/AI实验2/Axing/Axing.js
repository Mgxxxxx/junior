const { stdout, exit } = require('process');

const optionOfBlock = [     //设置每个格子的方向
    ['r', 'd'], ['r', 'd', 'l'], ['d', 'l'],
    ['u', 'r', 'd'], ['u', 'r', 'd', 'l'], ['u', 'd', 'l'],
    ['u', 'r'], ['u', 'r', 'l'], ['u', 'l']
];
let successStatu = [1, 2, 3, 8, 0, 4, 7, 6, 5]; //最终的状态

class StatuNode {   //状态
    constructor(pre, level, statu) {
        this.pre = pre; //记录父节点
        this.level = level; //记录所在层
        this.statu = statu; //记录当前九宫格的状态
        this.pos = statu.findIndex(value => value === 0);   //记录0所在的位置
        let diffCount = 0;
        this.statu.forEach((value, pos) => {
            if (value !== successStatu[pos])
                diffCount++;
        });
        this.cost = this.level + diffCount; //当前结点的cost
    }
}
let initStatu = new StatuNode(-1, 1, [2, 8, 3, 1, 0, 4, 7, 6, 5]);  //设置初始状态
// let initStatu = new StatuNode(-1, 1, [1, 0, 3, 7, 2, 4, 6, 8, 5]);

let openList = []; //open表
let closeList = []; //close表
// let res = null;   //记录路径
let flag = false;   //是否成功的标志
let curIndex = 0;   //记录当前是第几个扩展的结点

function display(s) {    //打印结果
    if (s.pre !== -1)
        display(closeList[s.pre]);
    // step++;
    for (let i = 0; i < s.statu.length;) {
        process.stdout.write(s.statu[i] + "  ");
        i++;
        if (i % 3 === 0) console.log('');
    }
    console.log('');
}

function exchange(pos, direction, curStatu) {   //移动格子并更新状态数组
    switch (direction) {
        case 'u': [curStatu[pos - 3], curStatu[pos]] = [curStatu[pos], curStatu[pos - 3]]; return pos - 3;
        case 'r': [curStatu[pos + 1], curStatu[pos]] = [curStatu[pos], curStatu[pos + 1]]; return pos + 1;
        case 'd': [curStatu[pos + 3], curStatu[pos]] = [curStatu[pos], curStatu[pos + 3]]; return pos + 3;
        case 'l': [curStatu[pos - 1], curStatu[pos]] = [curStatu[pos], curStatu[pos - 1]]; return pos - 1;
    }
}

function findMinCost() {    //遍历open表，找出cost值最小的结点
    let min = Infinity;
    let index = -1;
    openList.forEach((item, curIndex) => {
        if (item.cost < min) {
            min = item.cost;
            index = curIndex;
        }
    })
    return openList.splice(index, 1)[0];
}

function informedSearch() {    //A*算法
    openList.push(initStatu);
    console.log('open表添加[' + initStatu.statu + ']结点');
    //lastPos记录当前扩展状态的上一个状态中0的位置，避免扩展时回到上一个状态
    //nextPos记录扩展后的状态中0的位置
    let lastPos, nextPos, expandNode, success;
    while (openList.length > 0) {
        expandNode = findMinCost(); //找到f值最小的结点
        console.log('open表取出[' + expandNode.statu + ']结点');
        closeList.push(expandNode);     //把取出的结点放入close表
        console.log('close表添加[' + expandNode.statu + ']结点');
        console.log('--------------------------------------------');
        success = successStatu.every((value, index) => value === expandNode.statu[index]);  //检测扩展的结点是否为最终状态
        if (success) {  //成功则展示退出
            console.log('解路径');
            display(expandNode);
            flag = true;
            return;
        };
        lastPos = closeList[expandNode.pre === -1 ? 0 : expandNode.pre].pos;
        for (let direction of optionOfBlock[expandNode.pos]) {  //开始扩展
            let tempStatu = [...(expandNode.statu)];
            nextPos = exchange(expandNode.pos, direction, tempStatu);
            if (nextPos === lastPos) continue;
            let tempNode = new StatuNode(curIndex, expandNode.level + 1, [...tempStatu]);
            openList.push(tempNode);    //加入open表
            console.log('open表添加[' + tempNode.statu + ']结点');
        }
        curIndex++;
    }
}


console.log('open表和close表初始为空');
try {
    informedSearch();
} catch (error) {
    if (error instanceof RangeError)
        console.log('在不超出栈容量的范围内，找不出解');
}
exit(0);