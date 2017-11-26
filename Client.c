#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int socket_fd;
    int length, ret_val;
    struct sockaddr_in dest_addr;
    char data[1024];

    //建立套接字（容错）
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	puts("-----------------------");
        puts("Fail to create socket!");
	puts("-----------------------");
        return -1;
    }

    //设置套接字的各项参数
    length = sizeof(dest_addr);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(4321);
    dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //尝试连接（容错）
    ret_val = connect(socket_fd, (const struct sockaddr *) &dest_addr, sizeof(struct sockaddr_in));
    if (ret_val == -1) {
	puts("---------------------------");
        puts("Fail to connet the server!");
	puts("---------------------------");
        return -1;
    }

    //输入要发送的数据，支持最大长度为1024
    puts("-------------------------------------------");
    puts("Please enter the message you want to send: ");
    puts("-------------------------------------------");
    fgets(data,1024,stdin);

    //尝试向server端发送数据（容错）
    ret_val = (int) send(socket_fd, (void *) data, strlen(data), 0);
    if (ret_val == -1) {
	puts("--------------------");
        puts("Fail to send data!");
	puts("--------------------");
        goto finish;
    }

    //如果数据被成功发出，则打印出刚刚发送的内容
    printf("\nThe message you have sent is : %s\n", data);
    finish:
    puts("------------");
    puts("Client exit");
    puts("------------");
    close(socket_fd);
    return 0;
}
