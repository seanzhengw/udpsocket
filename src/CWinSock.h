#ifndef UDPSOCKET_WIN_SUPPORT_H_
#define UDPSOCKET_WIN_SUPPORT_H_
#ifdef _WIN32

#include <winsock2.h>

class WinSock {
  public:
    static WSADATA &WSAInstance();
};

inline WSADATA &WinSock::WSAInstance() {
    static WSADATA wsaData;
    static bool winsockinit = false;
    if (!winsockinit) {
        winsockinit = WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
    }
    return wsaData;
}

#endif
#endif
