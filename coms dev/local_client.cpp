#include <iostream>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char ** argv){
    struct addrinfo hints;  //holds hints
    memset(&hints,0,sizeof hints);
    struct addrinfo * server_address;   //for returned addressinfo

    int status; //To check if functions were executed successfull

    hints.ai_family = AF_INET; //we wanna host IPv4
    hints.ai_socktype = SOCK_STREAM; //we wannt TCP/IP
    
    status = getaddrinfo("localhost","3187",&hints,&server_address);
    if(status != 0){
        fprintf(stderr,"gettaddrinfo error: %s\n", gai_strerror(status)); //print standard error
        exit(1);
    }

    int sock = socket(server_address->ai_family,server_address->ai_socktype,server_address->ai_protocol);
    if(sock < 0){
        std::cout << "Error at socket creation";
        exit(2);
    }
   status = connect(sock,server_address->ai_addr,server_address->ai_addrlen);
    if(status != 0){
        std::cout << "Error at connecting";
        exit(3);
    }
    char text[1040] = "Hewwow Server uwu";
    int bytes_send; //holdy bytes that were actually sent
    bytes_send = send(sock,text,strlen(text),0);
    if(bytes_send == 0){
        std::cout << "Error at sending";
        exit(3);
    }else if(bytes_send != strlen(text)){
        std::cout << "Didnt sent all";
        exit(4);
    }
    std::cout << text << std::endl;

    bytes_send = recv(sock,text,1040,0); //recieve msg

    std::cout << text << std::endl; 

}