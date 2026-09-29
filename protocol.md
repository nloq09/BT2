# MÔ TẢ GIAO THỨC - BÀI TẬP LẬP TRÌNH SOCKET

## 1. Tổng quan

Giao thức Client-Server tính tổng dãy số sử dụng TCP Socket.

- **Giao thức truyền tải**: TCP (SOCK_STREAM)
- **Port**: 8080
- **Địa chỉ**: 127.0.0.1 (localhost)
- **Byte order**: Network byte order (Big-Endian), sử dụng htonl/ntohl

## 2. Mô tả luồng giao thức

```
  CLIENT                                    SERVER
    |                                         |
    |-------- TCP Connect (SYN) ------------>|
    |<------- TCP Accept (SYN-ACK) ----------|
    |                                         |
    |   Bước 1: Server gửi số lượng phần tử  |
    |<-------- N (int32, 4 bytes) ------------|
    |                                         |
    |   Bước 2: Server gửi N số nguyên       |
    |<-------- num[0] (int32, 4 bytes) -------|
    |<-------- num[1] (int32, 4 bytes) -------|
    |<-------- ...                            |
    |<-------- num[N-1] (int32, 4 bytes) -----|
    |                                         |
    |   Bước 3: Client tính tổng và gửi lại  |
    |--------- SUM (int32, 4 bytes) -------->|
    |                                         |
    |   Bước 4: Server kiểm tra, gửi kết quả |
    |<-------- "DUNG" hoặc "SAI" (chuỗi) ----|
    |                                         |
    |-------- TCP Close (FIN) -------------->|
    |                                         |
```

## 3. Chi tiết các bước

### Bước 1: Server gửi N
| Trường   | Kiểu    | Kích thước | Mô tả                          |
|----------|---------|------------|---------------------------------|
| N        | int32_t | 4 bytes    | Số lượng phần tử (3 ≤ N ≤ 12)  |

### Bước 2: Server gửi dãy số
| Trường      | Kiểu    | Kích thước | Mô tả                        |
|-------------|---------|------------|-------------------------------|
| numbers[i]  | int32_t | 4 bytes    | Số nguyên thứ i (1 ≤ num ≤ 100) |

Gửi N lần liên tiếp, mỗi lần 4 bytes.

### Bước 3: Client gửi tổng
| Trường | Kiểu    | Kích thước | Mô tả                    |
|--------|---------|------------|---------------------------|
| SUM    | int32_t | 4 bytes    | Tổng các số client tính   |

### Bước 4: Server gửi kết quả
| Trường  | Kiểu   | Kích thước  | Mô tả                              |
|---------|--------|-------------|-------------------------------------|
| result  | string | 3-4 bytes   | "DUNG" (4 bytes) hoặc "SAI" (3 bytes) |

## 4. Xử lý lỗi

- Nếu kết nối thất bại: in thông báo lỗi và thoát
- Nếu gửi/nhận dữ liệu thất bại: đóng socket và thoát
- Server tiếp tục chạy sau khi phục vụ xong 1 client

## 5. Biên dịch và chạy

```bash
# Biên dịch
gcc server.cpp -o server
gcc client.cpp -o client

# Chạy server trước
./server

# Mở terminal khác, chạy client
./client
```
