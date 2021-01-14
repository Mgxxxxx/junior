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

//特征集
const feature = [
    '有爪', '有犬齿', '目盯前方',
    '有奶', '有毛发', '吃肉',
    '有羽毛', '会飞', '生蛋',
    '反刍', '有蹄', '白色',
    '不会飞', '会游泳', '黑白色',
    '善飞', '不怕风浪', '长腿',
    '长脖子', '有暗斑', '有黑色条纹',
    '黄褐色', '有黑色斑点', '哺乳动物',
    '食肉动物', '有蹄动物', '偶蹄动物',
    '鸟', '老虎', '金钱豹',
    '长颈鹿', '斑马', '鸵鸟',
    '企鹅', '海燕'
];

//规则结构
class Rule {
    constructor(evis, res) {
        this.evis = evis;   //条件，数组，数组中的每个值为特征集中元素的下标，例如0对应‘有爪’
        this.res = res;     //条件可推出的结果，结果为数值，含义同上
    }
}

//规则集
const ruleBase = [
    new Rule([3], 23),
    new Rule([4], 23),
    new Rule([6], 27),
    new Rule([7, 8], 27),
    new Rule([23, 0, 1, 2], 24),
    new Rule([23, 5], 24),
    new Rule([23, 10], 25),
    new Rule([25, 9], 26),
    new Rule([24, 21, 20], 28),
    new Rule([24, 21, 22], 29),
    new Rule([25, 11, 20], 31),
    new Rule([27, 12, 17, 18, 14], 32),
    new Rule([27, 12, 13, 14], 33),
    new Rule([27, 15, 16], 34),
];
// const factBase = [4, 5, 21, 20];

//打印推导语句
function showSentence(evis, res) {
    for (let i = 0; i < evis.length; i++) {
        if (i + 1 === evis.length) stdout.write(feature[evis[i]] + ',推导出:');
        else stdout.write(feature[evis[i]] + ',');
    }
    console.log(feature[res]);
}

//通过下标转化为文字特征
function num2str(arr) {
    let s = '';
    arr.forEach(value => {
        // s += (value + feature[value] + ',');
        s += (feature[value] + ',');
    })
    return s.slice(0, -1);
}

//打印规则集
function printRules() {
    ruleBase.forEach(rule => {
        console.log(`[${num2str(rule.evis)}]可推导出${num2str([rule.res])}`);
    })
}

//正向推理
function forward(factBase) {
    console.log('以下是推导过程：');
    //规则集中的每个规则前件与事实库中的事实进行匹配
    const flag = ruleBase.some(rule => {
        //检查事实库中是否含有当前规则前件
        const produceAble = rule.evis.every(evi => {
            return (factBase.includes(evi))
        });
        if (produceAble) {
            //若匹配成功，则将该规则推导出的结论加入事实库
            factBase.push(rule.res);
            showSentence(rule.evis, rule.res);
            //若推出的结果已经是一种动物，则成功
            if (rule.res >= 28) {
                console.log('具有以上特征的动物为' + feature[rule.res]);
                return true;
            }
        }
        return false;
    });
    //若规则及中无匹配的则失败
    if (!flag) console.log('抱歉，这些事实推导不出是什么动物。');
}

//逆向推理
async function backward(factBase) {
    console.log('请选择您假设的是什么动物：');
    for (let i = 28; i < feature.length; i++) {
        stdout.write(i + feature[i] + '\t');
    }
    console.log();
    const animal = Number.parseInt(await readlineSync());
    // const animal = 28;
    console.log('以下是推导过程：');
    let assumes = [animal]; //建立当前假设集
    let branchs = new Array();  //创建假设集数组，存储推导过程中额外的假设集
    do {
        console.log(`当前假设集为[${num2str(assumes)}],取出${feature[assumes[0]]}假设`);
        const assume = assumes.shift();         //取出假设集中第一个假设
        //检查事实库中是否能证实当前假设
        const inFactBase = factBase.some(item => {
            if (item === assume) {
                console.log(`您选择的动物特征[${num2str(factBase)}]中满足${feature[assume]}假设`);
                return true;
            } else return false;
        })
        if (!inFactBase) {  //若事实库不能证实当前假设
            //新建一个知识集
            let konwledges = new Array();
            //遍历规则库，将能导出当前假设的规则下标放入知识集
            ruleBase.forEach((rule, index) => {
                if (rule.res === assume) konwledges.push(index);
            });
            if (konwledges.length >= 1) {
                //若知识集长度大于1，则将除第一个规则的前件以外的规则的前件放入假设集数组
                if (konwledges.length > 1) {
                    for (let i = 1; i < konwledges.length; i++) {
                        let temp = [...assumes];
                        temp.push(...ruleBase[konwledges[i]].evis);
                        console.log('知识库中[' + num2str(ruleBase[konwledges[i]].evis) + ']可推出' + feature[assume]);
                        branchs.push(temp);
                        console.log('假设集分支添加[' + num2str(temp) + ']');
                    }
                }
                //将第一个规则的前件放入假设集
                console.log('知识库中[' + num2str(ruleBase[konwledges[0]].evis) + ']可推出' + feature[assume]);
                assumes.push(...ruleBase[konwledges[0]].evis);
                console.log('假设集更新为[' + num2str(assumes) + ']');
            }
            else if (konwledges.length === 0) {
                //若规则集中没有能推导出当前假设
                console.log(`无法证实${feature[assume]}假设，推理失败。`);
                if (branchs.length > 0) {   //若有假设集数组中还有假设集，则取出第一个假设集作为新的假设集
                    assumes = branchs.shift();
                    console.log('将假设集分支中的[' + num2str(assumes) + ']取出，当前假设集替换为[' + num2str(assumes) + ']');
                } else {    //若有假设集数组中没有假设集，则推导失败
                    console.log(`假设集分支为空，无法继续进行推理。`);
                    exit();
                }
            }
        }
        console.log('-----------------------------------------------------------------------------------------------------------------------');
        if (assumes.length === 0) {     //若当前假设集为空，则表明所有假设都已证实，推导成功
            console.log(`当前所有假设集都已证实完毕，逆向推理成功！`);
            break;
        }
    } while (true);
}

async function main() {
    console.log('以下是可选择的动物特征：');
    for (let i = 0; i < 28; i++) {
        stdout.write(i + feature[i] + '\t');
        if ((i + 1) % 5 === 0) stdout.write('\n');
    }
    stdout.write('\n');
    console.log('请输入某种动物的特征，每个特征之间使用一个空格隔开：');
    const fea = await readlineSync();
    let factBase = fea.trim().split(' ').map(item => {
        const evi = Number.parseInt(item);
        if (evi >= 23) {
            console.log('抱歉，您输入的特征不在以上可选特征中。');
            exit();
        }
        return evi;
    });
    console.log('请选择你的推理方式：0.正向推理\t1.逆向推理');
    const way = Number.parseInt(await readlineSync());
    switch (way) {
        case 0: forward(factBase); break;
        case 1: await backward(factBase); break;
        default: console.log('输入非法！');
    }
}

main();

// printRules();
// r1.close();
