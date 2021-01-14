const { stdout, exit } = require('process');

const terminators = ['i', '+', '*', '(', ')', '#'];
const nonTerminators = ['E', 'X', 'T', 'Y', 'F'];

class Pro { //产生式
    constructor(condition, conclusion) {
        this.condition = condition; //产生式左部
        this.conclusion = conclusion;   //产生式右部
    }
}

const analyzeTable = [  //初始化分析表
    [new Pro('E', 'TX'), null, null, new Pro('E', 'TX'), null, null],
    [null, new Pro('X', '+TX'), null, null, new Pro('X', 'ε'), new Pro('X', 'ε')],
    [new Pro('T', 'FY'), null, null, new Pro('T', 'FY'), null, null],
    [null, new Pro('Y', 'ε'), new Pro('Y', '*FY'), null, new Pro('Y', 'ε'), new Pro('Y', 'ε')],
    [new Pro('F', 'i'), null, null, new Pro('F', '(E)'), null, null]
]

//打印LL(1)分析表
console.log('\ti\t+\t*\t(\t)\t#');
for (let i = 0; i < analyzeTable.length; i++) {
    stdout.write(`${nonTerminators[i]}\t`);
    for (let j = 0; j < analyzeTable[i].length; j++) {
        if (analyzeTable[i][j]) stdout.write(analyzeTable[i][j].condition + '->' + analyzeTable[i][j].conclusion + '\t');
        else stdout.write('null\t');
    }
    stdout.write('\n');
}
console.log('');

const input = 'i+i*i#'; //设置输入串

const stack = new Array();  //创建分析栈，并压入'#'
stack.push('#');

stack.push('E');//压入输入开始符
let pos = 0;    //指向当前输入符号
while (pos < input.length && stack.length) {    //开始分析
    stdout.write(`Stack:${stack.toString().padEnd(3, ' ')}\tInput:${input.slice(pos).padStart(input.length, ' ')}\t`);
    const c = stack.pop();
    if (c === input[pos]) { //若栈顶元素和当前输入符号一致
        if (c === '#') {    //若栈顶元素为'#'，则表示输入串为当前文法的句子
            console.log('\nsuccessful');
            break;
        } else { pos++; console.log(); continue; }  //当前输入符号后移一位
    } else if (nonTerminators.indexOf(c) !== -1) {
        const x = nonTerminators.indexOf(c), y = terminators.indexOf(input[pos]);
        const production = analyzeTable[x][y];  //在分析表中找到对应的产生式
        if (production === null) {    //分析表中无对应的产生式，则输入串不是当前文法的句子
            console.log(`\nError, there is no prodution. The top of stack is ${c}, current char is ${input[pos]}.`);
            break;
        }
        stdout.write(`Use production:${production.condition}->${production.conclusion}\n`)
        if (production.conclusion !== 'ε') {    //若产生式右部不为空则把左部的字符逆序压入分析栈
            for (let i = production.conclusion.length - 1; i >= 0; i--)
                stack.push(production.conclusion[i]);
        }
    } else {    //当前字符为非法字符
        console.log(`\nError, can not match this char. The top of stack is ${c}, current char is ${input[pos]}.`);
        break;
    }
}

exit();