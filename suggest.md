# ĐỀ XUẤT MỞ RỘNG - BÀI TẬP LẬP TRÌNH SOCKET

## 1. Hỗ trợ nhiều client đồng thời (Multi-threading)

**Mô tả**: Server hiện tại chỉ phục vụ 1 client tại một thời điểm. Có thể mở rộng bằng cách sử dụng `pthread` hoặc `fork()` để mỗi client được xử lý trong một thread/process riêng.

**Lợi ích**: Server có thể phục vụ nhiều client cùng lúc, tăng hiệu năng.

**Cách thực hiện**:
```c
// Sau khi accept, tạo thread mới để xử lý client
pthread_t thread_id;
pthread_create(&thread_id, NULL, handle_client, (void*)&client_fd);
```

## 2. Hỗ trợ nhiều phép toán

**Mô tả**: Mở rộng từ chỉ tính tổng sang hỗ trợ nhiều phép toán: tổng, hiệu, tích, trung bình cộng, tìm max/min.

**Cách thực hiện**: Server gửi thêm 1 byte mã phép toán trước khi gửi dãy số:
| Mã   | Phép toán        |
|------|------------------|
| 0x01 | Tính tổng        |
| 0x02 | Tính trung bình  |
| 0x03 | Tìm giá trị lớn nhất |
| 0x04 | Tìm giá trị nhỏ nhất |

## 3. Client nhập số thay vì nhận ngẫu nhiên

**Mô tả**: Cho phép client tự nhập danh sách số từ bàn phím, gửi lên server. Server tính tổng và gửi kết quả về.

**Lợi ích**: Tăng tính tương tác, cho phép người dùng kiểm soát dữ liệu.

## 4. Giao thức UDP

**Mô tả**: Thay thế TCP bằng UDP (SOCK_DGRAM) để so sánh:
- Tốc độ truyền
- Độ tin cậy
- Kích thước overhead

**Lợi ích**: Hiểu sự khác biệt giữa TCP và UDP trong thực tế.

## 5. Ghi log (Logging)

**Mô tả**: Server ghi log mỗi phiên làm việc vào file `server.log`:
- Thời gian kết nối
- IP client
- Dãy số đã gửi
- Kết quả client trả lời
- Đúng hay sai

**Lợi ích**: Theo dõi lịch sử hoạt động, hỗ trợ debug.

## 6. Bảo mật cơ bản

**Mô tả**: Thêm cơ chế xác thực đơn giản:
- Client gửi username/password trước khi nhận dãy số
- Server kiểm tra thông tin đăng nhập
- Chỉ cho phép client hợp lệ tham gia

## 7. Giao diện đồ họa (GUI)

**Mô tả**: Xây dựng giao diện đồ họa cho client bằng Qt hoặc GTK, hiển thị dãy số, ô nhập kết quả, và thông báo đúng/sai trực quan.
