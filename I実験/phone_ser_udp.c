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
#define N 1024

// エラー処理関数
void die(char *s) {
    perror(s);
    exit(1);
}


int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s  port_number\n", argv[0]);
    exit(1);
  }

  int err; // システムコールの返り値用
  struct pollfd fds[2]; // poll()で見るための読み込みソケット
  unsigned char data_rec[N], data_play[N]; // 音声用
  unsigned char buff[N]; // 標準入力,およびEOD送信用
  char flag[1]; // クライアントからの通知用(1バイト)

  // UDPでソケットを作る
  int sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (sock == -1) die("opening dgram socket");

  // クライアントデータ
  struct sockaddr_in senderinfo;
  socklen_t addrlen;
  addrlen = sizeof(senderinfo);
  // サーバデータ
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[1]));
  addr.sin_addr.s_addr = INADDR_ANY;

  err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  if (err == -1) die("bind");

  fds[0].fd = sock; // fdsにsockを登録
  fds[0].events = POLLIN; //reventsにPOLLINが返るように設定
  fds[1].fd = 0; // fdsに標準入力を登録
  fds[1].events = POLLIN; //reventsにPOLLINが返るように設定

  // 1バイト受け取ったら通話開始
  err = recvfrom(sock, flag, 1, 0, (struct sockaddr *)&senderinfo, &addrlen);
  if (err == -1) die("recvfrom");
  printf("Start Phone\n");

  // rec, playを起動
  FILE* rec_p;
  if ((rec_p = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r")) == NULL) exit(1);
  FILE* play_p;
  if ((play_p = popen("play -t raw -b 16 -c 1 -e s -r 44100 -", "w")) == NULL) exit(1);

  while (1) {
    poll(fds, 2, 0);	// sockと標準入力を監視
    if (fds[0].revents == POLLIN) { // クライアントからの通信要求がある場合に入る
      // play voice
      err = recvfrom(sock, data_play, sizeof(unsigned char)*N, 0, (struct sockaddr *)&senderinfo, &addrlen);
      if (err == -1) die("recvfrom");
      fwrite(data_play, sizeof(unsigned char), err, play_p);
      // rec voice
      int n = fread(data_rec, sizeof(unsigned char), N, rec_p);
      err = sendto(sock, data_rec, sizeof(unsigned char)*n, 0, (struct sockaddr *)&senderinfo, addrlen);
      if (err == -1) die("sendto");
    }

    if (fds[1].revents == POLLIN) {	// キーボードからの入力がある場合に入る
      scanf("%32s", buff);
      if (strcmp((char const *)buff, "END") == 0) break;	// "END"が入力されたら終了
      else printf("無効なコマンドです\n");
    }
  }
  // EOD(すべて1のデータ)を送信
  memset(&buff, 1, sizeof(buff));
  err = sendto(sock, buff, sizeof(unsigned char)*N, 0, (struct sockaddr *)&senderinfo, addrlen);
  if (err == -1) die("sendto");

  pclose(rec_p);
  pclose(play_p);
  printf("End Phone\n");
  close(sock);
  return 0;
}
