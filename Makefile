all:
	gcc server.c -o server -O3
	gcc client.c -o client -O3 -lm

run1:
	./server 7600

run2:
	./client gm-VirtualBox 7600

debug:
	gcc server.c -o server -g -Wall -Wextra
	gcc client.c -o client -g -Wall -Wextra -lm

