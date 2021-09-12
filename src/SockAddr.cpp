#include "udpsocket/SockAddr.h"

#include <arpa/inet.h>
#include <memory.h>

SockAddr::SockAddr()
    : data(new char[sizeof(sockaddr_in6)]), len(sizeof(sockaddr_in6)) {
    memset(data, 0, sizeof(sockaddr_in6));
}

SockAddr::SockAddr(const char *ip, int port)
    : data(new char[sizeof(sockaddr_in6)]), len(sizeof(sockaddr_in6)) {
    memset(data, 0, sizeof(sockaddr_in6));
    sockaddr_in6 *sa = (sockaddr_in6 *)data;

    bool ok = inet_pton(AF_INET6, ip, &(sa->sin6_addr)) == 1;
    if (!ok) {
        if (strlen(ip) <= INET_ADDRSTRLEN) {
            char ipstr[INET6_ADDRSTRLEN];
            sprintf(ipstr, "::FFFF:%s", ip);
            ok = inet_pton(AF_INET6, ipstr, &(sa->sin6_addr)) == 1;
        }
        if (!ok) {
            return;
        }
    }

    sa->sin6_family = AF_INET6;
    sa->sin6_port = htons(port);
}

SockAddr::SockAddr(const SockAddr &sa) {
    set(sa.data, sa.len);
}

SockAddr::~SockAddr() {
    clear();
}

SockAddr &SockAddr::operator=(const SockAddr &sa) {
    set(sa.data, sa.len);
    return *this;
}

std::string SockAddr::str() const {
    sockaddr_in6 *addr = (sockaddr_in6 *)data;
    char ipstr[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(addr->sin6_addr), ipstr, INET6_ADDRSTRLEN);
    return std::string(ipstr) + ":" + std::to_string(ntohs(addr->sin6_port));
}

void SockAddr::set(void *data, unsigned int len) {
    clear();
    this->data = new char[len];
    this->len = len;
    memcpy(this->data, data, len);
}

void SockAddr::clear() {
    if (data != nullptr) {
        delete[]((char *)data);
        data = nullptr;
        len = 0;
    }
}