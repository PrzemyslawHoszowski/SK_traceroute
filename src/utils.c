//
// Created by Przemysław Hoszowski on 26/03/2021.
//

#include "utils.h"

void print_as_bytes (unsigned char* buff, ssize_t length)
{
    printf("\n");
    for (ssize_t i = 0; i < length; i++, buff++)
        printf ("%.2x ", *buff);

    printf("END\n");
}

u_int16_t compute_icmp_checksum (const void *buff, int length)
{
    u_int32_t sum;
    const u_int16_t* ptr = buff;
    assert (length % 2 == 0);
    for (sum = 0; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16) + (sum & 0xffff);
    return (u_int16_t)(~(sum + (sum >> 16)));
}

int64_t millis()
{
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((int64_t) now.tv_sec) * 1000000000 + ((int64_t) now.tv_nsec);
}


void print_ip(in_addr_t addr){
    char str[20];
    struct in_addr myaddr;
    myaddr.s_addr = addr;

    inet_ntop(AF_INET,
              &myaddr,
              str,
              sizeof(str));
    printf("%s ", str);
}

void clear_buffer(void *buff, int n){
    memset(buff, 0, n);
}

int check_ip(char* ip){
    for(int part = 0; part < 4; part++){
        int number = 0;
        while(*ip != 0) {
            if ('0' <= *ip && *ip <= '9') {
                if (*ip == '0' && number == 0) return 0;
                number = number * 10 + (*ip - '0');
            }
            else if (*ip == '.') {
                ip++;
                break;
            }
            else return 0;
            ip++;
        }
        if(number > 255) return 0;
    }
    if(*ip != 0 || *(ip-1) == '.') return 0;
    return 1;
}