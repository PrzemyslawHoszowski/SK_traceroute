//
// Created by Przemysław Hoszowski on 19/03/2021.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "src/socket.h"
#include "src/packet.h"

int main(int args, char **argv) {
    if (args < 1) {
        printf("Host IPv4 address is required.");
        return 1;
    }
    uint32_t dsc_addres = inet_addr(argv[1]);
    printf("traceroute to %s\n", argv[1]);

    int socket = get_socket();
    if (socket == 0) {
        printf("Unable to get socket.");
        return 1;
    }

    char send_buffer[SEND_BUFFER_SIZE];
    struct ip *ip = &send_buffer;
    uint8_t buffer[IP_MAXPACKET];
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    int packet_len;
    fd_set descriptors;
    FD_ZERO (&descriptors);
    FD_SET(socket, &descriptors);
    struct timeval tv;


    pid_t pid = getpid();
    int packet_id = 0;
    for (int ttl = 1; ttl <= TTL_limit - 20; ttl++) {
        printf("\n %i:", ttl);
        for (int i = 0; i < 3; i++) {
            packet_id = prep_packet(ip, ttl, dsc_addres, pid);
            send_packet(ip, socket);
        }
        set_timer(&tv);
        int counter = 0;
        while (1) {
            packet_len = recvfrom(
                    socket,
                    buffer,
                    IP_MAXPACKET,
                    MSG_DONTWAIT,
                    (struct sockaddr *) &sender,
                    &sender_len);

            if (packet_len > 0) {
                struct ip *ip_rec = (struct ip*) buffer;
                struct icmp *icmp = buffer + 4 * ip->ip_hl;
                if(check_if_valid(pid, packet_id, buffer) == 1){
                    print_ip(&sender);
                    printf(" ");
                    counter++;
                    if((sender.sin_addr.s_addr) == dsc_addres)
                    if(counter>2) break;
                }
            }
            else {
                int ready = select(socket + 1, &descriptors, NULL, NULL, &tv);
                if(tv.tv_sec == 0 && tv.tv_usec == 0){
                    for(;counter <=2; counter++) printf(" * ");
                    break;
                }
            }
            struct ip *ip_header = (struct ip *) buffer;
            struct icmp *icmp_header = buffer + 4 * ip_header->ip_hl;

        }
    }
    putchar('\n');
    close(socket);
    return 0;
}
