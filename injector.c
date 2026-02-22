#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <unistd.h>

// Standard Internet Checksum function (from Erickson's book)
unsigned short checksum(unsigned short *ptr, int nbytes) {
    long sum;
    unsigned short oddbyte;
    short answer;
    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((unsigned char *)&oddbyte) = *(unsigned char *)ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = (short)~sum;
    return answer;
}

int main() {
    int sockfd;
    char buffer[4096];
    struct iphdr *ip = (struct iphdr *) buffer;
    struct udphdr *udp = (struct udphdr *) (buffer + sizeof(struct iphdr));
    struct sockaddr_in sin;
    char *data = buffer + sizeof(struct iphdr) + sizeof(struct udphdr);

    // 1. Create a raw socket
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    // 2. Set up destination info
    sin.sin_family = AF_INET;
    sin.sin_port = htons(12345);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1"); // Target: Localhost

    memset(buffer, 0, 4096);

    // 3. Craft the Data (Payload)
    strcpy(data, "SOC Analyst Test Packet");

    // 4. Craft the IP Header
    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
    ip->id = htons(54321);
    ip->ttl = 255;
    ip->protocol = IPPROTO_UDP;
    ip->saddr = inet_addr("1.2.3.4"); // SPOOFED Source IP!
    ip->daddr = sin.sin_addr.s_addr;
    ip->check = checksum((unsigned short *)buffer, ip->tot_len);

    // 5. Craft the UDP Header
    udp->source = htons(1234);
    udp->dest = htons(12345);
    udp->len = htons(sizeof(struct udphdr) + strlen(data));
    udp->check = 0; // UDP checksum is optional

    // 6. Send the packet
    if (sendto(sockfd, buffer, ip->tot_len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("Sendto error");
    } else {
        printf("Packet Sent! Source: 1.2.3.4 -> Dest: 127.0.0.1\n");
    }

    close(sockfd);
    return 0;
}
