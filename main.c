/* A simple server in the internet domain using TCP.
Server.cD. Thiebaut
Adapted from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
The port number used in 51717.
This code is compiled and run on the Raspberry as follows:

    g++ -o turntable main.c network.c turntable.c -lwiringPi -lwiringPiDev
    ./turntable

The server waits for a connection request from a client.
The server assumes the client will send positive integers, which it sends back multiplied by 2.
If the server receives -1 it closes the socket with the client.
If the server receives -2, it exits.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "network.c"
#include "turntable.c"

//#define h_addr h_addr_list[0] /* uncomment if an error for backwards compatability */

const char serverIp[] = "169.254.17.24";
const int portno = 51717;

int func( int a ) {
   return 2 * a;
}

int main(int argc, char *argv[]) {
    int sockfd=0, n;
    //struct sockaddr_in serv_addr;
    //struct hostent *server;
    char buffer[256];
    int data;
    time_t start, end;
    double diff = 0;

    while(1){
        time(&start);

        // connect to the server
        if(sockfd == 0){
            sockfd = connectServer(argc, serverIp, portno);
            //send a connect request to the server
            sendData( sockfd, 1.0 );
        }

        // retreive data from the server
        data = getData( sockfd );
        printf("%d ->  %d\n",n, data );
        // check if the turntable needs to perform an action
        if(data == 3){
            //perform motor movement
            int ack = spinMotor(45);
            sleep(2);
            sendData( sockfd, ack );
        }else{
            sendData( sockfd, 0 );
        }

        while(diff < 1.0){
            time(&end);
            diff = difftime(end, start);
            //printf("diff %d\n", diff);
        }
        diff = 0;

        // close the socket when done
        //sendData(sockfd, -2);
        //close(sockfd);
    }
    return 0;
}

