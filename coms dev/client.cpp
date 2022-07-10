#include <iostream>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>




//simple programm that at first does dns lookups
int main(int argc, char ** argv){

    

    struct addrinfo hints; //contains hints for dns lookup
    memset(&hints,0,sizeof hints);  //set the memory to zero, important for padding

    struct addrinfo * serviceaddresses; //Poiter to liked list containing addresses (IPv4 or IPv6)
    serviceaddresses = NULL;

    hints.ai_socktype = SOCK_STREAM; //hint it has to be TCP/IP
    hints.ai_family = AF_UNSPEC; //both ipv4 and v6

    //get Linked list with addresses corresponding to hint, service and target
    if(0 != getaddrinfo("www.google.com","http",&hints, &serviceaddresses)){
        std::cout << "Error REEEEEEE" << std::endl;
        exit(1);
    }

    //print out info in linked list
    {
        struct addrinfo * iterator = serviceaddresses;
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
        //dont free because we wannt to connect
        //freeaddrinfo(serviceaddresses); //Free linked list memory

        int Sock; //File descriptors (a socket is a file, are integers)
        
        //create a socket with the information we got from getaddrinfo
        Sock = socket(serviceaddresses->ai_family,serviceaddresses->ai_socktype,serviceaddresses->ai_protocol); 
        if(Sock < 0){ //Socket returns -1 on error
            std::cout << "Socket error";
            exit(3);
        }

        //now we connect
        if(connect(Sock,serviceaddresses->ai_addr,serviceaddresses->ai_addrlen) < 0){
            std::cout << "connection error";
            exit(4);
        }

        char text[2000] = "GET / HTTP/1.1\nHost: www.google.com\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:101.0) Gecko/20100101 Firefox/101.0\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\nAccept-Language: de,en-US;q=0.7,en;q=0.3\nAccept-Encoding: gzip, deflate, br\nConnection: keep-alive\nCookie: AEC=AakniGMRFAzJgEtZbA9zxnAzctENo5xOjDhPqum9oI6kMntlZE-eOo5Kk0U; CONSENT=YES+srp.gws-20220413-0-RC1.de+FX+831; NID=511=exIzMA7d8spGjV72YDco4bXbkUAb69qS_MXJyHHIAruwMehXWefaTQZpMOxhz8gMeGpOSEE7zWjdjFN9hLNFFXoTZCjC4xQfJRi9gZl1VA0sycWRCYPOc6L3YOqse6tNRVJ_DOIKGUgSl2stidL8ENRc4tqvXGhKazJDSjxnG8g; 1P_JAR=2022-06-18-15\nUpgrade-Insecure-Requests: 1\nSec-Fetch-Dest: document\nSec-Fetch-Mode: navigate\nSec-Fetch-Site: none\nSec-Fetch-User: ?1\nSec-GPC: 1\nDNT: 1";
        int len = strlen(text); //get actuall size
        int bytes_send = send(Sock,text,len,0); //returns the ammount of bytes that were actually sent
        if(bytes_send != len){
            std::cout << "Send error";
            exit(5);
        }
        char rec_buffer[1040];
        /*int rec_len = recv(Sock,rec_buffer,1040,0); //recieve answer
        if(rec_len <= 0){
            std::cout << "rec error";
            exit(6);
        }
        std::cout << "Answer  " << std::endl;
        std::cout << rec_buffer << std::endl; */



    }

    //Preparing address info for hosting a socket

    struct addrinfo serverhints; // hints for hosting
    struct addrinfo * serverinfo;

    memset(&serverhints,0,sizeof serverhints); //set memory to all zeros

    serverhints.ai_family = AF_UNSPEC; //We want IPv4
    serverhints.ai_socktype = SOCK_STREAM; // TCP/IP
    serverhints.ai_flags = AI_PASSIVE; // give me the address of this machine

    if(getaddrinfo(NULL,"3187",&serverhints,&serverinfo) != 0){ //again getaddrinfo
                                        //, but we need no addres because we habe AI_Passive flag
        std::cout << "Cant get host setup REEEEEEEEEEEEEE";
        exit(2);
    }
    std::cout << "SERVER STUFF" << std::endl;

    {
        struct addrinfo * iterator = serverinfo;
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
        //freeaddrinfo(serverinfo); //Free linked list memory
        //dont free, we use info to host a socket
        int serv_sock = socket(serverinfo->ai_family,serverinfo->ai_socktype,serverinfo->ai_protocol);
        //because we wannt I certain socket we use bind to get that socket
        if(0 > bind(serv_sock,serverinfo->ai_addr,serverinfo->ai_addrlen)){
            std::cout << "Bind error";
            exit(8);
        }
        //start listening
        if(listen(serv_sock,10) < 0){
            std::cout << "Listen error";
            exit(9);
        }

        //now connections can Que up and we can accept them
    }

    return 0;    
}