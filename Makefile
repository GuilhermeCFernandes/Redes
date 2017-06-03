all:
	gcc server.c -o server -O3 -lm
	gcc client.c -o client -O3 -lm
	
debug:
	gcc server.c -o server -g -Wall -Wextra -lm
	gcc client.c -o client -g -Wall -Wextra -lm

