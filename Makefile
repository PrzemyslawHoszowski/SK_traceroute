init:
	gcc -std=gnu17 -c -Wall -Wextra src/utils.c -o utils.o
	gcc -std=gnu17 -c -Wall -Wextra src/socket.c -o socket.o
	gcc -std=gnu17 -c -Wall -Wextra src/packet.c -o packet.o
	gcc -std=gnu17 -c -Wall -Wextra main.c -o main.o
	gcc -o traceroute -Wall -Wextra main.o packet.o socket.o utils.o
clear:
	rm *.o

distclean:
	rm *.o -f
	rm traceroute -f