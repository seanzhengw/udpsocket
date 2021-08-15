#ifndef UDPSOCKET_SOCKADDR_H_
#define UDPSOCKET_SOCKADDR_H_

#include <string>

class UDPSocket;

class SockAddr {
  public:
    SockAddr();
    SockAddr(const char *ip, int port);
    SockAddr(const SockAddr &sa);
    ~SockAddr();

    SockAddr &operator=(const SockAddr &sa);

    std::string str() const;

    friend class UDPSocket;

  private:
    void *data;
    unsigned int len;

    void set(void *data, unsigned int len);
    void clear();
};

#endif
