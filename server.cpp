#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){

    // IP and port number in order
    if(argc != 2){
        cerr << "Usage: [program] [port_Number] " << endl;
        exit(0);
    }

    // get port number
    int port = atoi(argv[2]); 

    //create message buffer
    char msg[1500];

    //setup a socket and connection tools
    sockaddr_in sendSockAddr;
    bzero((char*) &sendSockAddr, sizeof(sendSockAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    // open stream oriented socket with internet address
    // also keep track of socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0){
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }

    //bind socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if( bindStatus < 0){
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    cout << "Waiting for a client to connect" << endl;

    //listen for 5 requests at a time
    listen(serverSd, 5);

    // receive a request from client using accept
    // Need a new address to connect with client 
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    // to accept, create a new socket descript to handel new connection with client
    int newSd = accept(serverSd, (sockaddr *) &newSockAddr, &newSockAddrSize);
    if(newSd < 0){
        cerr << "Error accepting request from client" << endl;
        exit(1);
    }

    cout << "Connected with client!" << endl;

    // lets keep track of session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    // keep track of amount of data sent as well
    int bytesRead, bytesWritten = 0;
    while(1){
        // receive a message from client (listen)
        cout << "Awaiting client response..." << endl;
        // clear buffer
        memset(&msg, 0, sizeof(msg)); 

        bytesRead += recv(newSd, (char*) &msg, sizeof(msg), 0);
        if( !strcmp(msg, "exit")){
            cout << "Client has quit the session" << endl;
            break;
        }

        cout << "Client: " << msg << endl;
        cout << ">";
        string data;

        getline(cin, data );
        memset(&msg, 0, sizeof(msg) ); // clear the buffer

        strcpy(msg, data.c_str());
        if(data == "exit" ){
            //send to the client that server has closed the connection
            send(newSd, (char*) &msg, strlen(msg), 0);
            break
        }

        //send message to client
        bytesWritten += send(newSd, (char*) &msg, strelen(msg), 0);

    }

    // close the socket descriptors after chatting is done
    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);
    cout << "*****Session*****" << endl;
    cout << "Bytes written: " << bytesWritten << "Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << endl;
    cout << "Connection closed.." << endl; 

    return 0;
 }