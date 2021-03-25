//
// Created by Przemysław Hoszowski on 19/03/2021.
//

#include "packet.h"
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

int check_if_valid(pid_t pid, int id, char *buffer){ // split into two cases
    struct ip* packet = (struct ip*) buffer;
    struct icmp* icmp1 = buffer + (packet->ip_hl) * 4;

    /// if ECHO_REQUEST came to the target
    if(icmp1->icmp_type == ECHO_REPLAY) {
        if (icmp1->icmp_hun.ih_idseq.icd_seq / 3 == id / 3 && icmp1->icmp_hun.ih_idseq.icd_id == UINT16_C(pid))
            return 2;
        else return 0;
    }

    else if(icmp1->icmp_type == TTL_EXCEEDED) {
        struct ip *packet_inner = (struct ip *) (buffer + (packet->ip_hl) * 4 + 8);
        struct icmp *icmp = buffer + (packet->ip_hl) * 4 + (packet_inner->ip_hl) * 4 + 8;

        uint16_t icd_id = icmp->icmp_hun.ih_idseq.icd_id;
        uint16_t icd_seq = icmp->icmp_hun.ih_idseq.icd_seq;

        if ((icd_id == UINT16_C(pid)) && icd_seq / 3 == id / 3)
            return 1;
    }
    return 0;
}

int prep_packet(struct ip *ip, int8_t ttl, uint32_t dsc_address, pid_t pid){
    if(ttl > TTL_limit){
        printf("TTL limit exceeded");
        exit(2);
    }
    static unsigned short packet_id = 0;

    // fill icmp part
    struct icmp *icmp = (struct icmp *)(ip + 1);
    clear_buffer(ip, SEND_BUFFER_SIZE);
    icmp->icmp_type = ECHO_REQUEST;
    icmp->icmp_code = 0;
    icmp->icmp_hun.ih_idseq.icd_id = UINT16_C(pid);
    icmp->icmp_hun.ih_idseq.icd_seq = packet_id;
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = compute_icmp_checksum(icmp, sizeof(struct icmp));

    // fill ip part
    ip->ip_v = 4;
    ip->ip_hl = 5;
    ip->ip_ttl = ttl;
    ip->ip_dst.s_addr = dsc_address;
    ip->ip_p= IPPROTO_ICMP;
    ip->ip_len = htons(sizeof(struct ip) + sizeof(struct icmphdr));
    packet_id++;
    return packet_id -1;
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