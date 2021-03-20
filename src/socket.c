//
// Created by Przemysław Hoszowski on 19/03/2021.
//

#include "socket.h"


int get_socket(){
    return socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
}

void wait_for_packet(int socket){
    //todo
    return;
}