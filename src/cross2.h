#ifndef UDPSOCKET_CROSS_WIN_H_
#define UDPSOCKET_CROSS_WIN_H_
#include "udpsocket/corss.h"

#ifdef _WIN32
typedef char data_t;
typedef int fromlen_t;
#else
typedef void data_t;
typedef unsigned int fromlen_t;
#endif

#endif
