#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define PORT_NUM 50122
#define GROUP_ADDRESS "239.0.0.1" // Multicast group IP address
#define LOCAL_ADDRESS "169.254.17.247" // ethernet IP address

struct Tuple {
    int socket;
    struct sockaddr_in address;
};

struct Tuple initMulticast(){
    struct in_addr localInterface;
    struct sockaddr_in groupSock;
    struct ip_mreq group;
    int sd;

    // Create a datagram socket on which to send.
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0) {
      perror("Opening datagram socket error");
      exit(1);
    }

    // Initialize the group sockaddr structure with a
    memset((char *) &groupSock, 0, sizeof(groupSock));
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr(GROUP_ADDRESS);
    groupSock.sin_port = htons(PORT_NUM);

    // Disable loopback so you do not receive your own datagrams.
    char loopch = 0;
    if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0) {
        perror("Setting IP_MULTICAST_LOOP error");
        close(sd);
        exit(1);
    }

    // Set local interface for outbound multicast datagrams.
    // The IP address specified must be associated with a local,
    // multicast capable interface.
    localInterface.s_addr = inet_addr(LOCAL_ADDRESS);
    if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
      perror("Setting local interface error");
      exit(1);
    }

    // Enable SO_REUSEADDR to allow multiple instances of this
    // application to receive copies of the multicast datagrams.
    int reuse = 1;
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
        perror("Setting SO_REUSEADDR error");
        close(sd);
        exit(1);
    }

    // Bind to the proper port number with the IP address
    if(bind(sd, (struct sockaddr*)&groupSock, sizeof(groupSock))) {
        perror("Binding datagram socket error");
        close(sd);
        exit(1);
    }

    // Join the multicast group on the local IP4
    // interface. Note that this IP_ADD_MEMBERSHIP option must be
    // called for each local interface over which the multicast
    // datagrams are to be received.
    group.imr_multiaddr.s_addr = inet_addr(GROUP_ADDRESS);
    group.imr_interface.s_addr = inet_addr((char*) LOCAL_ADDRESS);

    if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
        perror("Adding multicast group error");
        close(sd);
        exit(1);
    }

    struct Tuple r = {sd, groupSock};
    return r;
}
