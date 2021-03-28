//
// Created by Przemysław Hoszowski 314379 on 19/03/2021.
//


#ifndef TRACE_PACKET_H
#define TRACE_PACKET_H

#endif //TRACE_PACKET_H
#define TTL_limit 30
#define SEND_BUFFER_SIZE 128
#define TTL_EXCEEDED 11
#define ECHO_REQUEST 8
#define ECHO_REPLAY 0

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include "utils.h"

int check_if_valid(pid_t pid, int id, char *buffer);
int prep_packet(struct ip *packet, int8_t ttl, uint32_t dsc_address, pid_t pid);
