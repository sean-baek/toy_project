#include "protocol.h"

int get_nic()
{
    int sock;
    struct ifconf ifconf;
    struct ifreq ifr[50];
    int ifs;
    int i;

    if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket ");
        return -1;
    }

    ifconf.ifc_buf = (char *)ifr;
    ifconf.ifc_len = sizeof(ifr);

    printf("%s\n",ifconf.ifc_buf);
    printf("%d\n",ifconf.ifc_len);
    
    if(ioctl(sock, SIOCGIFCONF, &ifconf) == -1)
    {
        perror("ioctl() - ifconf");
        return -1;
    }

    ifs = ifconf.ifc_len / sizeof(ifr[0]);
    printf("interfaces = %d\n", ifs);
    for(i=0; i<ifs; i++)
    {
            printf("%s\n", ifr[i].ifr_name);
    }

    close(sock);
    return 0;
}
