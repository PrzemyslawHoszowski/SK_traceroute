//
// Created by Przemysław Hoszowski on 19/03/2021.
//

#ifndef TRACE_SOCKET_H
#define TRACE_SOCKET_H

#endif //TRACE_SOCKET_H

#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>

#define WAITING_TIME_S 1
#define WAITING_TIME_MS 0


int get_socket();
void set_timer(struct timeval *tv);
void send_packet(struct ip *ip, int socket);
void get_data();