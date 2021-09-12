#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <WinSock2.h>
#endif

#include <udpsocket/UDPSocket.h>

#define MYPORT 8888

void err_exit(const char *tag) {
#ifdef _WIN32
    int e = WSAGetLastError();
    fprintf(stderr, "%s: %d", tag, e);
#else
    perror(tag);
#endif
    exit(EXIT_FAILURE);
}

void server(UDPSocket sock) {
    SockAddr addr;
    int res;
    char recvbuf[1024];

    while (1) {
        memset(recvbuf, 0, sizeof(recvbuf));

        res = sock.recvfrom(recvbuf, sizeof(recvbuf), addr);
        if (res < 0) {
            if (errno == EINTR)
                continue;
            err_exit("sendto");
        }
        printf("%s->me:%s\n", addr.str().c_str(), recvbuf);

        res = sock.sendto(recvbuf, res, addr);
        if (res < 0) {
            err_exit("sendto");
        }
        printf("me->%s:%s\n", addr.str().c_str(), recvbuf);

        if (memcmp(recvbuf, "exit", 4) == 0) {
            break;
        }
    }

    sock.close();
}

int main(void) {
    UDPSocket sock;

    printf("Listening port %d\n", MYPORT);
    sock.bind(MYPORT);

    server(sock);

    return 0;
}