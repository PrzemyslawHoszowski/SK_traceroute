//
// Created by Przemysław Hoszowski 314379 on 19/03/2021.
//

#include "packet.h"
int check_if_valid(pid_t pid, int id, char *buffer){ // split into two cases
    struct ip* packet = (struct ip*) buffer;
    struct icmp* icmp1 = (struct icmp*) (buffer + (packet->ip_hl) * 4);

    /// if ECHO_REQUEST came to the target
    if(icmp1->icmp_type == ECHO_REPLAY) {
        if (icmp1->icmp_hun.ih_idseq.icd_seq / 3 == id / 3 && icmp1->icmp_hun.ih_idseq.icd_id == UINT16_C(pid))
            return 2;
        else return 0;
    }

    else if(icmp1->icmp_type == TTL_EXCEEDED) {
        struct ip *packet_inner = (struct ip *) (buffer + (packet->ip_hl) * 4 + 8);
        struct icmp *icmp = (struct icmp*) (buffer + (packet->ip_hl) * 4 + (packet_inner->ip_hl) * 4 + 8);

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
    struct icmp *icmp = (struct icmp*)(ip + 1);
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
