//
// Created by Przemysław Hoszowski on 19/03/2021.
//

#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include "src/socket.h"
#include "src/packet.h"


int main(int args, char** argv) {
    if(args < 1){
        printf("Host IPv4 address is required.");
        return 1;
    }
    struct sockaddr_in address;
    char address_str[INET_ADDRSTRLEN];
    inet_pton(AF_INET, argv[1], &(address.sin_addr));
    inet_ntop(AF_INET, &(address.sin_addr), address_str, INET_ADDRSTRLEN);
    printf("traceroute to %s", address_str);

    int socket = get_socket();
    if(socket == 0){
        printf("Unable to get socket.");
        return 1;
    }

    while(1){
        wait_for_packet(socket);
        pid_t pid = getpid();
        printf("pid: %lu", pid);
        break;
    }
    return 0;
}
