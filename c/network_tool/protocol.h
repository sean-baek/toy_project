#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "pcap.h"

#define ETHERTYPE_IP 0x0800
#define ETHERTYPE_ARP 0x0806

struct nic_info
{
    uint8_t my_mac[6];
    struct in_addr addr;
    struct in_addr broadaddr;
    struct in_addr maskaddr;
    int ifindex;
};
#ifndef __linux__
    #pragma pack(push,1)
#endif
struct etherhdr
{
	uint8_t  ether_dhost[6];		/* destination eth addr	*/
	uint8_t  ether_shost[6];		/* source ether addr	*/
	uint16_t ether_type;		/* packet type ID field	*/
}
#ifndef __linux__
    ;
    #pragma pack(pop)
#else
__attribute__((__packed__));
#endif

#ifndef __linux__
    #pragma pack(push ,1)
#endif
struct	arphdr
{
	uint16_t ar_hrd;		// Format of hardware address
   	uint16_t ar_pro;		// Format of protocol address
   	uint8_t ar_hln;			// Length of hardware address
   	uint8_t ar_pln;			// Length of protocol address
   	uint16_t ar_op;			// ARP opcode (command)
   	uint8_t ar_sha[6];		// Sender hardware address
   	uint32_t ar_sip;		// Sender IP address why uint32_t? becouse sin_addr.s_addr is 4 byte
   	uint8_t ar_tha[6];		// Target hardware address
   	uint32_t ar_tip;		// Target IP address
}
#ifndef __linux__
    ;
    #pragma pack(pop)
#else
__attribute__((__packed__));
#endif

#ifndef __linux__
    #pragma pack(push, 1);
#endif
struct iphdr
{
    uint8_t ip_hl:4;		/* header length */
	uint8_t ip_v:4;		/* version */
	uint8_t ip_tos;			/* type of service */
	uint16_t ip_len;			/* total length */
	uint16_t ip_id;			/* identification */
	uint16_t ip_off;			/* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	uint8_t ip_ttl;			/* time to live */
	uint8_t ip_p;			/* protocol */
	uint16_t ip_sum;			/* checksum */
	struct in_addr ip_src, ip_dst;	/* source and dest address */
}
#ifndef __linux
    ;
    #pragma pack(pop)
#else
__attribute__((__packed__));
#endif

#ifndef __linux__
    #pragma pack(push, 1);
#endif
struct tcphdr
{
	uint16_t th_sport;
    uint16_t th_dport;
    uint32_t th_seq;
    uint32_t th_ack;
    uint8_t th_x2:4;
    uint8_t th_off:4;
    uint8_t th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
    uint16_t th_win;
    uint16_t th_sum;
    uint16_t th_urp;

}
#ifndef __linux
    ;
    #pragma pack(pop)
#else
__attribute__((__packed__));
#endif

#ifndef __linux__
    #pragma pack(push, 1);
#endif
struct pseudohdr
{
    struct in_addr ip_src, ip_dst;
    uint8_t reserved;
    uint8_t protocol_type;
    uint16_t tcp_total_length;
}
#ifndef __linux
    ;
    #pragma pack(pop)
#else
__attribute__((__packed__));
#endif

#ifndef __linux__
    #pragma pack(push, 1);
#endif
struct icmphdr
{
    uint8_t  icmp_type;
	uint8_t  icmp_code;
	uint16_t icmp_cksum;
	uint16_t icmp_id;
	uint16_t icmp_seq;
}
#ifndef __linux
    ;
    #pragma pack(pop)
#else
__attribute__((__packed__));
#endif

int get_info(struct nic_info *nic_info, char *if_name);
int get_interface_devices(char *arg);
//int get_ip_addr(char *arg, char *ip_addr);
//int get_subnetmask(char *arg, char *sub_addr);
//int get_boardcast_addr(char *arg, char *br_addr);
//int get_mac_addr(char *arg, char *mac);
//int get_nic();

#endif
