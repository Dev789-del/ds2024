#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
int main(int argc, char* argv[]){
    int so;
    char s[200];
    struct sockaddr_in ad;
    FILE *pfile;
    long filelen;

    socklen_t ad_length = sizeof(ad);
    struct hostent *hep;

    //Create socket

    int serv = socket(AF_INET, SOCK_STREAM, 0);

    //Init address
    hep = gethostbyname(argv[0]);
    if (hep == NULL) {
        perror("gethostbyname");
        exit(1);
    }
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr = *(struct in_addr *)hep->h_addr_list[0];
    ad.sin_port = htons(12345);

    //Connect to server
    if (connect(serv, (struct sockaddr *)&ad, ad_length) == -1) {
        perror("connect");
        exit(1);
    }
    //Open sender file and check if it is empty
    pfile = fopen("send.txt", "r");
    if (pfile == NULL) {
        printf("Failed to open file.\n");
        exit(1);
    }
    //Define buffer to read file and send to server
    char buffer[1024] = {0};
    while (fgets(buffer, sizeof(buffer), pfile) != NULL) {
        int i = send(serv, buffer, sizeof(buffer), 0);
        if (i == -1) {
            printf("Send data fail.\n");
            exit(1);
        }
        memset(&buffer, 0, sizeof(buffer));
    }

    close(serv);
    return 0;
}
