/*
TCP通信による複数人チャットツール(サーバ側)

・新たなユーザがログインするたびに, 新しいソケットを生成する。
ユーザ数が上限に達した場合, 警告文をサーバとユーザに表示し、新しいソケットが生成されないようにする。
・新しくログインしたユーザにはログインしている他のユーザの名前を送信する。
一方, 既にログインしているユーザには, 新たにログインがあったことを通知する。
・ユーザから"LOGOUT"メッセージを受け取ったときは, ログアウト処理を行い, 他のユーザに通知する。
・通常のメッセージが送られてきたときは, 全ユーザにメッセージを転送する。
・"SERVER_END"と入力すると, 全ユーザにソケットを閉じるように促し, ソケットを閉じてプログラムを終了する。
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#define MAX_USER 10 // 接続できるユーザ数の上限
#define PORT 50000 // ポート番号

// ユーザの情報を持つ構造体
struct user {
  char name[32]; // ユーザ名
  int login; // 1でログイン状態, 0でログアウト状態
  struct pollfd sockfd[1]; // pollで見る読み込みソケット
};

// エラー処理関数
void die(char *s) {
  perror(s);
  exit(1);
}

// 同じユーザ名がないかチェックする関数
int is_same_name(struct user *user, char *buff) {
  for (int i = 0;i < MAX_USER;++i) {
    if (user[i].login && (strcmp(buff, user[i].name) == 0)) return 1;
  }
  return 0;
}


int main() {
  struct user user[MAX_USER]; // ユーザを管理する配列
  struct pollfd fds[2]; // poll()で見るための読み込みソケット
  char buff[128], new_buff[128];	// 送受信用
  int err, nuser = 0; // err:システムコールの返り値用, nuser:ログイン中のユーザ数

  // ログイン状態を初期化
  for (int i = 0;i < MAX_USER;++i) {
    user[i].login = 0;
  }

  // TCPでソケットを作る
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) die("opening stream socket");

  // サーバデータ
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  err = bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (err == -1) die("bind");

  fds[0].fd = sock; // fdsにsockを登録
  fds[0].events = POLLIN; // reventsにPOLLINが返ってくるように設定
  fds[1].fd = 0; // fdsに標準入力を登録
  fds[1].events = POLLIN; // reventsにPOLLINが返ってくるように設定

  err = listen(fds[0].fd, 10);
  if (err == -1) die("listen");
  printf("このサーバーの最大接続人数は%dです\n", MAX_USER);

  while(1) {
    poll(fds, 2, 0);	//sockfdと標準入力を監視
    if (fds[0].revents == POLLIN  &&  nuser < MAX_USER) {
    // sockに通信要求がある場合入る。ユーザの人数が上限の場合は入らない。
      for (int i = 0;i < MAX_USER;++i) {
        if (!user[i].login) {
          // fdを空いている配列に保存
          err = user[i].sockfd[0].fd = accept(fds[0].fd, NULL, NULL);
          if (err == -1) die("accept");
          user[i].sockfd[0].events = POLLIN;

          // ユーザ名を受け取る
          err = recv(user[i].sockfd[0].fd, buff, 32, 0);
          if (err == -1) die("recv name");

          // 同じユーザ名がいたら聞き直す
          while (is_same_name(user, buff)) {
            sprintf(buff, "NAME_USED");
            err = send(user[i].sockfd[0].fd, buff, 128, 0);
            if (err == -1) die("send message");
            err = recv(user[i].sockfd[0].fd, buff, 32, 0);
            if (err == -1) die("recv name");
          }
          // ユーザ名の承認メッセージを送信
          sprintf(new_buff, "OK");
          err = send(user[i].sockfd[0].fd, new_buff, 128, 0);
          if (err == -1) die("send message");

          strcpy(user[i].name, buff); // nameにユーザ名を保存
          user[i].login = 1; // ログイン状態にする
          ++nuser; // 接続中のユーザ数を1人増やす
          sprintf(new_buff, "->%sさんがloginしました ＊残り接続可能数:%d\n", buff, MAX_USER-nuser);

          if (nuser == MAX_USER) { // 最大人数に達した時警告文をいれる
            sprintf(buff, "%s＊＊ユーザ人数が上限になりました＊＊\n", new_buff);
            strcpy(new_buff, buff);
          }
          for (int j = 0;j < MAX_USER;++j) { // 全ユーザに
            if (user[j].login) { // ログイン中の
              err = send(user[j].sockfd[0].fd, new_buff, 128, 0);
              if (err == -1) die("send message");
              if (i != j) { // ログインしたユーザに誰がいるかを知らせる
                sprintf(buff, "->%sさんがloginしています\n", user[j].name);
                err = send(user[i].sockfd[0].fd, buff, 128, 0);
                if (err == -1) die("send message");
              }
            }
          }
          printf("%s", new_buff);
          break;
        }
      }
    }

    for (int i = 0;i < MAX_USER;++i) {
      poll(user[i].sockfd, 1, 0);
      if (user[i].sockfd[0].revents == POLLIN && user[i].login) {
      // 通信要求がある場合に入る。ログイン状態でない場合は入らない。
        err = recv(user[i].sockfd[0].fd, buff, 128, 0);
        if (err == -1) die("recv message");

        if (strcmp(buff, "LOGOUT") == 0) { // "LOGOUT"が来た時
          err = send(user[i].sockfd[0].fd, buff, 128, 0);	// "LOGOUT"を送り返す
          if (err == -1) die("send logout");

          --nuser; // 接続中のユーザ数を1人減らす
          user[i].login = 0; // ログアウト状態にする
          sprintf(new_buff, "<-%sさんがlogoutしました ＊残り接続可能数:%d\n", user[i].name, MAX_USER-nuser);

        } else { // それ以外のコメント
          sprintf(new_buff, "%s: %s\n", user[i].name, buff);
        }

        for (int j = 0;j < MAX_USER;++j) { // 全ユーザに
          if (user[j].login) { // ログイン中の
            err = send(user[j].sockfd[0].fd, new_buff, 128, 0);
            if (err == -1) die("send message");
          }
        }
        printf("\033[%dD", 128); // カーソルを行の先頭に戻す
        printf("%s",new_buff);
      }
    }

    if (fds[1].revents == POLLIN) { // キーボードからの入力がある場合に入る
      scanf("%128s", buff);
      if (strcmp(buff, "SERVER_END") == 0) break;	//"SERVER_END"が入力されたら終了
      else printf("無効なコマンドです\n");
    }
  }


  for (int i = 0;i < MAX_USER;++i) {
    if (user[i].login) {
      err = send(user[i].sockfd[0].fd, "SERVER_END", 128, 0);
      if (err == -1) die("send server_end");
    }
  }

  printf("サーバーを終了します\n");
  close(sock);
}
