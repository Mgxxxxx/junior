const { time } = require('console');

const r = require('readline').createInterface({
    input: process.stdin,
    output: process.stdout
})

let count = 0;

// function HanNuo(n, a, b, c) {
//     count++;
//     if (n == 1) return;
//     // if (n == 1) console.log(`将第${n}个盘子从${a}移到${c}`);
//     else {
//         HanNuo(n - 1, a, c, b);
//         // console.log(`将第${n}个盘子从${a}移到${c}`);
//         HanNuo(n - 1, b, a, c);
//     }
// }

function HanNuo(n, a, b, c) {
    count++;
    if (n == 1) return;
    else {
        HanNuo(n - 1, a, c, b);
        HanNuo(n - 1, b, a, c);
    }
}

process.stdout.write('请输入盘子个数(输入0则退出):');
r.on('line', (num) => {
    if (num == 0) r.close();
    else {
        count = 0;
        console.time(`盘子个数为${num}时，花费时间`);
        for (let i = 0; i < 10000; i++) HanNuo(num, 'a', 'b', 'c');
        console.timeEnd(`盘子个数为${num}时，花费时间`);
        console.log(`递归调用次数为${count / 10000}`);
        process.stdout.write('请输入盘子个数(输入0则退出):');
    }
})
