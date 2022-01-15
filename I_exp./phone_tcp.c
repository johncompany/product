#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define N 10000

// for server
void server(char *port) {
    int ss = socket(PF_INET, SOCK_STREAM, 0);
    if (ss == -1) {
        perror("opening stream socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET; // IPv4
    addr.sin_port = htons(atoi(port)); // port number
    addr.sin_addr.s_addr = INADDR_ANY; // any address
    bind(ss, (struct sockaddr *)&addr, sizeof(addr));
    listen(ss, 10);

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int s = accept(ss, (struct sockaddr *)&client_addr, &len);
    close(ss);
    FILE* rec_p;
    if ((rec_p = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r")) == NULL) exit(1);
    FILE* play_p;
    if ((play_p = popen("play -t raw -b 16 -c 1 -e s -r 44100 -", "w")) == NULL) exit(1);

    unsigned char data_rec[N], data_play[N];
    int nr, nw;
    while (1) {
        // rec voice
        nr = fread(data_rec, sizeof(unsigned char), N, rec_p);
        if (nr == 0) break;
        nw = send(s, data_rec, nr, 0);
        if (nw == -1) {
            perror("send");
            exit(1);
        }
        // play voice
        nr = recv(s, data_play, N, 0);
        if (nr == -1) {
            perror("recv");
            exit(1);
        }
        nw = fwrite(data_play, sizeof(unsigned char), nr, play_p);
    }
    pclose(rec_p);
    pclose(play_p);
    close(s);
}

// for client
void client(char *ip_addr, char *port) {
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        perror("opening stream socket");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET; // IPv4
    addr.sin_addr.s_addr = inet_addr(ip_addr); // IP address
    addr.sin_port = htons(atoi(port)); // port number
    int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr)); // connect
    if (ret == -1) {
        perror("connecting stream socket");
        exit(1);
    }
    FILE* rec_p;
    if ((rec_p = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r")) == NULL) exit(1);
    FILE* play_p;
    if ((play_p = popen("play -t raw -b 16 -c 1 -e s -r 44100 -", "w")) == NULL) exit(1);

    unsigned char data_rec[N], data_play[N];
    int nr, nw;
    while (1) {
        // rec voice
        nr = fread(data_rec, sizeof(unsigned char), N, rec_p);
        if (nr == 0) break;
        nw = send(s, data_rec, nr, 0);
        if (nw == -1) {
            perror("send");
            exit(1);
        }
        // play voice
        nr = recv(s, data_play, N, 0);
        if (nr == -1) {
            perror("recv");
            exit(1);
        }
        nw = fwrite(data_play, sizeof(unsigned char), nr, play_p);
    }
    pclose(rec_p);
    pclose(play_p);
    close(s);
}

int main(int argc, char **argv) {
    if (argc == 2) {
        server(argv[1]);
    } else if (argc == 3) {
        client(argv[1], argv[2]);
    } else {
        printf("usage\nserver: %s port\nclient: %s address port\n", argv[0], argv[0]);
        exit(1);
    }
    return 0;
}
