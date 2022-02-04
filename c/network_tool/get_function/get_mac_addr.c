#include "protocol.h"

int get_mac_addr(char *arg, char *mac)
{
    int sock;
    struct ifreq ifr;
    uint8_t mac_addr[6] = {0,};

    if((sock = socket(PF_INET,SOCK_STREAM, 0)) < 0)
    {
        perror("socket ");
        return -1;
    }

    strcpy(ifr.ifr_name, arg);
    if(ioctl(sock, SIOCGIFHWADDR, &ifr) < 0)
    {
        perror("ioctl() - get mac");
        close(sock);
        return -1;
    }

    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);

    close(sock);
    return 0;
}

/*
unsigned char cMacAddr[8]; //server's MAC addr

static int GetServerMacAddress(char *pIface)
{
	int nSD;
	struct ifreq sIfReq;
	struct if_nameindex *pIfList;
	struct if_nameindex *pListSave;

	pIfList = (struct if_nameindex *)NULL;
	pListSave = (struct if_nameindex *)NULL;

	#ifndef SIOCGIFADDR
	return 0;
	#endif

	nSD = socket(PF_INET, SOCK_STREAM, 0);
	if(nSD < 0)
	{
		printf("File %s : line %d : Socket failed\n", __FILE__, __LINE__);
		return 0;
	}

	pIfList = pListSave = if_nameindex();

	for(pIfList; *(char*)pIfList != 0; pIfList++)
	{
		if(strcmp(pIfList->if_name, pIface))
		continue;
		strncpy(sIfReq.ifr_name, pIfList->if_name, IF_NAMESIZE);

		if(ioctl(nSD, SIOCGIFHWADDR, &sIfReq) != 0)
		{
			printf("File %s : line %d : Ioctl failed\n", __FILE__, __LINE__);
			return 0;
		}

		memmove((void*)&cMacAddr[0], (void*)&sIfReq.ifr_ifru.ifru_hwaddr.sa_data[0], 6);
		break;
	}

	if_freenameindex(pListSave);
	close(nSD);
	return 1;
}

int main(int argc, char* argv[])
{
	bzero((void*)&cMacAddr[0], sizeof(cMacAddr));
	if(!GetServerMacAddress("eth0"))
	{
		printf("Fatal error : Failed to get local host's MAC Address\n");
	}

	printf("HWaddr %02X:%02X:%02X:%02X:%02X:%02X\n", cMacAddr[0], cMacAddr[1], cMacAddr[2], cMacAddr[3], cMacAddr[4], cMacAddr[5]);

	exit(0);
}
*/
