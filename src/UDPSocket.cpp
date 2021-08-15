#include "udpsocket/UDPSocket.h"

#include <arpa/inet.h>
#include <atomic>
#include <cstring>
#include <memory>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct sock_st {
    int fd;
    std::atomic_long ref;

    sock_st() : fd(0), ref(0) {}
};

#define SOCK_ST ((sock_st *)data)
#define SOCKFD (SOCK_ST->fd)
#define SOCKREF (SOCK_ST->ref)

UDPSocket::UDPSocket() {
    this->data = new sock_st;
    ++SOCKREF;

    SOCKFD = socket(PF_INET6, SOCK_DGRAM, 0);
    if (SOCKFD < 0) {
        SOCKFD = -1;
    }

    int no = 0;
    int *off = &no;
    setsockopt(SOCKFD, IPPROTO_IPV6, IPV6_V6ONLY, off, sizeof(*off));
}

UDPSocket::UDPSocket(const UDPSocket &sock) {
    this->data = sock.data;
    ++SOCKREF;
}

UDPSocket::~UDPSocket() {
    --SOCKREF;
    if (SOCKREF == 0) {
        delete SOCK_ST;
    }
}

UDPSocket &UDPSocket::operator=(const UDPSocket &sock) {
    this->data = sock.data;
    ++SOCKREF;
    return *this;
}

int UDPSocket::bind(int port) {
    struct sockaddr_in6 servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(port);
    servaddr.sin6_addr = in6addr_any;
    return ::bind(SOCKFD, (struct sockaddr *)&servaddr, sizeof(servaddr));
}

ssize_t UDPSocket::sendto(const void *buf, size_t len, const SockAddr &addr) {
    return ::sendto(SOCKFD, buf, len, 0, (const sockaddr *)addr.data, addr.len);
}

ssize_t UDPSocket::recvfrom(void *__restrict__ buf, size_t len,
                            SockAddr *__restrict__ addr) {
    if (addr == nullptr) {
        return ::recvfrom(SOCKFD, buf, len, 0, NULL, NULL);
    }
    return recvfrom(buf, len, *addr);
}

ssize_t UDPSocket::recvfrom(void *__restrict__ buf, size_t len,
                            SockAddr &addr) {
    sockaddr_storage ra;
    socklen_t ral = sizeof(sockaddr_storage);
    auto r = ::recvfrom(SOCKFD, buf, len, 0, (sockaddr *)(&ra), &(ral));
    addr.set(&ra, ral);
    return r;
}

int UDPSocket::close() {
    return ::close(SOCKFD);
}
