#ifndef UDPSOCKET_UDPSOCKET_H_
#define UDPSOCKET_UDPSOCKET_H_

#include <sys/types.h>

#include "SockAddr.h"
#include "corss.h"

class UDPSocket {
  public:
    UDPSocket();
    UDPSocket(const UDPSocket &sock);
    ~UDPSocket();

    UDPSocket &operator=(const UDPSocket &sock);

    int bind(int port);

    ssize_t sendto(const void *buf, data_size_t len, const SockAddr &addr);

    ssize_t recvfrom(void *__restrict__ buf, data_size_t len,
                     SockAddr *__restrict__ addr);
    ssize_t recvfrom(void *__restrict__ buf, data_size_t len, SockAddr &addr);

    int close();

    operator sockfd_t();

  private:
    void *data;
};

#endif
