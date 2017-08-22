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

#include <stdio.h>

#include "turntable.c"
#include "network.c"

#define MSGBUFSIZE 256

int main(int argc, char *argv[])
{
    struct sockaddr_in groupSock;
    struct sockaddr_in addr;
    int sd;
    char databuf_out[MSGBUFSIZE] = "Multicast test message lol!";
    int datalen_out = sizeof(databuf_out);
    char databuf_in[MSGBUFSIZE];
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
        }
    }

    return 0;
}
