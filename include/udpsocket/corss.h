#ifndef UDPSOCKET_CROSS_H_
#define UDPSOCKET_CROSS_H_

#ifdef _WIN32
#define __restrict__ __restrict
typedef long ssize_t;
typedef uint16_t in_port_t;
typedef unsigned long long sockfd_t;
typedef int data_size_t;
#else
typedef int sockfd_t;
typedef size_t data_size_t;
#endif

#endif
