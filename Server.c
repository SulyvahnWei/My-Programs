#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

int readLine(int fd, void *buf, int max_length);

int main(int argc, char **argv) {
    int socket_fd, client_socket;	
    int length;
    char buf[1024];
    struct sockaddr_in remote_address;
    struct sockaddr_in local_address;
   
    //建立套接字（容错）
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	puts("----------------------");
        puts("Fail to create socket!");
	puts("----------------------");
        return -1;
    }
    
    //绑定地址和端口
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(4321);
    length = sizeof(local_address);
    
    //绑定（容错）
    if (bind(socket_fd, (struct sockaddr *) &local_address, (socklen_t) length) == -1) {
	puts("--------------");
        puts("Fail to bind!");
	puts("--------------");
        return -1;
    }
    
    //建立套接字队列（容错）
    if (listen(socket_fd, 5) == -1) {
        puts("---------------");
        puts("Fail to listen!");
        puts("---------------");
        return -1;
    }
    
    //readLine函数读取一次数据，并打印至命令行窗口，然后再次等待client发送数据，依次循环下去，直至关闭server端
    while (1) {
     	puts("--------------------------");
        puts("Waiting for the client...");
	puts("--------------------------");
        printf("\n");
        fflush(stdout);
        length = sizeof(remote_address);
        client_socket = accept(socket_fd, (struct sockaddr *) 			&remote_address, (socklen_t *) &length);
        readLine(client_socket, (void *) buf, 1024);
       
        //接收数据
        printf("Server read line: %s", buf);
	printf("\n");
      
        //关闭连接
        close(client_socket);
	puts("-------------");
        puts("Close client");
	puts("-------------");
	printf("\n");
    }
}

//readLine函数接受传输过来的data数组中的数据
int readLine(int fd, void *buf, int max_length) {
    int n, ret_val;
    char c, *ptr;
    ptr = buf;
    for (n = 1; n < max_length; n++) {
        again:
        ret_val = (int) recv(fd, &c, 1, 0);
        if (ret_val == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
        } else {
            if (ret_val == 0) {
                if (1 == n)
                    return 0;
            } else {
                if (errno == EINTR)
                    goto again;
                return -1;
            }
        }
    }
    *ptr = 0;
    return n;
}
