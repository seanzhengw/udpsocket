#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <WinSock2.h>
#endif

#include <udpsocket/UDPSocket.h>

#define MYPORT 8888
#define SERVERIP "127.0.0.1"

void err_exit(const char *tag) {
#ifdef _WIN32
    int e = WSAGetLastError();
    fprintf(stderr, "%s: %d", tag, e);
#else
    perror(tag);
#endif
    exit(EXIT_FAILURE);
}

void client(UDPSocket sock) {
    SockAddr serv(SERVERIP, MYPORT);
    SockAddr addr;
    int ret;
    char sendbuf[1024];
    char recvbuf[1024];

    printf("Send to %s\n", serv.str().c_str());

    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {
        memset(recvbuf, 0, sizeof(recvbuf));

        ret = sock.sendto(sendbuf, strlen(sendbuf) - 1, serv);
        if (ret < 0) {
            err_exit("sendto");
        }
        printf("me->%s:%s", serv.str().c_str(), sendbuf);

        ret = sock.recvfrom(recvbuf, sizeof(recvbuf), addr);
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            err_exit("recvfrom");
        }
        printf("%s->me:%s\n", serv.str().c_str(), recvbuf);

        memset(sendbuf, 0, sizeof(sendbuf));
    }

    sock.close();
}

int main(void) {
    UDPSocket sock;

    client(sock);

    return 0;
}