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

async function main() {
    let mc, contain;
    process.stdout.write("input m&c:");
    mc = await readlineSync();
    process.stdout.write("input contain:");
    contain = await readlineSync();

    let success = { m: mc, c: mc, p: 1 };   //m是牧师，c是野人，p是船的位置
    let arr = [];
    let res = [];
    arr.push({ m: 0, c: 0, p: 0 });

    function cross(x) {
        for (let i = 0; i < x; i++) //检测是否与之前的状态一样，一样则退出
            if (arr[i].m == arr[x].m && arr[i].c == arr[x].c && arr[i].p == arr[x].p) return;
        if (arr[x].m == success.m && arr[x].c == success.c && arr[x].p == success.p) {  //若达到最终状态则打印过河方法
            for (let i = 0; i < x + 1; i++) {
                process.stdout.write(`(${arr[i].m},${arr[i].c},${arr[i].p})->`);
            }
            process.stdout.write('success\n');
            // console.log(arr.slice(0, x + 1))
            res.push([...arr.slice(0, x + 1)])  //把过河方法压入结果数组
            return;
        }
        //若某一边牧师人数少于野人人数，则返回上一次状态
        if ((arr[x].m != 0 && arr[x].m < arr[x].c) || (((success.m - arr[x].m) != 0) && (success.m - arr[x].m < success.c - arr[x].c))) {
            // for (let i = 0; i < x + 1; i++) {
            //     console.log(`(${arr[i].m},${arr[i].c},${arr[i].p})->`);
            // }
            // console.log('fail');
            return;
        }

        //设置遍历范围，遍历每种过河的情况
        let _i = arr[x].p == 0 ? success.m - arr[x].m : arr[x].m;
        let _j = arr[x].p == 0 ? success.c - arr[x].c : arr[x].c;
        for (let i = 0; i <= _i; i++) {
            if (i <= contain) {
                for (let j = 0; j <= _j; j++) {
                    if (i + j > contain) break;
                    if (i + j < 1) continue;
                    else {
                        let temp = {};
                        if (arr[x].p == 0) {
                            temp.m = arr[x].m + i;
                            temp.c = arr[x].c + j;
                            temp.p = 1;
                        } else {
                            temp.m = arr[x].m - i;
                            temp.c = arr[x].c - j;
                            temp.p = 0;
                        }
                        arr[x + 1] = temp;
                        cross(x + 1);
                    }
                }
            }
        }
    }

    cross(0)

}

main();

// console.log(res)