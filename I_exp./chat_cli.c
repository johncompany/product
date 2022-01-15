/*
TCP通信による複数人チャットツール(クライアント側)

・起動すると名前の入力を促し, 入力した名前でサーバに登録される。
既に他のユーザにより使われている名前は登録できない。
・ログイン時に, 既にログインしているユーザの名前を出力する。
・メッセージを入力すると, 全ユーザに自分の名前とメッセージが表示される。
自分の送ったメッセージだけは赤文字で出力される。
・メッセージを受け付けると, 送信者とそのメッセージを出力する。
・"LOGOUT"と入力すると, サーバ側でログアウト処理が行われ, ソケットを閉じてプログラムを終了することができる。
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
#define PORT 50000

// エラー処理用関数
void die(char *s) {
  perror(s);
  exit(1);
}


int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s ip_address\n", argv[0]);
    exit(1);
  }

  struct pollfd fds[2]; // poll()で見るための読み込みソケット
  char buff[128], name_buff[128]; // 送受信用
  int err; // システムコールの返り値用

  // TCPでソケットを作る
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) die("opening stream socket");

  // サーバデータ
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(PORT);

  printf("ユーザ名を入力:");
  fgets(name_buff, 32, stdin);
  if (name_buff[strlen(name_buff)-1] == '\n') {
    name_buff[strlen(name_buff)-1] = '\0'; // 改行文字の削除
  }
  printf("接続中...\n");
  err = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (err == -1) die("connecting stream socket");

  fds[0].fd = sock; // fdsにsockを登録
  fds[0].events = POLLIN; // reventsにPOLLINが返ってくるように設定
  fds[1].fd = 0; // fdsに標準入力を登録
  fds[1].events = POLLIN; // reventsにPOLLINが返ってくるように設定

  err = send(fds[0].fd, name_buff, 32, 0); // ユーザ名の送信
  if (err == -1) die("send name");
  err = recv(fds[0].fd, buff, 128, 0); // 承認メッセージの受信
  if (err == -1) die("receive message");

  // 承認されなかったら再入力
  while (strcmp(buff, "NAME_USED") == 0) {
    printf("このユーザ名はすでに使われています\nユーザ名を入力:");
    fgets(name_buff, 32, stdin);
    if (name_buff[strlen(name_buff)-1] == '\n') name_buff[strlen(name_buff)-1] = '\0';
    printf("接続中...\n");
    err = send(fds[0].fd, name_buff, 32, 0);
    if (err == -1) die("send name");
    err = recv(fds[0].fd, buff, 128, 0);
    if (err == -1) die("receive message");
  }

  while(1) {
    poll(fds, 2, 0); // sockfdと標準入力を監視
    if (fds[1].revents == POLLIN) { // キーボードからの入力がある場合に入る
      fgets(buff, 128, stdin);
      if (buff[strlen(buff)-1] == '\n') buff[strlen(buff)-1] = '\0';
      if (strlen(buff) > 0) { // 1文字以上入力されていたらデータを送信
        printf("\e[A\e[K");
        err = send(fds[0].fd, buff, 128, 0);
        if (err == -1) die("send message");
      }
    } else if (fds[0].revents == POLLIN) { // サーバからの通信要求がある場合に入る
      err = recv(fds[0].fd, buff, 128, 0);
      if (err == -1) die("recv message");

      if (strcmp(buff, "LOGOUT") == 0) {
        printf("logoutします\n");
        break;
      } else if (strcmp(buff, "SERVER_END") == 0) {
        printf("サーバが終了しました\n");
        break;
      } else {
        printf("\033[%dD", 128); // カーソルを行の先頭に戻す
        if (strncmp(buff, name_buff, strlen(name_buff)) == 0) {
          printf("\x1b[31m%s\x1b[m", buff); // 自分のメッセージは赤字
        } else printf("%s", buff); // 他ユーザのは黒字
      }
    }
  }
  close(sock);
}
