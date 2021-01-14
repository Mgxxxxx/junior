const { stdout, exit } = require('process');

const optionOfBlock = [     //设置每个格子的方向
    ['r', 'd'], ['r', 'd', 'l'], ['d', 'l'],
    ['u', 'r', 'd'], ['u', 'r', 'd', 'l'], ['u', 'd', 'l'],
    ['u', 'r'], ['u', 'r', 'l'], ['u', 'l']
];
let successStatu = [1, 2, 3, 8, 0, 4, 7, 6, 5]; //最终的状态

//初始状态
let initStatu = {
    pre: -1,    //记录父节点
    statu: [2, 8, 3, 1, 0, 4, 7, 6, 5]  //记录当前九宫格的状态
    // statu: [2, 5, 4, 3, 0, 7, 1, 8, 6]
};
let openList = []; //open表
let closeList = []; //close表
let curIndex = 0;   //记录当前是第几个扩展的结点
let flag = false;   //是否成功的标志
let step = 0;   //记录多少层

function display(s) {    //打印结果
    if (s.pre !== -1)
        display(closeList[s.pre]);
    step++;
    for (let i = 0; i < s.statu.length;) {
        process.stdout.write(s.statu[i] + "  ");
        i++;
        if (i % 3 === 0) console.log('');
    }
    console.log('');
}

function exchange(pos, direction, curStatu) {   //移动格子并更新状态数组
    switch (direction) {
        case 'u': [curStatu[pos - 3], curStatu[pos]] = [curStatu[pos], curStatu[pos - 3]];
            return pos - 3;
        case 'r': [curStatu[pos + 1], curStatu[pos]] = [curStatu[pos], curStatu[pos + 1]];
            return pos + 1;
        case 'd': [curStatu[pos + 3], curStatu[pos]] = [curStatu[pos], curStatu[pos + 3]];
            return pos + 3;
        case 'l': [curStatu[pos - 1], curStatu[pos]] = [curStatu[pos], curStatu[pos - 1]];
            return pos - 1;
    }
}

function breadthSearch(lastPos) {   //广度优先算法
    openList.push(initStatu);
    console.log('open表添加[' + initStatu.statu + ']结点');
    if (!flag) {
        if (openList.length !== 0) {
            let expandNode = openList.shift();    //取出open表的第一个结点
            console.log('open表取出[' + expandNode.statu + ']结点');
            closeList.push(expandNode);    //把取出的结点放入close表
            console.log('close表添加[' + expandNode.statu + ']结点');
            console.log('--------------------------------------------');
            let pos = expandNode.statu.findIndex(value => value === 0); //记录被扩展结点的0的位置
            for (let direction of optionOfBlock[pos]) {  //开始扩展
                let tempStatu = [...(closeList[closeList.length - 1].statu)];
                let nextPos = exchange(pos, direction, tempStatu);
                if (nextPos === lastPos) continue;
                let temp = { pre: curIndex, statu: [...tempStatu] };
                openList.push(temp);   //加入open表末端
                console.log('open表添加[' + temp.statu + ']结点');
                let success = successStatu.every((value, index) => value === tempStatu[index]);
                if (success) {  //成功则展示退出
                    console.log('解路径');
                    display(temp);
                    console.log(`总步数${step - 1}`);
                    flag = true;
                    return;
                }
            }
            curIndex++;
            breadthSearch(pos); //继续扩展
        } else {
            console.log('No Answer.')
        }
    }
}

let pos = initStatu.statu.findIndex(value => value === 0);

console.log('open表和close表初始为空');
try {
    breadthSearch(pos);
} catch (error) {
    if (error instanceof RangeError)
        console.log('在不超出栈容量的范围内，找不出解');
}
exit();