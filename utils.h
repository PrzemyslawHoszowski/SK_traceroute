//
// Created by Przemysław Hoszowski 314379 on 26/03/2021.
//

#ifndef TRACE_UTILS_H
#define TRACE_UTILS_H

#endif //TRACE_UTILS_H
#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <netdb.h>
#include <memory.h>
#include <arpa/inet.h>


int64_t millis(); // https://stackoverflow.com/questions/3557221/how-do-i-measure-time-in-c
void clear_buffer(void *buff, int n);
void print_ip(in_addr_t addr);
int check_ip(char *ip);
u_int16_t compute_icmp_checksum (const void *buff, int length);
void print_as_bytes (unsigned char* buff, ssize_t length);