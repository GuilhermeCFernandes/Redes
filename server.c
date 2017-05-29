/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define CMD_ALTITUDE    "cmd alt"
#define CMD_LONGITUDE   "cmd long"
#define CMD_LATITUDE    "cmd lat"
#define CMD_VELOCIDADE  "cmd vel"
#define CMD_ORIENTACAO  "cmd ori"
#define CMD_TEMPO_VOO   "cmd time"
#define CMD_PESO        "cmd peso"
#define CMD_DIST        "cmd dist"

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    srand(time(NULL));

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char msg[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");

    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");

    while(1){
        //lendo o comando
        bzero(msg,256);
        fgets(msg,255,stdin);
        if(strcmp(msg, "exit") == 0) break;

        //enviando o comando
        n = write(newsockfd, msg, 255);
        if (n < 0) error("ERROR writing to socket");

        //lendo a resposta
		n = read(newsockfd,msg,255);
		if (n < 0) error("ERROR reading from socket");

		//TODO fazer as operacoes de sensores virtuais
		printf("%s\n", msg);
    }
     close(newsockfd);
     close(sockfd);
     return 0;
}
