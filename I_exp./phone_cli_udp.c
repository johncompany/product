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
#define N 1024

// エラー処理関数
void die(char *s) {
    perror(s);
    exit(1);
}

// EOD(すべて1のデータ)判定関数
int is_EOD(unsigned char *data, int len) {
  for (int i = 0;i < len;++i) {
    if (data[i] != 1) return 0;
  }
  return 1;
}


int main(int argc, char **argv) {
  if (argc != 3) {
    printf("usage: %s  ip_address  port_number\n", argv[0]);
    exit(1);
  }

  int err, n; // システムコールの返り値用
  unsigned char data_rec[N], data_play[N]; // 音声用

  // UDPでソケットを作る
  int sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (sock == -1) die("opening dgram socket");

  // アドレスを作る
  struct sockaddr_in senderinfo;
  socklen_t senderinfolen;
  senderinfolen = sizeof(senderinfo);
  // サーバデータ
  struct sockaddr_in send_addr;
  memset(&send_addr, 0, sizeof(send_addr));
  send_addr.sin_family = AF_INET;
  send_addr.sin_port = htons(atoi(argv[2]));
  send_addr.sin_addr.s_addr = inet_addr(argv[1]);

  // 通話開始の通知として1バイト("A")を送信
  err = sendto(sock, "A", 1, 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
  if (err == -1) die("sendto");
  printf("start phone\n");

  // rec, playを起動
  FILE* rec_p;
  if ((rec_p = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r")) == NULL) exit(1);
  FILE* play_p;
  if ((play_p = popen("play -t raw -b 16 -c 1 -e s -r 44100 -", "w")) == NULL) exit(1);

  while (1) {
    // rec voice
    n = fread(data_rec, sizeof(unsigned char), N, rec_p);
    err = sendto(sock, data_rec, sizeof(unsigned char)*n, 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
    if (err == -1) die("sendto");
    // play voice
    err = recvfrom(sock, data_play, sizeof(unsigned char)*N, 0, (struct sockaddr *)&senderinfo, &senderinfolen);
    if (err == -1) die("recvfrom");
    if (is_EOD(data_play, N)) break; // EODを受信したら通話終了
    fwrite(data_play, sizeof(unsigned char), err, play_p);
  }
  pclose(rec_p);
  pclose(play_p);
  printf("End Phone\n");
  close(sock);
  return 0;
}
