
void error(const char *msg ) {
    perror(  msg );
    exit(1);
}

void sendData(const int sockfd, const int x ) {
    int n;

    char buffer[32];
    sprintf( buffer, "%d\n", x );
    if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 ){
        error("ERROR writing to socket");
    }
    buffer[n] = '\0';
}

int getData(const int sockfd ) {
    char buffer[32];
    int n;

    if ( (n = read(sockfd,buffer,31) ) < 0 ) {
        error("ERROR reading from socket");
    }
    buffer[n] = '\0';
    return atoi( buffer );
}

int connectServer(const int argc, const char serverIp[], const int portno){
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

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

    bzero( (char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR connecting");
    }

    return sockfd;
}
