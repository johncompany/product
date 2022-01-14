/*
2Aセメスターの授業「ソフトウエア1」で作成した、ターミナル上で動作する2048ゲームのコード

「遊び方」
4×4のマスに数字が書かれたタイルがあり、スライドさせるとそれらはマスの端まで移動し、同時に新たなタイルが出現する。
同じ数字のタイルがぶつかると2+2=4、4+4=8というように数字が足し合わされていく。
最終的に2048のタイルができればゲームクリアだが、それ以後も続けることはできる。
また、完全にタイルが動かせない状態になるとゲームオーバー。
[引用元] https://ja.wikipedia.org/wiki/2048_(%E3%82%B2%E3%83%BC%E3%83%A0)

以下のように、キーボード入力を受け取って移動する。
i: 上（up）　j: 左（left）　k: 右（right）　m: 下（down）
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
・特殊マスとして、「数が2倍になるマス（マゼンダ）」、「数が半分（2は変わらない）になるマス（シアン）」、
「数が消えるマス（白）」の3つを追加した
・それぞれのマスはランダムに出現し、真上に数が止まっている時に効果が発動する
--->効果を持つのは、出現した次のターンから
・発動した特殊マスは消える
*/

//ランダムに数字と特殊マスを発生させる
void random_board(int arr[4][4],int cell[4][4]) {
  //数字を発生させる
  int y = rand()%4,x = rand()%4;
  if (cell[y][x] == 0) {
    if (rand()%10 == 0) { //10%で'4'が発生
      cell[y][x] = 4;
    } else { //90%で'2'が発生
      cell[y][x] = 2;
    }
  } else {
    random_board(arr,cell);
  }

  //特殊マスを発生させる
  for (int y = 0;y < 4;++y) {
    for (int x = 0;x < 4;++x) {
      if (arr[y][x] == 0) {
        int r = rand()%3000;
        if (r < 10) {
          arr[y][x] = 1; //2倍マス
        } else if (r >= 10 && r < 20) {
          arr[y][x] = 2; //半分マス
        } else if (r >= 20 && r < 25) {
          arr[y][x] = 3; //zeroマス
        }
      }
    }
  }
}


//画面を表示する
void print_board(int arr[4][4],int cell[4][4]) {
  for (int y = 0;y < 4;++y) {
    for (int x = 0;x < 4;++x) {
      if (cell[y][x] == 0) {
        if (arr[y][x] == 1) {
          printf("\e[45m|\t\e[0m"); //2倍マス(マゼンダ)
        } else if (arr[y][x] == 2) {
          printf("\e[46m|\t\e[0m"); //半分マス(シアン)
        } else if (arr[y][x] == 3) {
          printf("\e[47m|\t\e[0m"); //zeroマス(白)
        } else {
          printf("|\t");
        }
      } else {
        if (arr[y][x] == 1) {
          printf("\e[45m|%d\t\e[0m",cell[y][x]); //2倍マス(マゼンダ)
        } else if (arr[y][x] == 2) {
          printf("\e[46m|%d\t\e[0m",cell[y][x]); //半分マス(シアン)
        } else if (arr[y][x] == 3) {
          printf("\e[47m|%d\t\e[0m",cell[y][x]); //zeroマス(白)
        } else {
          printf("|%d\t",cell[y][x]);
        }
      }
    }
    printf("|\r\n");
  }
}


//特殊マスによる効果を反映する
void special_cell(int arr[4][4],int cell[4][4]) {
  for (int y = 0;y < 4;++y) {
    for (int x = 0;x < 4;++x) {
      if (cell[y][x] != 0) {
        if (arr[y][x] == 1) {
          cell[y][x] *= 2; //2倍
          arr[y][x] = 0; //特殊マスが消える
        } else if (arr[y][x] == 2) {
          if (cell[y][x] == 2) {
            cell[y][x] = 2; //2は変わらない
          } else {
            cell[y][x] /= 2; //半分
          }
          arr[y][x] = 0; //特殊マスが消える
        } else if (arr[y][x] == 3) {
          cell[y][x] = 0; //zero
          arr[y][x] = 0; //特殊マスが消える
        }
      }
    }
  }
}


//上移動
void move_up(int cell[4][4]) {
  for (int x = 0;x < 4;++x) {
    int yy = 0; //どこまでスライドできるかを表す
    int flag = 0; //一つ前の操作を表す--->2倍の操作が連続するのを防ぐ
    for (int y = 0;y < 4;++y) {
      if (cell[y][x] != 0) {
        if (yy > 0 && cell[yy-1][x] == cell[y][x] && flag == 0) {
          //数字が合体
          cell[yy-1][x] *= 2;
          cell[y][x] = 0;
          flag = 1;
        } else {
          if (yy != y) {
            //yyまでスライド
            cell[yy][x] = cell[y][x];
            cell[y][x] = 0;
          }
          ++yy;
          flag = 0;
        }
      }
    }
  }
}


//下移動
void move_down(int cell[4][4]) {
  for (int x = 0;x < 4;++x) {
    int yy = 3; //どこまでスライドできるかを表す
    int flag = 0; //一つ前の操作を表す--->2倍の操作が連続するのを防ぐ
    for (int y = 3;y > -1;--y) {
      if (cell[y][x] != 0) {
        if (yy < 3 && cell[yy+1][x] == cell[y][x] && flag == 0) {
          //数字が合体
          cell[yy+1][x] *= 2;
          cell[y][x] = 0;
          flag = 1;
        } else {
          if (yy != y) {
            //yyまでスライド
            cell[yy][x] = cell[y][x];
            cell[y][x] = 0;
          }
          --yy;
          flag = 0;
        }
      }
    }
  }
}


//左移動
void move_left(int cell[4][4]) {
  for (int y = 0;y < 4;++y) {
    int xx = 0; //どこまでスライドできるかを表す
    int flag = 0; //一つ前の操作を表す--->2倍の操作が連続するのを防ぐ
    for (int x = 0;x < 4;++x) {
      if (cell[y][x] != 0) {
        if (xx > 0 && cell[y][xx-1] == cell[y][x] && flag == 0) {
          //数字が合体
          cell[y][xx-1] *= 2;
          cell[y][x] = 0;
          flag = 1;
        } else {
          if (xx != x) {
            //xxまでスライド
            cell[y][xx] = cell[y][x];
            cell[y][x] = 0;
          }
          ++xx;
          flag = 0;
        }
      }
    }
  }
}


//右移動
void move_right(int cell[4][4]) {
  for (int y = 0;y < 4;++y) {
    int xx = 3; //どこまでスライドできるかを表す
    int flag = 0; //一つ前の操作を表す--->2倍の操作が連続するのを防ぐ
    for (int x = 3;x > -1;--x) {
      if (cell[y][x] != 0) {
        if (xx < 3 && cell[y][xx+1] == cell[y][x] && flag == 0) {
          //数字が合体
          cell[y][xx+1] *= 2;
          cell[y][x] = 0;
          flag = 1;
        } else {
          if (xx != x) {
            //xxまでスライド
            cell[y][xx] = cell[y][x];
            cell[y][x] = 0;
          }
          --xx;
          flag = 0;
        }
      }
    }
  }
}


//2つの画面が同じかを判定
int is_sameboard(int cell1[4][4],int cell2[4][4]) {
  for (int y = 0;y < 4;++y) {
    for (int x = 0;x < 4;++x) {
      if (cell1[y][x] != cell2[y][x]) {
        return 0;
      }
    }
  }
  return 1;
}


//クリア、ゲームオーバーを判定
int is_gameend(int cell[4][4]) {
  int max = 0;
  for (int y = 0;y < 4;++y) {
    for (int x = 0;x < 4;++x) {
      if (cell[y][x] == 0) {
        return 0;
      } else if (x < 3 && cell[y][x] == cell[y][x+1]) {
        return 0;
      } else if (y < 3 && cell[y][x] == cell[y+1][x]) {
        return 0;
      } else if (max < cell[y][x]) {
        max = cell[y][x];
      }
    }
  }
  if (max >= 2048) { //2048以上の数字があればクリア
    return 1;
  } else { //なければゲームオーバー
    return 2;
  }
}


int main (int argc, char *argv[]) {
    srand(time(NULL));
    printf("Press any key to start\n");
    int c;
    int flag = 0;

    int arr[4][4]; //特殊マス用配列
    int cell[4][4];
    for (int y = 0;y < 4;++y) {
      for (int x = 0;x < 4;++x) {
        arr[y][x] = 0;
        cell[y][x] = 0;
      }
    }

    system("/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる

    while((c = getchar()) != '.') {
        system("clear");
        printf("Press '.' to close\r\n");
        printf("i:up  j:left  k:right  m:down\r\n\r\n");
        printf("\e[45m    \e[0m");
        printf(":Twice zone ");
        printf("\e[46m    \e[0m");
        printf(":Half zone ");
        printf("\e[47m    \e[0m");
        printf(":Zero zone\r\n\r\n");
        //移動前の画面
        int now_cell[4][4];
        for (int y = 0;y < 4;++y) {
          for (int x = 0;x < 4;++x) {
            now_cell[y][x] = cell[y][x];
          }
        }

        if (c == 'i') {
          move_up(cell);
        } else if (c == 'm') {
          move_down(cell);
        } else if (c == 'j') {
          move_left(cell);
        } else if (c == 'k') {
          move_right(cell);
        }

        //移動したときだけ
        if (!is_sameboard(now_cell,cell)) {
          special_cell(arr,cell); //特殊マスによる効果を発動させる
          random_board(arr,cell); //数字と特殊マスを発生させる
        }
        //初期配置
        if (flag == 0) {
          random_board(arr,cell);
          ++flag;
        }

        print_board(arr,cell);
        printf("You pressed '%c'\r\n", c);

        if (is_gameend(cell) == 1) {
          printf("Game Clear!\r\n");
          break;
        } else if (is_gameend(cell) == 2) {
          printf("Game Over!\r\n");
          break;
        }
    }

    system("/bin/stty cooked");  // 後始末

    return 0;
}
