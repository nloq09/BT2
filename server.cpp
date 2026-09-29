#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 8080
#define MAX_NUMBERS 100

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Tao socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Loi tao socket");
        exit(1);
    }

    // Cho phep tai su dung port
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Cau hinh dia chi server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Loi bind");
        close(server_fd);
        exit(1);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("Loi listen");
        close(server_fd);
        exit(1);
    }

    printf("=== SERVER TINH TONG ===\n");
    printf("Server dang lang nghe tren port %d...\n\n", PORT);

    srand(time(NULL));

    while (1) {
        // Accept ket noi tu client
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Loi accept");
            continue;
        }

        printf("Client ket noi: %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port));

        // Tao danh sach N so ngau nhien
        int N = rand() % 10 + 3;  // N tu 3 den 12
        int numbers[MAX_NUMBERS];
        int expected_sum = 0;

        for (int i = 0; i < N; i++) {
            numbers[i] = rand() % 100 + 1;  // So tu 1 den 100
            expected_sum += numbers[i];
        }

        // Buoc 1: Gui so luong phan tu N
        int32_t net_N = htonl(N);
        if (send(client_fd, &net_N, sizeof(net_N), 0) <= 0) {
            perror("Loi gui N");
            close(client_fd);
            continue;
        }

        // Buoc 2: Gui tung so trong danh sach
        printf("  Gui %d so: ", N);
        for (int i = 0; i < N; i++) {
            int32_t net_num = htonl(numbers[i]);
            if (send(client_fd, &net_num, sizeof(net_num), 0) <= 0) {
                perror("Loi gui so");
                break;
            }
            printf("%d ", numbers[i]);
        }
        printf("\n");
        printf("  Tong dung: %d\n", expected_sum);

        // Buoc 3: Nhan ket qua tong tu client
        int32_t net_client_sum;
        int bytes_received = recv(client_fd, &net_client_sum, sizeof(net_client_sum), 0);
        if (bytes_received <= 0) {
            perror("Loi nhan ket qua");
            close(client_fd);
            continue;
        }
        int client_sum = ntohl(net_client_sum);
        printf("  Client tra loi: %d\n", client_sum);

        // Buoc 4: Kiem tra va gui ket qua DUNG/SAI
        char result[10];
        if (client_sum == expected_sum) {
            strcpy(result, "DUNG");
            printf("  >> KET QUA: DUNG!\n\n");
        } else {
            strcpy(result, "SAI");
            printf("  >> KET QUA: SAI! (Dung la %d, client gui %d)\n\n", 
                   expected_sum, client_sum);
        }

        send(client_fd, result, strlen(result), 0);

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
