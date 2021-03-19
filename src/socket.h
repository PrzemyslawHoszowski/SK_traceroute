//
// Created by Przemysław Hoszowski on 19/03/2021.
//

#ifndef TRACE_SOCKET_H
#define TRACE_SOCKET_H

#endif //TRACE_SOCKET_H

#include <arpa/inet.h>

int get_socket(){
    return socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
}