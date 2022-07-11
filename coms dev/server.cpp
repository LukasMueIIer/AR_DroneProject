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

    int status;     //Integer that holds error results

    struct addrinfo hints;  //contains hints for getaddrinfo

    memset(&hints,0,sizeof hints); //Set memmory to 0

    hints.ai_family = AF_INET; //we wanna host IPv4
    hints.ai_socktype = SOCK_STREAM; //we wannt TCP/IP
    hints.ai_flags = AI_PASSIVE;    //get our own IP address

    struct addrinfo * hostinfo;     //Pointer to linked list containing server info

    status = getaddrinfo(NULL,"3188",&hints,&hostinfo); //get address info

    if(status != 0){    //error occured
        fprintf(stderr,"gettaddrinfo error: %s\n", gai_strerror(status)); //print standard error
        exit(1);
    }

    struct addrinfo * iterator = hostinfo;
        char ip[INET6_ADDRSTRLEN]; //String which can hold a full size IPv6 address
        while (iterator != NULL)
        {

            if(iterator->ai_family == AF_INET){ //IPv4
                std::cout << "IPv4: ";
                 //convert IP to readable
                inet_ntop(AF_INET, &(((struct sockaddr_in*)iterator->ai_addr)->sin_addr),ip,INET6_ADDRSTRLEN);
                //first we cast the sockaddr pointer into an IPv4 sockadress then we acces the addr element
                std::cout << ip << std::endl;
            }else{
                std::cout << "IPv6: ";
                inet_ntop(AF_INET6, &(((struct sockaddr_in6*)iterator->ai_addr)->sin6_addr),ip,INET6_ADDRSTRLEN);
                //same as above but we cast into IPv6 sockadress and excess an addr element with diffrent name
                std::cout << ip << std::endl;
            }
           
            iterator = iterator->ai_next; //go to next element

        }

    //get socket descriptor
    int server_sock = socket(hostinfo->ai_family,hostinfo->ai_socktype,hostinfo->ai_protocol);
    
    status = bind(server_sock,hostinfo->ai_addr,hostinfo->ai_addrlen);
    if(status != 0){    //error occured
        fprintf(stderr,"bind error: %s\n", gai_strerror(status)); //print standard error
        exit(1);
    }

    status = listen(server_sock,10); //listen on socket with max 10 queed
    if(status != 0){    //error occured
        fprintf(stderr,"listen error: %s\n", gai_strerror(status)); //print standard error
        exit(1);
    }
    struct sockaddr_storage client_addr;    //this struct can store IPv4 and IPv6, just cast to the 
                                            //right one
    socklen_t addr_size; //wierd but its just set to the max size of the clienat addr struct
    addr_size = sizeof client_addr;

    int client = accept(server_sock,(struct sockaddr *) &client_addr,&addr_size);

    char rec_buffer[1024];

    int bytes_rec = recv(client,rec_buffer,1024,0); //recieve string from a connected client

    std::cout << rec_buffer;
    

    char test[123] = "HEWWOWWWW";
    memset(test,sizeof(char)*123);

    send(client,test,strlen(test),0);

  //  close(client);
  //  close(server_sock);
}