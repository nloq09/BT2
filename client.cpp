#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define MAX_NUMBERS 100

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;

    // Tao socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Loi tao socket");
        exit(1);
    }

    // Cau hinh dia chi server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Ket noi den server
    printf("=== CLIENT TINH TONG ===\n");
    printf("Dang ket noi den server %s:%d...\n", SERVER_IP, PORT);

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Loi ket noi");
        close(sock_fd);
        exit(1);
    }
    printf("Ket noi thanh cong!\n\n");

    // Buoc 1: Nhan so luong phan tu N tu server
    int32_t net_N;
    int bytes_received = recv(sock_fd, &net_N, sizeof(net_N), 0);
    if (bytes_received <= 0) {
        perror("Loi nhan N");
        close(sock_fd);
        exit(1);
    }
    int N = ntohl(net_N);
    printf("Nhan duoc %d so tu server:\n", N);

    // Buoc 2: Nhan tung so va tinh tong
    int numbers[MAX_NUMBERS];
    int sum = 0;

    for (int i = 0; i < N; i++) {
        int32_t net_num;
        bytes_received = recv(sock_fd, &net_num, sizeof(net_num), 0);
        if (bytes_received <= 0) {
            perror("Loi nhan so");
            close(sock_fd);
            exit(1);
        }
        numbers[i] = ntohl(net_num);
        sum += numbers[i];
        printf("  So thu %d: %d\n", i + 1, numbers[i]);
    }

    printf("\nTong tinh duoc: %d\n", sum);

    // Buoc 3: Gui ket qua tong ve server
    int32_t net_sum = htonl(sum);
    if (send(sock_fd, &net_sum, sizeof(net_sum), 0) <= 0) {
        perror("Loi gui ket qua");
        close(sock_fd);
        exit(1);
    }
    printf("Da gui tong ve server.\n");

    // Buoc 4: Nhan ket qua DUNG/SAI tu server
    char result[10];
    memset(result, 0, sizeof(result));
    bytes_received = recv(sock_fd, result, sizeof(result) - 1, 0);
    if (bytes_received <= 0) {
        perror("Loi nhan ket qua");
        close(sock_fd);
        exit(1);
    }

    printf("\n=============================\n");
    printf("  Server phan hoi: %s\n", result);
    printf("=============================\n");

    close(sock_fd);
    return 0;
}
