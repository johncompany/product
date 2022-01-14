/*
2Aセメスターの授業「ソフトウエア2」で作成した、ターミナル上で動作するライフゲームのコード

「基本的なルール」
2次元グリッド上にセルが存在し、世代が進むにつれてセルの配置を変化させる
・生きているセルが有る場所
-> 周囲に生きているセルが2個または3個存在するならばそのまま
-> そうでなければセルは消滅
・生きているセルが無い場所
-> 周囲に生きているセルがちょうど3個存在するならばセルが誕生
-> そうでなければそのまま

ただし、拡張として以下に示すゾンビセルや爆弾投下などの要素を追加した。
初期配置はRLEフォーマットのファイル、またはランダムで与えられる。
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
#include <time.h>

/*
・10%で生きたセル('#',1)、1%でゾンビセル('*',2)となるように初期配置する
・生きたセルはゾンビセルに隣接すると、ゾンビセルへと変わる
・ゾンビセルは障害物がないとき、上下左右ランダムに1マスずつ進む
・ゾンビセルの周りに3つ以上他のゾンビセルが集まると、ここに爆弾('!',3)が落とされる
--->9マスのセルが消滅し、通行不可('x',4)になる
*/

//ファイルを与えられないときの初期配置
//生きたセル(1)10% ゾンビセル(2)1%
void init_cells(const int height, const int width, int cell[height][width], FILE* fp) {
  if (fp == NULL) {
    for (int y = 0;y < height;++y) {
      for (int x = 0;x < width;++x) {
        if (rand()%10 == 0) {
          cell[y][x] = 1;
        } else if (rand()%100 == 0) {
          cell[y][x] = 2;
        }
      }
    }
  } else {
    int c,x,y;
    while ((c = fgetc(fp)) != '\n');
    while (fscanf(fp,"%d %d",&x,&y) != EOF) {
      cell[y][x] = 1;
    }
  }
}

void print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]) {
  //存在するセルの比率
  double cnt_living = 0.0,cnt_undead = 0.0;
  for (int y = 0;y < height;++y) {
    for (int x = 0;x < width;++x) {
      if (cell[y][x] == 1) {
        cnt_living++;
      } else if (cell[y][x] == 2) {
        cnt_undead++;
      }
    }
  }
  fprintf(fp,"generation = %d\n",gen);
  fprintf(fp,"survivor rate = %f%%\n",cnt_living/height/width*100);
  fprintf(fp,"undead rate = %f%%\n",cnt_undead/height/width*100);
  fprintf(fp,"\e[34m#\e[0m:Survivor \e[32m*\e[0m:Undead \e[31m!\e[0m:Bomb \e[31mx\e[0m:Impassable\n");

  //上の壁
  fprintf(fp,"+");
  for (int x = 0;x < width;++x)
    fprintf(fp, "-");
  fprintf(fp, "+\n");

  /* 外壁と 内側のゲーム部分 */
  for (int y = 0;y < height;++y) {
    fprintf(fp,"|");
    for (int x = 0;x < width;++x) {
      // ANSIエスケープコードを用いて、赤い"#" を表示
      // \e[31m で 赤色に変更
      // \e[0m でリセット（リセットしないと以降も赤くなる）
      if (cell[y][x] == 1) { //生きたセル(青)
        fprintf(fp, "\e[34m#\e[0m");
      } else if (cell[y][x] == 2) { //ゾンビセル(緑)
        fprintf(fp, "\e[32m*\e[0m");
      } else if (cell[y][x] == 3) { //爆弾(赤)
        fprintf(fp, "\e[31m!\e[0m");
      } else if (cell[y][x] == 4) { //通行不可(赤)
        fprintf(fp, "\e[31mx\e[0m");
      } else {
        fprintf(fp, " ");
      }
    }
    fprintf(fp,"|\n");
  }

  // 下の壁
  fprintf(fp, "+");
  for (int x = 0;x < width;++x)
    fprintf(fp, "-");
  fprintf(fp, "+\n");

  fflush(fp); // バッファされている文字列を出力する
}

int count_adjacent_cells(int h, int w, const int height, const int width, int cell[height][width]) {
  int cnt_living = 0,cnt_undead = 0;
  //上段
  if (h > 0) {
    if (w > 0) {
      if (cell[h-1][w-1] == 1) {
        cnt_living++;
      } else if (cell[h-1][w-1] == 2) {
        cnt_undead++;
      }
    }
    if (cell[h-1][w] == 1) {
      cnt_living++;
    }
    if (cell[h-1][w] == 2) {
      cnt_undead++;
    }
    if (w < width-1) {
      if (cell[h-1][w+1] == 1) {
        cnt_living++;
      } else if (cell[h-1][w+1] == 2) {
        cnt_undead++;
      }
    }
  }
  //中段
  if (w > 0) {
    if (cell[h][w-1] == 1) {
      cnt_living++;
    } else if (cell[h][w-1] == 2) {
      cnt_undead++;
    }
  }
  if (w < width-1) {
    if (cell[h][w+1] == 1) {
      cnt_living++;
    } else if (cell[h][w+1] == 2) {
      cnt_undead++;
    }
  }
  //下段
  if (h < height-1) {
    if (w > 0) {
      if (cell[h+1][w-1] == 1) {
        cnt_living++;
      } else if (cell[h+1][w-1] == 2) {
        cnt_undead++;
      }
    }
    if (cell[h+1][w] == 1) {
      cnt_living++;
    }
    if (cell[h+1][w] == 2) {
      cnt_undead++;
    }
    if (w < width-1) {
      if (cell[h+1][w+1] == 1) {
        cnt_living++;
      } else if (cell[h+1][w+1] == 2) {
        cnt_undead++;
      }
    }
  }
  //ゾンビセルがいないとき、生きたセルの数を返す
  if (cnt_undead == 0) {
    return cnt_living;
  //ゾンビセルがいるとき、ゾンビセルの数*(-1)を返す
  } else {
    return -cnt_undead;
  }
}

void update_cells(const int height, const int width, int cell[height][width]) {
  int next_cell[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      next_cell[y][x] = 0;
    }
  }
  for (int y = 0;y < height;++y) {
    for (int x = 0;x < width;++x) {
      int cnt = count_adjacent_cells(y,x,height,width,cell);
      if (next_cell[y][x] != 0) {
        continue;
      }
      else if (cell[y][x] == 1) {
        if (cnt == 2 || cnt == 3) {
          next_cell[y][x] = 1;
        } else if (cnt < 0) {
          next_cell[y][x] = 2; //ゾンビ化
        } else {
          next_cell[y][x] = 0;
        }
      } else if (cell[y][x] == 2) {
        if (cnt < -2) {
          //爆弾を落とす
          next_cell[y][x] = 3;
          if (y > 0) {
            if (x > 0) {
              next_cell[y-1][x-1] = 3;
            }
            next_cell[y-1][x] = 3;
            if (x < width-1) {
              next_cell[y-1][x+1] = 3;
            }
          }
          if (x > 0) {
            next_cell[y][x-1] = 3;
          }
          if (x < width-1) {
            next_cell[y][x+1] = 3;
          }
          if (y < height-1) {
            if (x > 0) {
              next_cell[y+1][x-1] = 3;
            }
            next_cell[y+1][x] = 3;
            if (x < width-1) {
              next_cell[y+1][x+1] = 3;
            }
          }
        } else {
          //障害物がないとき、上下左右ランダムに1マスずつ進む
          next_cell[y][x] = 0;
          switch (rand()%4) {
            case 0:
            if (y > 0 && cell[y-1][x] == 0) {
              next_cell[y-1][x] = 2;
            } else {
              next_cell[y][x] = 2;
            }
            break;
            case 1:
            if (y < height-1 && cell[y+1][x] == 0) {
              next_cell[y+1][x] = 2;
            } else {
              next_cell[y][x] = 2;
            }
            break;
            case 2:
            if (x > 0 && cell[y][x-1] == 0) {
              next_cell[y][x-1] = 2;
            } else {
              next_cell[y][x] = 2;
            }
            break;
            case 3:
            if (x < width-1 && cell[y][x+1] == 0) {
              next_cell[y][x+1] = 2;
            } else {
              next_cell[y][x] = 2;
            }
            break;
          }
        }
      } else if (cell[y][x] == 3) {
        //爆弾(3)--->通行不可(4)
        next_cell[y][x] = 4;
      } else if (cell[y][x] == 4) {
        next_cell[y][x] = 4;
      } else {
        if (cnt == 3) {
          next_cell[y][x] = 1;
        } else {
          next_cell[y][x] = 0;
        }
      }
    }
  }

  for (int y = 0;y < height;++y) {
    for (int x = 0;x < width;++x) {
      cell[y][x] = next_cell[y][x];
    }
  }
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  FILE *fp = stdout;
  const int height = 40;
  const int width = 70;

  int cell[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell[y][x] = 0;
    }
  }

  /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
  if ( argc > 2 ) {
    fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
    return EXIT_FAILURE;
  }
  else if (argc == 2) {
    FILE *lgfile;
    if ( (lgfile = fopen(argv[1],"r")) != NULL ) {
      init_cells(height,width,cell,lgfile); // ファイルによる初期化
    }
    else{
      fprintf(stderr,"cannot open file %s\n",argv[1]);
      return EXIT_FAILURE;
    }
    fclose(lgfile);
  }
  else{
    init_cells(height, width, cell, NULL); // デフォルトの初期値を使う
  }

  print_cells(fp, 0, height, width, cell); // 表示する
  sleep(1); // 1秒休止

  /* 世代を進める*/
  for (int gen = 1 ;; gen++) {
    update_cells(height, width, cell); // セルを更新
    print_cells(fp, gen, height, width, cell);  // 表示する
    sleep(1); //1秒休止する
    fprintf(fp,"\e[%dA",height+6);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
  }

  return EXIT_SUCCESS;
}
