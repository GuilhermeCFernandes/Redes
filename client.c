#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define CMD_ALTITUDE	"cmd alt\n"
#define CMD_LONGITUDE   "cmd long\n"
#define CMD_LATITUDE	"cmd lat\n"
#define CMD_VELOCIDADE  "cmd vel\n"
#define CMD_ORIENTACAO  "cmd ori\n"
#define CMD_TEMPO_VOO   "cmd tempo\n"
#define CMD_PESO	"cmd peso\n"

#define PI 3.14159265359
#define MSG_SIZE 256

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

	char msg[MSG_SIZE];
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
	clock_t beginning = time(NULL);
	clock_t tempoVoo;

	while(1){
		//recebendo o comando
		bzero(msg,MSG_SIZE);
		n = read(sockfd,msg,255);
		if (n < 0) error("ERROR reading from socket");

		if(strcmp(msg, "exit\n") == 0) break;
		else if(strcmp(CMD_ALTITUDE, msg) == 0){
			bzero(msg, MSG_SIZE);
			snprintf(msg, MSG_SIZE, "%lf\n", fAltitude((double)(time(NULL)-beginning)));
		}
		else if(strcmp(CMD_LONGITUDE, msg) == 0){
			bzero(msg, MSG_SIZE);
			snprintf(msg, MSG_SIZE, "%lf\n", fLongitude((double)(time(NULL)-beginning)));
		}
		else if(strcmp(CMD_LATITUDE, msg) == 0){
			bzero(msg, MSG_SIZE);
			snprintf(msg, MSG_SIZE, "%lf\n", fLatitude((double)(time(NULL)-beginning)));
		}
		else if(strcmp(CMD_VELOCIDADE, msg) == 0){
			bzero(msg, MSG_SIZE);
			snprintf(msg, MSG_SIZE, "%lf\n", fVelocidade((double)(time(NULL)-beginning)));
		}
		else if(strcmp(CMD_ORIENTACAO, msg) == 0){
			bzero(msg, MSG_SIZE);
			double x, y, z;
			fOrientacao(&x, &y, &z, (double)(time(NULL)-beginning));
			snprintf(msg, MSG_SIZE, "X = %lf Y = %lf Z = %lf\n", x, y, z);
		}
		else if(strcmp(CMD_TEMPO_VOO, msg) == 0){
			bzero(msg, MSG_SIZE);
                        tempoVoo = (time(NULL) - beginning)/60;
			snprintf(msg, MSG_SIZE, "%ld minutos", tempoVoo);
		}
		else if(strcmp(CMD_PESO, msg) == 0){
			bzero(msg, MSG_SIZE);
			snprintf(msg, MSG_SIZE, "%lf\n", fPeso((double)(time(NULL)-beginning)));
		}
		else{
			bzero(msg, MSG_SIZE);
			snprintf(msg, MSG_SIZE, "Comando invalido");
		}

		//enviar a resposta
		n = write(sockfd,msg,strlen(msg));
		if (n < 0) error("ERROR writing to socket");
	}

	close(sockfd);
	return 0;
}

double fAltitude(double t){
	if(t < 28800) return t; //4 horas
	else if(t < 57600) return 11000; //8 horas
	else return (t < 97400 ? 97400-t : 0); //12 horas
}

double fLongitude(double f){
	return 50000*sin(f/10000);
}

double fLatitude(double f){
	return 50000*cos(f/10000);
}

double fVelocidade(double f){
	return 400+5*sin(f)+(rand()%6)-3;
}

void fOrientacao(double *x, double *y, double *z, double f){
	*x = 5*cos(f/10000);
	*y = -5*sin(f/10000);
	if(f < 28800) *z = 1;
	else if(f < 57600) *z = 0;
	else *z = (f < 97400 ? -1 : 0);
}

double fPeso(double f){
	return (20000 - (f/7200)*0.32);
}

