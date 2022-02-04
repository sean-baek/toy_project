#include "protocol.h"

int get_ip_addr(char *arg, char *ip_addr)
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
    if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl() - ip addr ");
        close(sock);
        return -1;
    }
    sin = (struct sockaddr_in *)&ifr.ifr_addr;
    strcpy(ip_addr, inet_ntoa(sin->sin_addr));

    close(sock);
    return 0;
}
