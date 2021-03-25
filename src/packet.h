//
// Created by Przemysław Hoszowski on 19/03/2021.
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
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include "socket.h"

int check_if_valid(pid_t pid, int id, char *buffer);
int prep_packet(struct ip *packet, int8_t ttl, uint32_t dsc_address, pid_t pid);
void clear_buffer(void *buff, int n);
void print_ip(in_addr_t addr);
int check_ip(char *ip);