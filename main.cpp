#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>

#define SIZE 3

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void GetMessage(uint16_t len, int sockfd, char* message ) {
    char buff [len];
    strcpy(buff, message);
    int n = write(sockfd,buff, len);
    if (n < 0)
    {
        error("ERROR writing to socket");
    }
    bzero(buff, len);
    n = read(sockfd, buff, len);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }

    printf("%s\n", buff);

}

void GetDateTime(int sockfd) {
    char buff[11];
    bzero(buff, 11);

    int n = read(sockfd, buff, 11);
    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("%s\n", buff);
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[SIZE]; //because the largest length, which can be decoded in 2 bytes of memory is 65536
    // + 4 (for a flag, size and '\0')

    setbuf(stdout, 0);

    portno = 12347;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    const char* ip = argv[1];
    //const  char* ip = "192.168.1.10";
    if ( inet_aton(ip, &serv_addr.sin_addr) < 0)
    {
        error("Incorrect IP");
    }
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }

    bzero(buffer, SIZE);

    std::string flag;

    flag = argv[2];
    buffer[0] = flag[0];
    n = write(sockfd, buffer, 1);
    if (n < 0)
    {
        error("ERROR writing to socket");
    }

    if (flag == "m")
    {
        char* message;

        message = argv[3];
        uint16_t len = strlen(message) + 1;

        memcpy(buffer + 1, &len, sizeof(uint16_t));
        n = write(sockfd,buffer + 1, 2);
        if (n < 0)
        {
            error("ERROR writing to socket");
        }

        GetMessage(len, sockfd, message);


    }

    else
    {
        GetDateTime(sockfd);
    }

    close(sockfd);

    return 0;
}
