/*
Adapted from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
The port number used in 51717.
This code is compiled and run on the Raspberry as follows:

    g++ -o turntable main.c network.c turntable.c -lwiringPi -lwiringPiDev
    ./turntable

The server waits for a connection request from a client.
The server assumes the client will send positive integers, which it sends back multiplied by 2.
If the server receives -1 it closes the socket with the client.
If the server receives -2, it exits.

http://www.tenouk.com/Module41c.html
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "turntable.c"

#define HELLO_PORT 50122
#define HELLO_GROUP "239.0.0.1" // Multicast group IP address
#define MSGBUFSIZE 256
#define local_address "169.254.17.247" // ethernet IP address

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
    groupSock.sin_addr.s_addr = inet_addr(HELLO_GROUP);
    groupSock.sin_port = htons(HELLO_PORT);

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
    localInterface.s_addr = inet_addr(local_address);
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
    group.imr_multiaddr.s_addr = inet_addr(HELLO_GROUP);
    group.imr_interface.s_addr = inet_addr((char*) local_address);

    if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
        perror("Adding multicast group error");
        close(sd);
        exit(1);
    }

    struct Tuple r = {sd, groupSock};
    return r;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in groupSock;
    struct sockaddr_in addr;
    int sd;
    char databuf_out[1024] = "Multicast test message lol!";
    int datalen_out = sizeof(databuf_out);
    char databuf_in[1024];
    int datalen_in = sizeof(databuf_in);

    // iniatalise the multicast send and receive socket
    struct Tuple tup = initMulticast(&sd);
    sd = tup.socket;
    groupSock = tup.address;

    while (1){
        // Read from the socket.
        int nbytes=recvfrom(sd,&databuf_in,MSGBUFSIZE,0, (struct sockaddr *) &groupSock,sizeof(groupSock));

        printf("\n");

        if(sendto(sd, databuf_out, datalen_out, 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0) {
                perror("Sending datagram message error");
        } else {
          printf("Sending datagram message...OK\n");
        }
    }

    return 0;
}
