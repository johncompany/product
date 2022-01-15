/*
「ソフトウエアI」で作成した、ターミナル上で動作する蛇のゲームのコード

「遊び方」
5×5のマスがあり、o: 蛇　x: 障害物　1: ポイントマス とする。
障害物を避けながら蛇を移動させ、ポイントマスを通過すると体が伸びていく。
自分の体に当たったらゲームオーバー。

以下のように、キーボード入力を受け取って移動する。
i: 上（up）　j: 左（left）　k: 右（right）　m: 下（down）
*/


#include <stdio.h>
#include <stdlib.h>

//位置を表す配列を後ろにずらす
int move(int a[],int b[],int n) {
  for (int i = n-1;i > 0;--i) {
    a[i] = a[i-1];
    b[i] = b[i-1];
  }
  return 0;
}

//胴体にぶつかるような180°転換の場合1を返す
int overlap(int a[],int b[],int bodysize) {
  if (bodysize > 1 && a[0] == a[1] && b[0] == b[1]) {
    return 1;
  } else {
    return 0;
  }
}

int main() {
    int x[4] = {0};
    int y[4] = {0};
    int point = 0;  // !!!!! ここでpointを宣言
    int flag = 0;

    char board[5][6] = {
        "-1---",
        "---x-",
        "-x---",
        "---1-",
        "x-1x-"
    };

    for(int c = 0; c != EOF; c = getchar()) {
        // キーボード入力の処理
        if (c == '\n') {
            continue;
        } else if (c == 'k' && x[0] < 4 && board[y[0]][x[0]+1] != 'x') {
          ++x[0];
          if (overlap(x,y,point+1-flag) || board[y[0]][x[0]] == 'o') {
            printf("Game Over\n");
            return 0;
          } else {
            --x[0];
            move(x,y,4);
            ++x[0];
          }
        } else if (c == 'i' && y[0] > 0 && board[y[0]-1][x[0]] != 'x') {
          --y[0];
          if (overlap(x,y,point+1-flag) || board[y[0]][x[0]] == 'o') {
            printf("Game Over\n");
            return 0;
          } else {
            ++y[0];
            move(x,y,4);
            --y[0];
          }
        } else if (c == 'j' && x[0] > 0 && board[y[0]][x[0]-1] != 'x') {
          --x[0];
          if (overlap(x,y,point+1-flag) || board[y[0]][x[0]] == 'o') {
            printf("Game Over\n");
            return 0;
          } else {
            ++x[0];
            move(x,y,4);
            --x[0];
          }
        } else if (c == 'm' && y[0] < 4 && board[y[0]+1][x[0]] != 'x') {
          ++y[0];
          if (overlap(x,y,point+1-flag) || board[y[0]][x[0]] == 'o') {
            printf("Game Over\n");
            return 0;
          } else {
            --y[0];
            move(x,y,4);
            ++y[0];
          }
        }

        system("clear");  // これは画面を綺麗にする関数です

        flag = 0;

        // ポイントマスの記憶
        if (board[y[0]][x[0]] == '1') {
          flag = 1;
        }

        // 自分の体の位置の描画
        for (int i = 0;i <= point;++i) {
          board[y[i]][x[i]] = 'o';
        }

        // 得点の加算
        if (flag == 1) {
          ++point;
        }

        // ボードの表示
        for (int n = 0; n < 5; ++n) {
           printf("%s\n", board[n]);
        }

        // 得点の表示
        printf("point: %d\n", point);  // !!!! ここでpointを表示

        // 自分の尻尾の位置を背景に戻す
        board[y[point]][x[point]] = '-';
      }
    return 0;
}
