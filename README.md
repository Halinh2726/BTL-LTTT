# BTL-LTTT
Đây là bài tập lớn môn Lý thuyết thông tin

# Kiểm tra Tính Trực Giao Đầy Đủ của Mã Vòng Tuyến Tính

Dự án này cung cấp một bộ thư viện lập trình để xử lý các đối tượng toán học trong lý thuyết mã hóa kênh, cụ thể là trên trường hữu hạn $GF(2)$. Mục tiêu chính là kiểm tra xem một mã vòng tuyến tính $C(i, k, d_0)$ cho trước có khả năng trực giao đầy đủ hay không dựa trên đa thức kiểm tra $h(x)$.

## Cấu trúc Thành phần

Hệ thống được thiết kế theo hướng đối tượng với các lớp cơ bản sau:

### 1. Lớp `Bit`
- Quản lý các đơn vị dữ liệu nhị phân (0 và 1).
- Định nghĩa các phép toán cơ bản: `AND`, `OR`, `XOR` (tương ứng với phép cộng trên $GF(2)$).

### 2. Lớp `BinaryVector` (Vector Nhị Phân)
- Đại diện cho các từ mã (codewords).
- Hỗ trợ các phép toán vector: cộng vector, tích vô hướng, dịch vòng (cyclic shift).
- Lưu trữ và thao tác trên danh sách các đối tượng `Bit`.

### 3. Lớp `PolynomialGF2` (Đa thức trên $GF(2)$)
- Xử lý các đa thức có hệ số thuộc trường nhị phân.
- Các phép toán quan trọng:
  - Cộng/Trừ đa thức (giống nhau trên $GF(2)$).
  - Nhân đa thức.
  - Chia đa thức (tìm dư và thương) - Rất quan trọng để kiểm tra tính chia hết của $x^n - 1$ cho $g(x)$ hoặc $h(x)$.

### 4. Helper Classes
- Các công cụ hỗ trợ đọc dữ liệu từ file, định dạng đầu ra và các hàm tiện ích toán học khác.

## Chức năng Chính

Chương trình tập trung vào bài toán:
**Xét mã vòng tuyến tính $C(i, k, d_0)$ có đa thức kiểm tra $h(x)$ cho trước. Kiểm tra xem $C$ có phải mã vòng tuyến tính có khả năng trực giao đầy đủ?**

### Thuật toán kiểm tra
1. Xác định tham số $n, k$ từ đa thức $h(x)$.
2. Xây dựng ma trận kiểm tra hoặc tập hợp các phương trình kiểm tra từ $h(x)$.
3. Tìm hệ thống các phương trình trực giao đối với từng ký hiệu thông tin.
4. Kết luận về khả năng giải mã đa số (Majority Logic Decoding) dựa trên tính trực giao đầy đủ.

## Định dạng Dữ liệu Đầu vào (Input)

Chương trình nhận dữ liệu đầu vào trực tiếp từ Standard Input (bàn phím) hoặc thông qua chuyển hướng file (file redirection). Cấu trúc dữ liệu bao gồm 2 dòng cơ bản:

1. **Dòng 1:** Chứa 3 số nguyên $l, k, d_0$ phân tách nhau bởi khoảng trắng.
   - $l$: Chiều dài từ mã (Block length).
   - $k$: Chiều dài chuỗi thông tin (Message length).
   - $d_0$: Khoảng cách thiết kế (Design distance).
2. **Dòng 2:** Chứa $k + 1$ số nguyên ($0$ hoặc $1$), biểu diễn các hệ số của đa thức kiểm tra $h(x)$. Các hệ số được nhập theo thứ tự bậc tăng dần, từ $x^0$ đến $x^k$.

## Hướng dẫn Biên dịch và Thực thi

Do hệ thống mã nguồn sử dụng một số cấu trúc dữ liệu chuẩn như `std::vector` và kiểu `long long` để chống tràn bit (tránh bùng nổ không gian đối ngẫu khi $r = l - k$ lớn), trình biên dịch cần hỗ trợ chuẩn **C++11** trở lên.

### 1. Biên dịch (Build)
Mở Terminal/Command Prompt tại thư mục chứa mã nguồn và thực thi lệnh GCC cơ bản sau:
~~~bash
g++ -std=c++11 main.cpp -o CyclicDecoder
~~~

### 2. Thực thi (Run)
Bạn có thể chạy trực tiếp và nhập tham số bằng tay:
~~~bash
# Trên Linux/macOS
./CyclicDecoder

# Trên Windows
CyclicDecoder.exe
~~~
Hoặc chạy tự động với file dữ liệu đã chuẩn bị sẵn (ví dụ `input.txt`):
~~~bash
./CyclicDecoder < input.txt
~~~

## Kết quả xuất ra (Output)
Tùy thuộc vào mã vòng được nhập, hệ thống sẽ in ra 1 trong 2 thông báo kết luận sau:
- **MÃ TRỰC GIAO ĐẦY ĐỦ:** `=> KET LUAN: Ma vong CO kha nang truc giao day du [pos]` (Hệ thống dừng sớm - Early Exit ngay khi tìm thấy phương trình trực giao tại vị trí `pos`).
- **MÃ KHÔNG TRỰC GIAO ĐẦY ĐỦ:** `=> KET LUAN: Ma vong KHONG co kha nang truc giao day du.` (Duyệt hết không gian nhưng không thỏa mãn điều kiện).

### Ví dụ Minh họa
Khảo sát một mã vòng có chiều dài $l = 7$, số bit thông tin $k = 4$, khoảng cách thiết kế $d_0 = 3$. Đa thức kiểm tra tương ứng là $h(x) = 1 + x + x^3 + x^4$.

**Input:**
~~~text
7 4 3
1 1 0 1 1
~~~

**Output:**
~~~text
=> KET LUAN: Ma vong CO kha nang truc giao day du 0
~~~

## Cơ chế Xử lý Ngoại lệ (Validation)

Chương trình được tích hợp sẵn các lớp bảo vệ ở hàm `main()` để kiểm tra tính hợp lệ của bài toán Lý thuyết mã hóa. Dưới đây là 3 thông báo lỗi (`cout`) tương ứng với 3 trường hợp nhập sai:

1. **Lỗi đa thức rỗng:** - `[LOI] h(x) la da thuc toan 0` 
   - (Xảy ra nếu người dùng nhập toàn số $0$ cho hệ số của $h(x)$).
2. **Lỗi sai bậc đa thức:** - `[LOI] Bac cua h(x) phai bang k = [k]. Bac hien tai: [bac_hien_tai]` 
   - (Xảy ra nếu hệ số lớn nhất của bậc $x^k$ không phải là $1$).
3. **Lỗi không sinh ra mã vòng:** - `[LOI] (x^l + 1) khong chia het cho h(x). Ma khong hop le!` 
   - (Xác thực điều kiện cốt lõi $(x^l + 1) \pmod{h(x)} = 0$ trên $GF(2)$. Nếu $h(x)$ không phải là ước của $x^l - 1$, hệ thống sẽ từ chối thực thi).

---
*Dự án phục vụ mục đích học tập và nghiên cứu về Lý thuyết Mã hóa.*
