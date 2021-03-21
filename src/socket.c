//
// Created by Przemysław Hoszowski on 19/03/2021.
//


#include "socket.h"


int get_socket(){
    return socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
}

void send_packet(struct ip *ip, int socket) {
    struct sockaddr_in dest_info;
    int enable;
    setsockopt(socket, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));
    dest_info.sin_family = AF_INET;
    dest_info.sin_addr = ip->ip_dst;

    if(sendto(socket, ip, ntohs(ip->ip_len), 0, (struct sockaddr*) &dest_info, sizeof(dest_info)) < 0){
        printf("Unable to sent packet");
        exit(3);
    }
}

void set_timer(struct timeval *tv){
    tv->tv_sec = WAITING_TIME_S;
    tv->tv_usec = WAITING_TIME_MS;
}

