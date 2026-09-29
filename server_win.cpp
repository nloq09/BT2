#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#include <stdint.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAX_NUMBERS 100

int main() {
    WSADATA wsaData;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);

    // Khoi tao Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Loi khoi tao Winsock: %d\n", WSAGetLastError());
        return 1;
    }

    // Tao socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        printf("Loi tao socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Cho phep tai su dung port
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    // Cau hinh dia chi server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Loi bind: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Listen
    if (listen(server_fd, 5) == SOCKET_ERROR) {
        printf("Loi listen: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("=== SERVER TINH TONG ===\n");
    printf("Server dang lang nghe tren port %d...\n\n", PORT);

    srand((unsigned int)time(NULL));

    while (1) {
        // Accept ket noi tu client
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == INVALID_SOCKET) {
            printf("Loi accept: %d\n", WSAGetLastError());
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
        if (send(client_fd, (const char*)&net_N, sizeof(net_N), 0) <= 0) {
            printf("Loi gui N\n");
            closesocket(client_fd);
            continue;
        }

        // Buoc 2: Gui tung so trong danh sach
        printf("  Gui %d so: ", N);
        for (int i = 0; i < N; i++) {
            int32_t net_num = htonl(numbers[i]);
            if (send(client_fd, (const char*)&net_num, sizeof(net_num), 0) <= 0) {
                printf("Loi gui so\n");
                break;
            }
            printf("%d ", numbers[i]);
        }
        printf("\n");
        printf("  Tong dung: %d\n", expected_sum);

        // Buoc 3: Nhan ket qua tong tu client
        int32_t net_client_sum;
        int bytes_received = recv(client_fd, (char*)&net_client_sum, sizeof(net_client_sum), 0);
        if (bytes_received <= 0) {
            printf("Loi nhan ket qua\n");
            closesocket(client_fd);
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

        send(client_fd, result, (int)strlen(result), 0);

        closesocket(client_fd);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
