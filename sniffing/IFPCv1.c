/* 
 
 this program is called "IFPC"

 "IFPC" is an abbreviation for "interfaces packet capture"

 This tool shows the user the network intefaces on the user's computer,

 and when the user selects one of the interfaces,

 shows the packets coming through the interface to the use
 
 This tool is still in the version 1

 so it will continue to develop in the future.

 */

#include <stdio.h>
#include <string.h>
#include "pcap.h"

#ifndef __linux__
	#include <winsock2.h>
#else
	#include <unistd.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <netinet/ether.h>
	#include <linux/if_packet.h>
#endif

#define BUFSIZE 2048

int main(void)
{
	pcap_if_t *alldevs, *d;
	
	struct pcap_addr *a;
	struct sockaddr_ll sll;
	
	char errbuf[PCAP_ERRBUF_SIZE];
	char buffer[BUFSIZE];
	char *pkttype, *protocol;
	char input_agree;
	
	int i = 0;
	int sock, num, addr_len, len;

	if(pcap_findalldevs(&alldevs, errbuf) < 0)
	{
		perror("pcap_findalldevs() error");
		return 1;
	}

	for(d = alldevs; d; d=d->next)
	{
		printf("%d : %s\n", ++i, d->name);
	}

	printf("Which interface would you use : ");
	scanf("%d", &num);

	if(num < 0 || num >= i)
	{
		perror("Select from the number of interfaces presented");
		return 1;
	}

	for(d = alldevs, i=0; d; d = d->next)
	{
		if(num == ++i) break;
	}
	
	printf("name : %s\n", d->name);

	if(d -> description)
		printf("description : %s\n", d->description);

	for(a = d->addresses; a; a = a->next)
	{
		struct sockaddr_in *in_addr = (struct sockaddr_in*)a->addr;

		switch(a->addr->sa_family)
		{
			case AF_INET:
				printf("address : %s\n", inet_ntoa(in_addr->sin_addr));
				break;

			case AF_INET6:
				printf("address : %s\n", inet_ntoa(in_addr->sin_addr));
				break;

			case AF_LOCAL:
				printf("address : %s\n", inet_ntoa(in_addr->sin_addr));
				break;
		}
	}

	printf("Would you capture the packet using %s (y/n) : ", d->name);
	scanf("%c", &input_agree);

	if(input_agree == 'n' || input_agree == 'N')
	{
		printf("Okay goodbyeeeeee :)");
		return 0;
	}

	else
	{
		if((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
		{
			perror("socket()");
			return 1;
		}

		addr_len = sizeof(sll);

		while((len = recvfrom(sock, buffer, BUFSIZE, 0, (struct sockaddr*)&sll, &addr_len)) >= 0)
		{
			switch(sll.sll_pkttype)
			{
				case PACKET_HOST:
					pkttype = "PACKET_HOST";
					break;
				case PACKET_BROADCAST:
					pkttype = "PACKET_BROADCAST";
					break;
				case PACKET_MULTICAST:
					pkttype = "PACKET_MULTICAST";
					break;
				case PACKET_OTHERHOST:
					pkttype = "PACKET_OTHERHOST";
					break;
				default:
					pkttype = "Unknown";
			}

			switch(ntohs(sll.sll_protocol))
			{
				case ETH_P_IP:
					protocol = "IP";
					break;

				case ETH_P_ARP:
					protocol = "ARP";
					break; 

				default:
					protocol = "Unknown";
			}
			
			printf("[%d][%s][%s][len : %d]\n", sll.sll_ifindex, pkttype, protocol, len);
		}
	}

	pcap_freealldevs(alldevs);
	close(sock);

	return 0;
}
