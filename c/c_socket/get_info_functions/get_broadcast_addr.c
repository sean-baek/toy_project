#include "protocol.h"

int get_boardcast_addr(char *arg, char *br_addr)
{
    int sock;
    struct ifreq ifr;
    struct sockaddr_in *sin;

    if((sock = socket(PF_INET,SOCK_STREAM, 0)) < 0)
    {
        perror("socket ");
        return -1;
    }

    strcpy(ifr.ifr_name, arg);
    if(ioctl(sock, SIOCGIFBRDADDR, &ifr) < 0)
    {
        perror("ioctl() - get broadcast");
        close(sock);
        return -1;
    }
    sin = (struct sockaddr_in *)&ifr.ifr_addr;
    strcpy(br_addr, inet_ntoa(sin->sin_addr));

    close(sock);
    return 0;
}
