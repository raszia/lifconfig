#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

/*
    IFF_UP = 0x1,		// Interface is up.  
# define IFF_UP	IFF_UP
    IFF_BROADCAST = 0x2,	// Broadcast address valid.  
# define IFF_BROADCAST	IFF_BROADCAST
    IFF_DEBUG = 0x4,		// Turn on debugging.  
# define IFF_DEBUG	IFF_DEBUG
    IFF_LOOPBACK = 0x8,		// Is a loopback net.  
# define IFF_LOOPBACK	IFF_LOOPBACK
    IFF_POINTOPOINT = 0x10,	// Interface is point-to-point link.  
# define IFF_POINTOPOINT IFF_POINTOPOINT
    IFF_NOTRAILERS = 0x20,	// Avoid use of trailers.  
# define IFF_NOTRAILERS	IFF_NOTRAILERS
    IFF_RUNNING = 0x40,		// Resources allocated.  
# define IFF_RUNNING	IFF_RUNNING
    IFF_NOARP = 0x80,		// No address resolution protocol. 
# define IFF_NOARP	IFF_NOARP
    IFF_PROMISC = 0x100,	// Receive all packets.  
# define IFF_PROMISC	IFF_PROMISC
*/



int main(int argc, char *argv[]) {
    struct ifreq ifr;
    int sock, j, k, flags ;
    int mtu;
    char *p, addr[32], mask[32], mac[32], states[100];


    if (argc<2) {
        fprintf(stderr,"Give me a interface name\nexample: %s eth0\n",argv[0]);
        return 1;
    }

    sock=socket(PF_INET, SOCK_STREAM, 0);
    if (-1==sock) {
        perror("socket() ");
        return 1;
    }
    
    strncpy(ifr.ifr_name,argv[1],sizeof(ifr.ifr_name)-1);
    ifr.ifr_name[sizeof(ifr.ifr_name)-1]='\0';


    if (-1==ioctl(sock, SIOCGIFADDR, &ifr)) {
        perror("ioctl(SIOCGIFADDR) ");
        return 1;
    }
    p=inet_ntoa(((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr);
    strncpy(addr,p,sizeof(addr)-1);
    addr[sizeof(addr)-1]='\0';


    if (-1==ioctl(sock, SIOCGIFNETMASK, &ifr)) {
        perror("ioctl(SIOCGIFNETMASK) ");
        return 1;
    }

    p=inet_ntoa(((struct sockaddr_in *)(&ifr.ifr_netmask))->sin_addr);
    strncpy(mask,p,sizeof(mask)-1);
    mask[sizeof(mask)-1]='\0';


    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0){
        perror("ioctl(SIOCGIFFLAGS) ");
        return 1;
    }
    flags = ifr.ifr_flags;
	if (flags & IFF_UP)
        strcpy(states,"UP,");
    else
        strcpy(states,"DOWN,");
    if (flags & IFF_BROADCAST)
        strcat(states,"BROADCAST,");
    if (flags & IFF_LOOPBACK)
	    strcat(states,"LOOPBACK,");
    if (flags & IFF_RUNNING)
	    strcat(states,"RUNNING,");
    if (flags & IFF_NOARP)
	    strcat(states,"NOARP,");
    if (flags & IFF_MULTICAST)
	    strcat(states,"MULTICAST,");
    if (states[strlen(states)-1] == ',')
      states[strlen(states)-1] = ' ';
    else
      states[strlen(states)-1] = 0;

    if (-1==ioctl(sock, SIOCGIFHWADDR, &ifr)) {
        perror("ioctl(SIOCGIFHWADDR) ");
        return 1;
    }
    for (j=0, k=0; j<6; j++) {
        k+=snprintf(mac+k, sizeof(mac)-k-1, j ? ":%02X" : "%02X",(int)(unsigned int)(unsigned char)ifr.ifr_hwaddr.sa_data[j]);
    }
    mac[sizeof(mac)-1]='\0';

    if (ioctl(sock, SIOCGIFMTU, &ifr) < 0)
		mtu = 0;
	else
		mtu = ifr.ifr_mtu;



    printf("ifname: %s\n",ifr.ifr_name);
    printf("address: %s\n",addr);
    printf("netmask: %s\n",mask);
    printf("macaddr: %s\n",mac);
    printf("mtu: %d \n",mtu);
    printf("state: %s\n",states);
    printf("flags: %d\n",flags);

    close(sock);
    return 0;
}
