Name : Ritesh Chandna
Student ID : 6272516150


Aim of the project:
Implementation of Recursive DNS System

Project Description:
The search term entered by the user is mapped to its corresponding key. This key is then send to Server1, if the key sent is present in Server1.txt , then corresponding value is returned otherwise Server1 requests Server2 for the value. If the key is present is Server2.txt, it returns the correspomding value else Server2 requests Server3 for the value. 


Files:
According to the requirements, we have the following source code files:
1. dhtserver1.c - This is the code for the dhtserver1. It receives the key sent by the Client, if the key is present in its table, it returns the corresponding value else it requests Server2 for the value. 
2. dhtserver2.c - This is the code for dhtserver2. It receives the key sent by the Server1, if the key is present in its table, it returns the corresponding value else it requests Server3 for the value. 
3. dhtserver3.c - This is the code for dhtserver3. It receives the key sent by the Server2 and returns the corresponding value. 
4. client1.c - This is the code for Client1. The search term entered by the user  is mapped by the Client to its corresponding key and then the key is sent to Server1 to get its value.
5. client2.c - This is the code for Client2. The search term entered by the user  is mapped by the Client to its corresponding key and then the key is sent to Server1 to get its value.


To run the program:
The program can be compiled using make file. Just run the command make to compile all the files. The files can also be compiled by the following commands:
The processes must be run in the following order in 5 different windows:
dhtserver1.c > dhtserver2.c > dhtserver3.c > client1.c > client2.c

For compilation :
gcc -o dhtserver1 dhtserver1.c -lsocket -lnsl -lresolv
gcc -o dhtserver2 dhtserver2.c -lsocket -lnsl -lresolv
gcc -o dhtserver3 dhtserver3.c -lsocket -lnsl -lresolv
gcc -o client1 client1.c -lsocket -lnsl -lresolv
gcc -o client2 client2.c -lsocket -lnsl -lresolv

After clean compilation, run the code:
./dhtserver1
./dhtserver2
./dhtserver3
./client1 nunki.usc.edu USC     (The search term is passed as command line argument.If the user doesn't enter the 3 arguments,then the message "Please enter Search term" is displayed )
./client2 nunki.usc.edu USC


Format of Messages:
The messages being exchanged are in string format. In all the messages, <space> or <\n> are the delimiters used to break the string into tokens.


All errors in the code will be handled by the perror() function.


Beej's Guide to Network Programming has been the basis on which the code is built. The code has been reused for implementing many functions. Such functions have also been marked in the code.
