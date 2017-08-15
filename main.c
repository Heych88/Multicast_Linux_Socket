/* A simple server in the internet domain using TCP.
Server.cD. Thiebaut
Adapted from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
The port number used in 51717.
This code is compiled and run on the Raspberry as follows:

    g++ -o myServer myServer.c
    ./myServer

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

//#define h_addr h_addr_list[0] /* uncomment if an error for backwards compatability */

const char serverIp[] = "169.254.17.24";
const int portno = 51717;

void error( char *msg ) {
  perror(  msg );
  exit(1);
}

int func( int a ) {
   return 2 * a;
}

void sendData( int sockfd, int x ) {
    int n;

    char buffer[32];
    sprintf( buffer, "%d\n", x );
    if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 ){
        error("ERROR writing to socket");
    }
    buffer[n] = '\0';
}

int getData( int sockfd ) {
    char buffer[32];
    int n;

    if ( (n = read(sockfd,buffer,31) ) < 0 ) {
        error("ERROR reading from socket");
    }
    buffer[n] = '\0';
    return atoi( buffer );
}

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    int data;
    time_t start, end;
    double diff = 0;

    while(1){
        time(&start);
        if (argc < 3) {
          // error( const_cast<char *>( "usage myClient2 hostname port\n" ) );
          printf( "contacting %s on port %d\n", serverIp, portno );
          // exit(0);
        }
        if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
            error("ERROR opening socket");
        }
        if ( ( server = gethostbyname( serverIp ) ) == NULL ){
            error("ERROR, no such host\n");
        }
        printf("sockfd: %d\n", sockfd);

        bzero( (char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(portno);
        if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
            error("ERROR connecting");
        }

        for ( n = 0; n < 10; n++ ) {
          sendData( sockfd, n );
          data = getData( sockfd );
          printf("%d ->  %d\n",n, data );
        }
        sendData( sockfd, -2 );

        while(diff < 1.0){
            time(&end);
            diff = difftime(end, start);
            //printf("diff %d\n", diff);
        }
        diff = 0;
        sendData(sockfd, -2);
        close(sockfd);
    }


    return 0;
}

