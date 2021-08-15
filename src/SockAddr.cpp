#include "udpsocket/SockAddr.h"

#include <arpa/inet.h>
#include <memory.h>

SockAddr::SockAddr() : data(nullptr), len(0) {}

SockAddr::SockAddr(const char *ip, int port) {
    struct in6_addr in6;
    if (inet_pton(AF_INET6, ip, &in6) == 1) {
        sockaddr_in6 *sa = new sockaddr_in6;
        memset(sa, 0, sizeof(sockaddr_in6));
        sa->sin6_family = AF_INET6;
        sa->sin6_addr = in6;
        sa->sin6_port = htons(port);
        this->data = sa;
        this->len = sizeof(sockaddr_in6);
        return;
    }

    struct in_addr in;
    if (inet_pton(AF_INET, ip, &in) == 1) {
        sockaddr_in *sa = new sockaddr_in;
        memset(sa, 0, sizeof(sockaddr_in));
        sa->sin_family = AF_INET;
        sa->sin_addr = in;
        sa->sin_port = htons(port);
        this->data = sa;
        this->len = sizeof(sockaddr_in);
        return;
    }

    this->data = nullptr;
    this->len = 0;
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
    if (data == nullptr) {
        return std::string();
    }

    char ipstr[INET6_ADDRSTRLEN];
    in_port_t port;
    switch (((sockaddr *)data)->sa_family) {
    case AF_INET: {
        sockaddr_in &sa = *(sockaddr_in *)data;
        inet_ntop(AF_INET, &(sa.sin_addr), ipstr, INET6_ADDRSTRLEN);
        port = ntohs(sa.sin_port);
    } break;
    case AF_INET6: {
        sockaddr_in6 &sa6 = *(sockaddr_in6 *)data;
        inet_ntop(AF_INET6, &(sa6.sin6_addr), ipstr, INET6_ADDRSTRLEN);
        port = ntohs(sa6.sin6_port);
    } break;
    }
    return std::string(ipstr) + ":" + std::to_string(port);
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