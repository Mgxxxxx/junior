const fs = require("fs");
const data = fs.readFileSync("a.txt");  //读取源码文件
class WD {                      //关键词类
    constructor(chara, value) {
        this.chara = chara;     //关键//关键词字符串
        this.value = value;     //关键词的编码
    }
}
class Reader {
    constructor(text) {
        this.text = text;           //保存要分析的文本
        this.pos = -1;              //初始读取的字符串位置为-1
        this.length = text.length;  //需要分析的文本的字符串长度
        this.flag = true;           //是否能继续分析的标志
        this.reserveList = [        //关键词表
            new WD('if', 0), new WD('then', 1), new WD('else', 2), new WD('while', 3), new WD('begin', 4),
            new WD('do', 5), new WD('end', 6), new WD('and', 39), new WD('or', 40), new WD('not', 41),
        ]
    }
    getChar() {                     //读取一个字符
        this.pos++;
        if (this.pos >= this.length) {
            // console.log('Reached end.');
            return;
        }
        return this.text[this.pos];
    }
    retract() {                     //回退一个字符
        this.pos--;
    }
    letter(c) {                     //判断是否为字母
        return /^[a-zA-Z]$/.test(c);
    }
    digit(c) {                      //判断是否为数字
        return /^[0-9]$/.test(c);
    }
    rop(c) {                        //判断是否为关系运算符
        if (c === '>' || c === '<' || c === '=')
            return true;
    }
    reserve(s) {                    //判断是否为关键字
        for (let i = 0; i < 7; i++) {
            if (s === this.reserveList[i].chara)
                return this.reserveList[i].value;
        }
        return 0;
    }
    judge1(c) {                     //c是字母的时候进行的判断
        let s = '';
        while (this.letter(c) || this.digit(c)) {
            s += c;
            c = this.getChar();
        }
        this.retract();
        if (this.reserve(s) === 0) return `(15,${s})`;
        else return `(${s},${this.reserve(s)})`;
    }
    judge2(c) {                     //c是数字的时候进行的判断
        let s = '';
        while (this.digit(c)) {
            s += c;
            c = this.getChar();
        }
        this.retract();
        return `(57,${s})`;
    }
    judge3(c) {                     //c是:的时候进行的判断
        let s = '';
        s += c;
        c = this.getChar();
        if (c === '=') {
            s += c;
            return `(38,${s})`;
        }
    }
    judge4(c) {                     //c是关系运算符进行的判断
        let s = '';
        while (c === '>' || c === '<' || c === '=') {
            s += c;
            c = this.getChar();
        }
        this.retract();
        const op_rop = ['<', '>', '=', '<=', '>=', '<>'];
        if (op_rop.some(item => item === s))
            return `(42,${s})`;
    }
    judge5(c) {                      //c是#进行的判断
        let t = this.getChar();
        if (t === '~') {
            this.flag = false;
            return 'analyze done.';
        }else return 'can not distinguish ${c+t}';
    }
    analyze() {
        while (this.pos <= this.length && this.flag) {
            let c = this.getChar();
            if (this.letter(c)) console.log(this.judge1(c));
            else if (this.digit(c)) console.log(this.judge2(c));
            else if (c === ':') console.log(this.judge3(c));
            else if (this.rop(c)) console.log(this.judge4(c));
            else if (c === '#') console.log(this.judge5(c));
            else if (c === ';') console.log('(8,;)');
            else if (c === '+') console.log('(34,+)');
            else if (c === '*') console.log('(36,*)');
            else if (c === '(') console.log('(48,()');
            else if (c === ')') console.log(('(49,))'));
            else if (!/[:#;+*()\s]/.test(c)) {          //若是非法字符，则直接退出
                console.log(`Error, '${c}' is illegal.`);
                break;
            }
        }
    }
}
const r = new Reader(data.toString());
try {
    r.analyze();
} catch (error) {
    console.log(error);
}
