#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#include "cpu_usage.h"
#include "mem_info.h"

#define BUFFER 1500

int socket_server(int port){
    int sockfd, new_fd, pid;
    struct sockaddr_in my_addr;
    struct sockaddr their_addr;
    unsigned int sin_size;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1){
        printf("create socket error");
        exit(1);
    }

    memset(&my_addr, '0', sizeof(my_addr));
    memset(&their_addr, '0', sizeof(their_addr));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1){
        printf("bind socket error");
        exit(1);
    }

    if(listen(sockfd, 10)==-1){
        printf("listen");
        exit(1);
    }

    while(1){
        sin_size = sizeof(struct sockaddr);

        printf("server is run %i\n", port);

        if((new_fd = accept(sockfd, &their_addr, &sin_size)) < 0){
            printf("accept");
            exit(1);
        }
        
        pid = fork();

        if(pid == 0){
            printf("fork success %i\n", getpid());

            struct timeval timeout;      
            timeout.tv_sec = 5;

            setsockopt(new_fd, IPPROTO_TCP, SO_RCVTIMEO, 
                    (char *)&timeout, sizeof(timeout));

            setsockopt(new_fd, IPPROTO_TCP, SO_SNDTIMEO, 
                    (char *)&timeout, sizeof(timeout));

            return new_fd;
        }else{
            printf("close socket %i\n", getpid());
            close(new_fd);
        }
    }
    printf("close sockfd %i\n", getpid());
    close(sockfd);
}

int while_interface(int sockfd){
    printf("sockfd %d\n", sockfd);

    uint8_t buffer[BUFFER] = {0};
    intmax_t len;
	fd_set fdset;

	
    while(1){
		FD_ZERO(&fdset);
		FD_SET(sockfd, &fdset);
		
        if (select(sockfd+1, &fdset, NULL, NULL, NULL) < 0){
            printf("select");
            exit(1);
        } 
        
        memset(&buffer, 0, sizeof(buffer));

         if(FD_ISSET(sockfd, &fdset)) {
            len = recv(sockfd, buffer, sizeof(buffer), 0);

            if( len <= 0 ){
                close(sockfd);
                printf("recv error");
                break;
            }

			if (send(sockfd, buffer ,sizeof(buffer),0) < 0){ 
                printf("send error");
                exit(1);
            }
		}
    }
    return 0;
}

void  kill_child(int sig)
{
    int saved_errno = errno;
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
    errno = saved_errno;
}


int main(int argc, char *argv[]) {
	printf("starting\n");

    init();
    printf("%f\n", getCurrentValue());
    sleep(1);
    printf("%f\n", getCurrentValue());

    printf("%f\n", getMemoryUsage());
    printf("%f\n", getMemoryTotal());

    int sockfd;

    signal(SIGCHLD, kill_child);

    sockfd = socket_server(8895);

    if (sockfd > 0)
        while_interface(sockfd);
 
	return 0;
}
