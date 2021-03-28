//
// Created by Przemysław Hoszowski 314379 on 19/03/2021.
//
// lhttps://github.com/PrzemyslawHoszowski/SK_traceroute.git

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "socket.h"
#include "packet.h"


int main(int args, char **argv) {
    if (args <= 1) {
        printf("Wymagany jest adres IP docelowego hosta.");
        return 1;
    }
    if (check_ip(argv[1]) == 0){
        printf("Podano błędny adres ip.");
        return 4;
    }
    uint32_t dsc_addres = inet_addr(argv[1]);
    printf("traceroute to %s\n", argv[1]);

    int socket = get_socket();
    if (socket == 0) {
        printf("Unable to get socket.");
        return 6;
    }

    char send_buffer[SEND_BUFFER_SIZE];
    struct ip *ip = (struct ip*) &send_buffer;
    char buffer[500];
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    int packet_len;
    pid_t pid = getpid();
    int packet_id = 0;
    int reached_target = 0;
    fd_set descriptors;
    struct timeval tv;

    in_addr_t senders[3];
    long long start;
    double sum=0;
    setbuf(stdout, 0);

    for (int ttl = 1; ttl <= TTL_limit && reached_target==0; ttl++) {
        // clearing variables before bigger ttl
        FD_ZERO (&descriptors);
        FD_SET(socket, &descriptors);
        set_timer(&tv);
        int counter = 0;
        sum = 0;
        for(int i=0; i<3;i++) senders[i] = 0;

        if(ttl < 10)
            printf("\n %i:  ", ttl);
        else
            printf("\n %i: ", ttl);

        start = millis();
        for (int i = 0; i < 3; i++) {
            packet_id = prep_packet(ip, ttl, dsc_addres, pid);
            send_packet((struct ip*) ip, socket);
        }

        while (1) {
            packet_len = recvfrom(
                    socket,
                    buffer,
                    IP_MAXPACKET,
                    MSG_DONTWAIT,
                    (struct sockaddr *) &sender,
                    &sender_len);

            if (packet_len > 0) {
                int option = check_if_valid(pid, packet_id, buffer);
                if(option > 0){
                    sum += millis() - start;
                    for (int i=0; i<3 && senders[i] != sender.sin_addr.s_addr; i++){
                        if(senders[i] != sender.sin_addr.s_addr){
                            senders[i] = sender.sin_addr.s_addr;
                            break;
                        }
                    }
                    counter++;
                    if(option == 2) reached_target = 1;
                    if(counter>2) break;
                }
            }
            else if(tv.tv_sec == 0 && tv.tv_usec == 0)
                break;
            else {
                if(0 > select(socket + 1, &descriptors, NULL, NULL, &tv)){
                    printf("Wystąpił błąd podczas oczekiwania na pakiet.");
                    return 5;
                }
            }
        }
        for (int i=0; i<3 && senders[i] != 0; i++) print_ip(senders[i]);
        if(counter == 3) printf("%.3f ms", sum/3000000);
        else if(counter > 0) printf("???");
        else printf("*");
    }
    putchar('\n');
    close(socket);
    return 0;
}
