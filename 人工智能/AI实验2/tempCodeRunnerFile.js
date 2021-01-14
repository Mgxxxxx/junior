let initPos = initStatu.findIndex(value => value === 0);
// let openList = [initStatu];
// let openListLevel = [1]
// let closeList = [];
// let res = [];
// let maxLen = 5;
// let flag = false;

// function display(len) {
//     for (let item of res) {
//         for (let i = 0; i < item.length;) {
//             process.stdout.write(item[i] + "  ");
//             i++;
//             if (i % 3 === 0) console.log('');
//         }
//         if (item === res[res.length - 1]) console.log('SUCCESS! TOTAL LENGTH IS ' + len + '.');
//         else console.log('NEXT');
//     }
// }

// function exchange(pos, direction, curStatu) {
//     switch (direction) {
//         case 'u': [curStatu[pos - 3], curStatu[pos]] = [curStatu[pos], curStatu[pos - 3]]; return pos - 3;
//         case 'r': [curStatu[pos + 1], curStatu[pos]] = [curStatu[pos], curStatu[pos + 1]]; return pos + 1;
//         case 'd': [curStatu[pos + 3], curStatu[pos]] = [curStatu[pos], curStatu[pos + 3]]; return pos + 3;
//         case 'l': [curStatu[pos - 1], curStatu[pos]] = [curStatu[pos], curStatu[pos - 1]]; return pos - 1;
//     }
// }

// function deepFirstSearch(initPos) {
//     let pos = initPos, lastPos, nextPos, expandNode, success, addCount;
//     while (!flag && openList.length > 0) {
//         //扩展点要在res最后一个元素的下一层
//         if (res.length >= maxLen) {
//             res.pop();
//             for (let i = 0; i < addCount; i++) {
//                 openList.pop();
//                 openListLevel.pop();
//             }
//             while (openListLevel[openListLevel.length - 1] <= res.length)
//                 res.pop();
//             continue;
//         }
//         expandNode = openList.pop();
//         closeList.push(expandNode);
//         res.push(expandNode);
//         success = successStatu.every((value, index) => value === expandNode[index]);
//         if (success) {
//             // display(nextLen);
//             console.log("success");
//             flag = true;
//             return;
//         };
//         console.log(res);
//         lastPos = res.length >= 2 ? res[res.length - 2].findIndex(value => value === 0) : initPos;
//         let curPos = pos;
//         addCount = 0;
//         for (let direction of optionOfBlock[curPos]) {
//             let tempStatu = [...expandNode];
//             nextPos = exchange(curPos, direction, tempStatu);
//             if (nextPos === lastPos) continue;
//             pos = nextPos;
//             openList.push(tempStatu);
//             openListLevel.push(res.length + 1);
//             addCount++;
//         }
//     }

// }