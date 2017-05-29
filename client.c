#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define CMD_ALTITUDE    "cmd alt"
#define CMD_LONGITUDE   "cmd long"
#define CMD_LATITUDE    "cmd lat"
#define CMD_VELOCIDADE  "cmd vel"
#define CMD_ORIENTACAO  "cmd ori"
#define CMD_TEMPO_VOO   "cmd time"
#define CMD_PESO        "cmd peso"

double fAltitude(double t);
double fLongitude(double t);
double fLatitude(double t);
double fVelocidade(double t);
void   fOrientacao(double* x, double* y, double* z, double t);
double fPeso(double t);

void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    srand(time(NULL));

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

    clock_t beginning = clock();

	while(1){
        //recebendo o comando
        bzero(msg,256);
        n = read(sockfd,msg,255);
        if (n < 0) error("ERROR reading from socket");

        if(strcmp(CMD_ALTITUDE, msg) == 0){
            bzero(msg,256);
            sscanf(msg, "%lf", fAltitude((clock()-beginning)/CLOCKS_PER_SEC));
        }
        else if(strcmp(CMD_LONGITUDE, msg) == 0){

        }
        else if(strcmp(CMD_LATITUDE, msg) == 0){

        }
        else if(strcmp(CMD_VELOCIDADE, msg) == 0){

        }
        else if(strcmp(CMD_ORIENTACAO, msg) == 0){

        }
        else if(strcmp(CMD_TEMPO_VOO, msg) == 0){

        }
        else if(strcmp(CMD_PESO, msg) == 0){

        }

        //enviar a resposta
        n = write(sockfd,msg,strlen(msg));
        if (n < 0) error("ERROR writing to socket");
	}

    close(sockfd);
    return 0;
}

double fAltitude(double t){
    if(t < 4*7200) return t; //da pra mudar pra uma parabola e tals, mas whatever
    else if(t < 8*7200) return 11000;
    else return (t < 11000 ? 11000-t : 0);
}

