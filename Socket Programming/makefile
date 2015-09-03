all: dhtserver1 dhtserver2 dhtserver3 client1 client2

dhtserver1: dhtserver1.c
	gcc -g -o dhtserver1 dhtserver1.c -lsocket -lnsl -lresolv

dhtserver2: dhtserver2.c
	gcc -g -o dhtserver2 dhtserver2.c -lsocket -lnsl -lresolv

dhtserver3: dhtserver3.c
	gcc -g -o dhtserver3 dhtserver3.c -lsocket -lnsl -lresolv

client1: client1.c
	gcc -g -o client1 client1.c -lsocket -lnsl -lresolv

client2: client2.c
	gcc -g -o client2 client2.c -lsocket -lnsl -lresolv
	
clean:
	  rm dhtserver1
	  rm dhtserver2
	  rm dhtserver3
	  rm client1
	  rm client2
	  

	  

