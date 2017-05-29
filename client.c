#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define ALTITUDE    "cmd alt"
#define LONGITUDE   "cmd long"
#define LATITUDE    "cmd lat"
#define VELOCIDADE  "cmd vel"
#define ORIENTACAO  "cmd ori"
#define TEMPO_VOO   "cmd time"
#define PESO        "cmd peso"

void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char msg[256];
    if (argc < 3){
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL){
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR connecting");
    //conectado

	while(1){
        //recebendo o comando
        bzero(msg,256);
        n = read(sockfd,msg,255);
        if (n < 0) error("ERROR reading from socket");

        //TODO decidir a resposta
        bzero(msg,256);

        //enviar a resposta
        n = write(sockfd,msg,strlen(msg));
        if (n < 0) error("ERROR writing to socket");
	}

    close(sockfd);
    return 0;
}
