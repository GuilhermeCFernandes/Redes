#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define CMD_ALTITUDE	"cmd alt\n"
#define CMD_LONGITUDE   "cmd long\n"
#define CMD_LATITUDE	"cmd lat\n"
#define CMD_VELOCIDADE  "cmd vel\n"
#define CMD_ORIENTACAO  "cmd ori\n"
#define CMD_TEMPO_VOO   "cmd tempo\n"
#define CMD_PESO	"cmd peso\n"
#define CMD_DIST	"cmd dist\n"
#define CMD_COMB	"cmd comb\n"
#define CMD_PAIS	"cmd pais\n"
#define CMD_TEMPO_ESPERADO "cmd tesperado\n"

#define MSG_SIZE 256

void error(const char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){
	srand(time(NULL));

	int sockfd, newsockfd, portno;
	double combInicial = 1400;
	double pesoInicial = 20000;
	double comb;
	socklen_t clilen;
	char msg[MSG_SIZE];
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
		bzero(msg,MSG_SIZE);
		fgets(msg,255,stdin);

		if(strcmp(msg, CMD_COMB) == 0){
			n = write(newsockfd, CMD_PESO, 255);
			if (n < 0) error("ERROR writing to socket");
		
			n = read(newsockfd,msg,255);
			if (n < 0) error("ERROR reading from socket");
			
			sscanf(msg, "%lf", &comb);
			printf("%lf Kg\n\n", (combInicial - (pesoInicial - comb)));
			
		}else if(strcmp(msg, CMD_DIST) == 0){
			
			double alt, lon, lat;
			n = write(newsockfd, CMD_ALTITUDE, 255);
			if (n < 0) error("ERROR writing to socket");
		
			n = read(newsockfd,msg,255);
			if (n < 0) error("ERROR reading from socket");
			
			sscanf(msg, "%lf", &alt);

			n = write(newsockfd, CMD_LONGITUDE, 255);
			if (n < 0) error("ERROR writing to socket");
		
			n = read(newsockfd,msg,255);
			if (n < 0) error("ERROR reading from socket");
			
			sscanf(msg, "%lf", &lon);
			
			n = write(newsockfd, CMD_LATITUDE, 255);
			if (n < 0) error("ERROR writing to socket");
		
			n = read(newsockfd,msg,255);
			if (n < 0) error("ERROR reading from socket");
			
			sscanf(msg, "%lf", &lat);
			
			double dist = sqrt(pow((alt-rand())/1000, 2)+pow((lon-rand())/10000, 2)+pow((lat-rand())/20000, 2));
			printf("%lf KM\n\n", dist);
	
		}else if(strcmp(msg, CMD_TEMPO_ESPERADO) == 0){
			double t = 0, x = 480;
			
			n = write(newsockfd, CMD_TEMPO_VOO, 255);
			if (n < 0) error("ERROR writing to socket");
		
			n = read(newsockfd,msg,255);
			if (n < 0) error("ERROR reading from socket");
			
			sscanf(msg, "%lf", &t);
		
			printf("%d minutos\n\n", (int) (x - t));
			
			
		}else if(strcmp(msg, CMD_PAIS) == 0){
			n = write(newsockfd, CMD_TEMPO_VOO, 255);
			if (n < 0) error("ERROR writing to socket");
		
			n = read(newsockfd,msg,255);
			if (n < 0) error("ERROR reading from socket");
			
			double t = 0;
			sscanf(msg, "%lf", &t);
			t = t/60;
			
			if(t < 2) printf("A\n\n");
			else if (t < 6) printf("B\n\n");
			else printf("C\n\n");
		
		}else{
			//enviando o comando
			n = write(newsockfd, msg, 255);
			if (n < 0) error("ERROR writing to socket");

			if(strcmp(msg, "exit\n") == 0) break;

			//lendo a resposta
			n = read(newsockfd,msg,255);
			if (n < 0) error("ERROR reading from socket");
			printf("%s\n", msg);
		}
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}

