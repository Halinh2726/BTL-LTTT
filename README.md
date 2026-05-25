# BTL-LTTT

## Kiểm tra Tính Trực Giao Đầy Đủ của Mã Vòng Tuyến Tính trên GF(2)

Dự án này xây dựng một hệ thống mô phỏng và kiểm tra mã vòng tuyến tính (Cyclic Linear Codes) trên trường hữu hạn GF(2), phục vụ cho học tập và nghiên cứu môn Lý thuyết thông tin và Mã hóa kênh.

Chương trình được viết bằng ngôn ngữ C++ theo hướng lập trình hướng đối tượng (OOP), cho phép:

* Xây dựng và thao tác trên các đối tượng toán học của mã hóa kênh.
* Sinh ma trận kiểm tra H.
* Sinh không gian mã đối ngẫu (Dual Codewords).
* Tìm hệ phương trình trực giao độc lập.
* Kiểm tra khả năng trực giao đầy đủ.
* Mô phỏng thuật toán Majority Logic Decoding.
* Tương tác với người dùng bằng hệ thống Quiz mô phỏng giải mã.

---

# 1. Tổng quan Lý thuyết

Xét mã vòng tuyến tính:

C(l, k, d₀)

Trong đó:

* l: chiều dài từ mã.
* k: số bit thông tin.
* d₀: khoảng cách thiết kế.

Mã vòng được đặc trưng bởi đa thức kiểm tra:

h(x)

Chương trình kiểm tra xem mã có:

* khả năng trực giao đầy đủ,
* và có thể áp dụng Majority Logic Decoding hay không.

---

# 2. Kiến trúc Hệ thống

Hệ thống được xây dựng theo mô hình OOP với các lớp sau.

---

# 2.1. Lớp Bit

Quản lý bit nhị phân trong trường GF(2).

## Chức năng

* Phép cộng XOR
* Phép nhân AND
* Phép OR logic

## Các phép toán

Trong GF(2):

a + b = XOR

a * b = AND

Ví dụ:

1 + 1 = 0

1 * 1 = 1

---

# 2.2. Lớp BinaryVector

Đại diện cho vector nhị phân (codeword).

Ví dụ:

1011010

## Chức năng

* Cộng vector trên GF(2)
* So sánh vector
* Dịch vòng phải
* Dịch vòng trái
* Kiểm tra vector 0
* Tính tích vô hướng (dot product)

## Các phép toán hỗ trợ

### Cộng vector

v₁ + v₂

### Dịch vòng

Cyclic Shift Left / Right

### Tích vô hướng

v₁ · v₂

được sử dụng trong thuật toán giải mã đa số.

---

# 2.3. Lớp Polynomial

Đại diện cho đa thức trên GF(2).

Ví dụ:

h(x) = 1 + x + x³

được lưu dưới dạng:

[1, 1, 0, 1]

## Các chức năng

* Cộng đa thức
* Nhân đa thức
* Chia đa thức
* Tính phần dư modulo
* Kiểm tra đa thức 0
* Sinh đa thức:

x^l + 1

## Các phép toán

### Cộng đa thức

Trong GF(2):

1 + 1 = 0

nên phép cộng và phép trừ là giống nhau.

### Nhân đa thức

Sử dụng quy tắc nhân thông thường nhưng modulo 2.

### Chia đa thức

Dùng để kiểm tra:

(x^l + 1) mod h(x)

Nếu phần dư bằng 0:

=> h(x) là đa thức hợp lệ của mã vòng.

---

# 2.4. Lớp CyclicCode

Lớp trung tâm của hệ thống.

Quản lý:

* thông số mã vòng,
* ma trận kiểm tra H,
* mã đối ngẫu,
* hệ trực giao,
* majority decoding.

## Thuộc tính chính

* l
* k
* d₀
* r = l - k
* h(x)
* parity-check matrix H
* dual codewords

---

# 3. Các Thuật toán Được Hỗ trợ

---

# 3.1. Sinh Ma trận Kiểm tra H

Từ đa thức kiểm tra h(x):

* đảo hệ số,
* sinh hàng đầu tiên,
* dịch vòng để tạo các hàng tiếp theo.

Ví dụ:

1011000
0101100
0010110

---

# 3.2. Sinh Mã Đối Ngẫu

Sinh tất cả tổ hợp XOR của các hàng trong H.

Nếu:

r = l - k

thì số lượng từ mã đối ngẫu là:

2^r

Chương trình sử dụng kỹ thuật tối ưu bằng:

__builtin_ctzll()

để tránh tính toán lại toàn bộ vector.

---

# 3.3. Kiểm tra Hệ Trực Giao Đầy Đủ

Chương trình sử dụng Backtracking để tìm:

J = d₀ - 1

phương trình trực giao độc lập.

Điều kiện:

* các vector phải chứa bit tại vị trí đang xét,
* các bit còn lại không được giao nhau.

Nếu tìm được:

=> mã có khả năng trực giao đầy đủ.

---

# 3.4. Majority Logic Decoding

Chương trình mô phỏng giải mã đa số:

* tính các tổng syndrome,
* đếm số phương trình cho kết quả 1,
* voting để xác định bit lỗi.

Nếu:

count_1 > J / 2

=> bit lỗi bằng 1.

Ngược lại:

=> bit lỗi bằng 0.

---

# 3.5. Early Exit

Hệ thống dừng ngay khi tìm thấy:

* một hệ trực giao hợp lệ,
* hoặc xác định chắc chắn không tồn tại.

Điều này giúp giảm thời gian chạy đáng kể.

---

# 3.6. Quiz Học tập Tương tác

Bao gồm 3 mức độ:

* Dễ
* Trung bình
* Khó

Các câu hỏi liên quan:

* kích thước ma trận H,
* trực giao,
* dịch vòng,
* majority decoding,
* xác định lỗi.

---

# 4. Định dạng Input

Dữ liệu nhập gồm 2 dòng.

## Dòng 1

l k d0

Ví dụ:

7 4 3

## Dòng 2

Các hệ số của h(x) theo thứ tự tăng dần.

Ví dụ:

1 1 0 1 1

tương ứng:

h(x) = 1 + x + x³ + x⁴

---

# 5. Biên dịch và Thực thi

## Biên dịch
Không menu
```bash
g++ -std=c++11 Fully_orthogonalizable.cpp -o CyclicDecoder
```
Có menu


## Chạy chương trình

Linux/macOS:

```bash
./CyclicDecoder
```

Windows:

```bash
CyclicDecoder.exe
```

---

# 6. Chạy bằng File Input

Ví dụ:

```bash
./CyclicDecoder < input.txt
```

Ví dụ nội dung file:

```text
7 4 3
1 1 0 1 1
```

---

# 7. Output

## Trường hợp thành công

```text
=> KET LUAN: Ma vong CO kha nang truc giao day du.
```

## Trường hợp thất bại

```text
=> KET LUAN: Ma vong KHONG co kha nang truc giao day du.
```

---

# 8. Validation và Xử lý Ngoại lệ

Hệ thống kiểm tra đầy đủ dữ liệu đầu vào.

## 8.1. Đa thức toàn 0

```text
[LOI] h(x) la da thuc toan 0
```

---

## 8.2. Sai bậc đa thức

```text
[LOI] Bac cua h(x) phai bang k = ...
```

---

## 8.3. Không sinh được mã vòng

```text
[LOI] (x^l + 1) khong chia het cho h(x)
```

---

## 8.4. Chưa nhập dữ liệu

```text
[CANH BAO] Ban chua nhap du lieu!
```

---

# 9. Độ phức tạp Thuật toán

## Sinh mã đối ngẫu

O(2^r)

---

## Backtracking trực giao

O(C(n, J))

---

## Chia đa thức

O(l²)

---

# 10. Giới hạn Hệ thống

Hệ thống phù hợp cho:

* mã vòng kích thước nhỏ,
* học tập,
* mô phỏng thuật toán.

Khi:

r = l - k

quá lớn:

* số lượng dual codewords tăng theo cấp số mũ,
* backtracking có thể rất chậm.

---

# 11. Công nghệ sử dụng

* C++11
* STL
* vector
* unique_ptr
* backtracking
* OOP

---

# 12. Các kiến thức Lý thuyết Thông tin được sử dụng

* GF(2)
* Cyclic Code
* Dual Code
* Parity Check Matrix
* Majority Logic Decoding
* Syndrome
* Orthogonal Check Equations

---

# 13. Ví dụ Minh họa

Input:

```text
7 4 3
1 1 0 1 1
```

Output:

```text
=> KET LUAN: Ma vong CO kha nang truc giao day du.
```

---

# 14. Hướng Phát triển

Trong tương lai có thể mở rộng:

* BCH Code
* Hamming Code
* Reed-Solomon
* Syndrome Decoding
* GUI Visualization
* Xuất file phân tích
* Mô phỏng truyền dẫn có nhiễu

---

# 15. Kết luận

Dự án là một mini-framework mô phỏng mã hóa kênh trên GF(2), kết hợp:

* Toán học mã hóa,
* Lập trình hướng đối tượng,
* Giải thuật tổ hợp,
* Majority Logic Decoding.

Phù hợp cho:

* học phần Lý thuyết thông tin,
* nghiên cứu cơ bản về Coding Theory,
* mô phỏng thuật toán mã hóa kênh.

---

*Tài liệu phục vụ mục đích học tập và nghiên cứu.*
