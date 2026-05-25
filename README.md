# BTL-LTTT

## Kiểm tra Tính Trực Giao Đầy Đủ của Mã Vòng Tuyến Tính trên GF(2)

Dự án xây dựng chương trình kiểm tra **tính trực giao đầy đủ** của mã vòng tuyến tính trên trường GF(2), phục vụ học tập môn **Lý thuyết thông tin và Mã hóa kênh**.

Dự án có 2 phiên bản chương trình:

| File | Mục đích |
|---|---|
| `Fully_orthogonalizable.cpp` | Bản đầy đủ **có menu**, dùng để học, xem từng bước, in thông tin mã, làm quiz và mô phỏng kiến thức |
| `Fully_orthogonalizable_only.cpp` | Bản rút gọn **không có menu**, dùng để chạy nhanh, test input/output, nộp bài hoặc chạy bằng file input có sẵn |

---

# 1. Tổng quan

Chương trình kiểm tra mã vòng tuyến tính:

```text
C(l, k, d0)
```

Trong đó:

- `l`: chiều dài từ mã
- `k`: số bit thông tin
- `d0`: khoảng cách thiết kế
- `r = l - k`: số bit kiểm tra
- `h(x)`: đa thức kiểm tra

Mục tiêu chính của chương trình là kiểm tra xem mã vòng có **hệ phương trình trực giao đầy đủ** hay không.

Nếu có hệ trực giao đầy đủ, mã có thể được dùng trong thuật toán **Majority Logic Decoding**.

---

# 2. Phân chia phiên bản chương trình

## 2.1. File `Fully_orthogonalizable.cpp` - bản có menu

Đây là phiên bản đầy đủ hơn, phù hợp để học và trình bày báo cáo.

Phiên bản này có:

- menu tương tác
- nhập dữ liệu nhiều lần
- kiểm tra thông số mã vòng
- in ma trận kiểm tra `H`
- in chi tiết từng bước kiểm tra trực giao
- kiểm tra kết quả có trực giao đầy đủ hay không
- hệ thống câu hỏi quiz
- mô phỏng kiến thức Majority Logic Decoding
- giải thích một số bước xử lý

Bản này phù hợp khi:

- cần học lý thuyết
- cần xem từng bước chương trình chạy
- cần trình bày với giáo viên
- cần minh họa thuật toán
- cần làm báo cáo/bài tập lớn

---

## 2.2. File `Fully_orthogonalizable_only.cpp` - bản không có menu

Đây là phiên bản rút gọn, chỉ tập trung vào chức năng chính:

- nhập dữ liệu từ `stdin`
- kiểm tra đa thức
- Hoặc file `input.txt` bất kì
- sinh ma trận kiểm tra `H`
- sinh mã đối ngẫu
- kiểm tra trực giao đầy đủ
- in kết luận cuối cùng

Bản này phù hợp khi:

- cần chạy nhanh
- cần test bằng file input có sẵn
- cần chạy nhiều bộ test bằng cách đổi nội dung `input.txt`
- cần dùng cho chấm tự động
- không cần giao diện menu
- không cần quiz
- không cần phần giải thích tương tác

---

# 3. So sánh hai file

| Nội dung | `Fully_orthogonalizable.cpp` | `Fully_orthogonalizable_only.cpp` |
|---|---|---|
| Có menu | Có | Không |
| Nhập dữ liệu nhiều lần | Có | Không |
| In thông số mã vòng | Có | Không đầy đủ |
| In ma trận kiểm tra H | Có | Có hàm in, nhưng không gọi mặc định |
| Kiểm tra trực giao đầy đủ | Có | Có |
| In chi tiết các bước | Có | Có hỗ trợ qua tham số `show_steps`, nhưng mặc định không bật |
| Quiz học tập | Có | Không |
| Majority Logic Decoding | Có phần mô phỏng trong quiz | Không |
| Phù hợp để học | Rất phù hợp | Ít phù hợp hơn |
| Phù hợp để nộp/chạy test | Không tối ưu bằng | Phù hợp hơn |
| Chạy bằng file input có sẵn | Có thể nhưng không thuận tiện do có menu | Rất phù hợp |
| Mức độ kiến thức thể hiện | Nhiều hơn | Ít hơn, tập trung thuật toán chính |

---

# 4. Kiến thức có trong bản có menu

File `Fully_orthogonalizable.cpp` không chỉ kiểm tra kết quả mà còn bổ sung nhiều phần kiến thức hơn.

## 4.1. Kiến thức về mã vòng

Chương trình cho phép xem:

- chiều dài từ mã `l`
- số bit thông tin `k`
- khoảng cách thiết kế `d0`
- số bit kiểm tra `r = l - k`
- đa thức kiểm tra `h(x)`
- ma trận kiểm tra `H`

---

## 4.2. Kiến thức về ma trận kiểm tra H

Từ đa thức `h(x)`, chương trình:

- đảo hệ số của `h(x)`
- tạo hàng đầu tiên của ma trận `H`
- dịch vòng để tạo các hàng tiếp theo

Ví dụ:

```text
1011000
0101100
0010110
```

---

## 4.3. Kiến thức về mã đối ngẫu

Chương trình sinh các từ mã đối ngẫu bằng cách lấy tất cả tổ hợp XOR của các hàng trong `H`.

Nếu:

```text
r = l - k
```

thì số lượng từ mã đối ngẫu tối đa là:

```text
2^r
```

---

## 4.4. Kiến thức về trực giao đầy đủ

Chương trình kiểm tra:

```text
J = d0 - 1
```

Sau đó tìm `J` phương trình kiểm tra trực giao thỏa mãn:

- cùng chứa bit tại vị trí đang xét
- các vị trí còn lại không bị trùng nhau
- tạo thành hệ phương trình trực giao độc lập

Nếu tìm được hệ như vậy, mã được xem là có khả năng trực giao đầy đủ.

---

## 4.5. Kiến thức về Majority Logic Decoding

Bản có menu có thêm phần quiz mức khó, mô phỏng giải mã đa số.

Ý tưởng:

- lấy vector nhận được `R`
- nhân với từng phương trình trực giao
- tính các tổng kiểm tra `A_j`
- đếm số lượng `A_j = 1`
- quyết định bit lỗi theo đa số

Nếu:

```text
count_1 > J / 2
```

thì kết luận bit đang xét có lỗi.

Ngược lại, bit đó được xem là không lỗi.

---

## 4.6. Quiz học tập

Bản có menu có phần câu hỏi trắc nghiệm với 3 mức:

| Mức | Nội dung |
|---|---|
| Dễ | Kích thước ma trận H, lý thuyết sửa lỗi |
| Trung bình | Tìm bộ trực giao, dịch vòng |
| Khó | Mô phỏng Majority Logic Decoding |

Phần này giúp người học kiểm tra lại kiến thức sau khi chạy chương trình.

---

# 5. Kiến thức có trong bản không menu

File `Fully_orthogonalizable_only.cpp` giữ lại phần lõi thuật toán.

Các kiến thức chính gồm:

- biểu diễn bit trên GF(2)
- biểu diễn vector nhị phân
- biểu diễn đa thức trên GF(2)
- kiểm tra đa thức hợp lệ
- sinh ma trận kiểm tra `H`
- sinh mã đối ngẫu
- dùng backtracking tìm hệ trực giao
- kết luận mã có trực giao đầy đủ hay không

Bản này không có:

- menu
- quiz
- mô phỏng Majority Logic Decoding
- phần in thông số mã vòng chi tiết
- phần hướng dẫn tương tác cho người dùng

Vì vậy, bản không menu phù hợp với mục đích **chạy thuật toán**, còn bản có menu phù hợp với mục đích **học và trình bày kiến thức**.

---

# 6. Cấu trúc lớp trong chương trình

## 6.1. Lớp `Bit`

Biểu diễn một bit trong GF(2).

Hỗ trợ:

- phép cộng XOR
- phép nhân AND
- phép cộng gán XOR

Trong GF(2):

```text
1 + 1 = 0
1 * 1 = 1
```

---

## 6.2. Lớp `BinaryVector`

Biểu diễn vector nhị phân.

Ví dụ:

```text
1011010
```

Hỗ trợ:

- cộng vector trên GF(2)
- so sánh vector
- dịch vòng phải
- kiểm tra vector 0
- in vector

---

## 6.3. Lớp `Polynomial`

Biểu diễn đa thức trên GF(2).

Ví dụ:

```text
h(x) = 1 + x + x^3
```

được nhập dưới dạng:

```text
1 1 0 1
```

Hỗ trợ:

- chuẩn hóa đa thức
- lấy bậc đa thức
- kiểm tra đa thức 0
- chia lấy dư
- sinh đa thức `x^l + 1`

---

## 6.4. Lớp `CyclicCode`

Đây là lớp chính của chương trình.

Quản lý:

- `l`
- `k`
- `d0`
- `r`
- ma trận kiểm tra `H`
- các từ mã đối ngẫu
- thuật toán kiểm tra trực giao đầy đủ

Riêng bản có menu còn lưu thêm:

- đa thức `h(x)`
- `J = d0 - 1`
- các hàm in thông tin
- hàm quiz

---

# 7. Thuật toán chính

## 7.1. Kiểm tra đa thức hợp lệ

Chương trình kiểm tra:

```text
(x^l + 1) mod h(x) = 0
```

Nếu chia hết, đa thức hợp lệ để sinh mã vòng.

Nếu không chia hết, chương trình báo lỗi.

---

## 7.2. Sinh ma trận kiểm tra H

Các bước:

1. Đảo hệ số của `h(x)`
2. Đưa vào hàng đầu tiên của `H`
3. Dịch vòng phải để sinh các hàng tiếp theo
4. Sinh tổng cộng `r = l - k` hàng

---

## 7.3. Sinh mã đối ngẫu

Chương trình sinh các tổ hợp XOR của các hàng trong `H`.

Để tối ưu, chương trình dùng:

```cpp
__builtin_ctzll(i)
```

Hàm này giúp xác định bit thay đổi khi duyệt tổ hợp, tránh phải XOR lại từ đầu.

---

## 7.4. Lọc ứng viên trực giao

Chương trình chỉ chọn các vector đối ngẫu có bit tại vị trí đang xét bằng `1`.

Trong code hiện tại, vị trí được xét là:

```text
pos = l - 1
```

Sau đó chương trình loại các vector trùng nhau.

---

## 7.5. Backtracking tìm hệ trực giao

Chương trình chọn `J` vector sao cho:

- mỗi vector đều chứa bit tại `pos`
- các vị trí khác `pos` không bị trùng nhau
- đủ `J = d0 - 1` phương trình

Nếu chọn được, kết luận mã có trực giao đầy đủ.

---

# 8. Định dạng input

Cả hai phiên bản đều dùng cùng định dạng input.

## Dòng 1

```text
l k d0
```

Ví dụ:

```text
7 3 4
```

## Dòng 2

Nhập `k + 1` hệ số của `h(x)` theo thứ tự từ bậc thấp đến bậc cao.

Ví dụ:

```text
1 0 1 1
```

tương ứng:

```text
h(x) = 1 + x^2 + x^3
```

Lưu ý: Trong chương trình này, code đang kiểm tra bậc của `h(x)` phải bằng `k`.

---

# 9. Biên dịch

## 9.1. Biên dịch bản có menu

```bash
g++ -std=c++11 Fully_orthogonalizable.cpp -o CyclicDecoderMenu
```

Nếu dùng `make_unique`, có thể cần C++14:

```bash
g++ -std=c++14 Fully_orthogonalizable.cpp -o CyclicDecoderMenu
```

---

## 9.2. Biên dịch bản không menu

```bash
g++ -std=c++11 Fully_orthogonalizable_only.cpp -o CyclicDecoder
```

---

# 10. Chạy chương trình

## 10.1. Chạy bản có menu

Linux/macOS:

```bash
./CyclicDecoderMenu
```

Windows:

```bash
CyclicDecoderMenu.exe
```

Sau khi chạy, chương trình hiện menu:

```text
1. Nhap cac he so l, k, d0 va va he so cua h(x)
2. Kiem tra cac he so l, k, d0 va va he so cua h(x)
3. In ket qua (Co truc giao doc lap khong?)
4. In chi tiet cac buoc giai
5. Cau hoi
6. Thoat chuong trinh
```

Nên dùng bản này khi muốn học vì có nhiều phần giải thích và kiến thức hơn.

---

## 10.2. Chạy bản không menu

Bản không menu có thể chạy theo 2 cách.

### Cách 1: Nhập trực tiếp từ bàn phím

Linux/macOS:

```bash
./CyclicDecoder
```

Windows:

```bash
CyclicDecoder.exe
```

Sau đó nhập trực tiếp input:

```text
7 3 4
1 0 1 1
```

### Cách 2: Chạy bằng file input có sẵn

Tạo file `input.txt`:

```text
7 3 4
1 0 1 1
```

Sau đó chạy:

Linux/macOS:

```bash
./CyclicDecoder < input.txt
```

Windows CMD:

```bash
CyclicDecoder.exe < input.txt
```

Cách này rất phù hợp khi đã chuẩn bị sẵn dữ liệu test, muốn chạy lại nhiều lần, hoặc dùng để kiểm tra chương trình theo kiểu input/output chuẩn.

Nên dùng bản này khi chỉ cần kết quả cuối cùng hoặc muốn chạy với file input có sẵn.

---

# 11. Ví dụ chạy bản không menu

Có thể nhập trực tiếp hoặc lưu dữ liệu vào file `input.txt`.

Nội dung `input.txt`:

```text
7 3 4
1 0 1 1
```

Chạy chương trình:

```bash
./CyclicDecoder < input.txt
```

Output có dạng:

```text
=> KET LUAN CUOI CUNG: Ma vong CO kha nang truc giao day du.
```

hoặc:

```text
=> KET LUAN CUOI CUNG: Ma vong KHONG co kha nang truc giao day du.
```

---

# 12. Ví dụ chạy bản có menu

Sau khi chạy:

```bash
./CyclicDecoderMenu
```

Chọn:

```text
1
```

để nhập dữ liệu.

Sau đó nhập:

```text
7 3 4
1 0 1 1
```

Có thể chọn:

```text
2
```

để in thông tin mã vòng và ma trận H.

Có thể chọn:

```text
3
```

để xem kết luận mã có trực giao đầy đủ không.

Có thể chọn:

```text
4
```

để xem chi tiết các bước kiểm tra.

Có thể chọn:

```text
5
```

để làm câu hỏi quiz.

---

# 13. Các thông báo lỗi

## 13.1. Sai bậc đa thức

```text
[LOI] Bac cua h(x) phai bang k = ...
```

Nguyên nhân: đa thức nhập vào sau khi bỏ các hệ số 0 ở cuối có bậc không bằng `k`.

---

## 13.2. Đa thức không hợp lệ với mã vòng

```text
[LOI] (x^l + 1) khong chia het cho h(x). Ma khong hop le!
```

Nguyên nhân: `h(x)` không chia hết `x^l + 1`.

---

## 13.3. Chưa nhập dữ liệu

Chỉ có trong bản menu:

```text
[CANH BAO] Ban chua nhap du lieu! Vui long chon Menu 1 truoc.
```

Nguyên nhân: người dùng chọn kiểm tra/in kết quả trước khi nhập mã vòng.

---

# 14. Độ phức tạp

## 14.1. Sinh mã đối ngẫu

```text
O(2^r)
```

với:

```text
r = l - k
```

## 14.2. Backtracking tìm hệ trực giao

Độ phức tạp phụ thuộc vào số vector ứng viên và số phương trình cần chọn `J`.

Trong trường hợp xấu có thể rất lớn.

## 14.3. Chia đa thức

```text
O(l^2)
```

---

# 15. Giới hạn chương trình

Chương trình phù hợp với:

- mã vòng kích thước nhỏ
- học tập
- mô phỏng thuật toán
- bài tập lớn

Không nên dùng với `r` quá lớn vì số lượng từ mã đối ngẫu tăng rất nhanh:

```text
2^r
```

Ví dụ:

```text
r = 20 => 2^20 từ mã đối ngẫu
```

---

# 16. Công nghệ sử dụng

- C++
- STL
- vector
- unique_ptr
- backtracking
- lập trình hướng đối tượng
- GF(2)
- Cyclic Code
- Dual Code
- Majority Logic Decoding

---

# 17. Kết luận

Dự án gồm 2 phiên bản:

- `Fully_orthogonalizable.cpp`: bản học tập đầy đủ, có menu và nhiều kiến thức hơn
- `Fully_orthogonalizable_only.cpp`: bản tối giản, phù hợp để chạy nhanh, kiểm tra kết quả và chạy bằng file input có sẵn

Bản có menu nên dùng khi cần hiểu thuật toán, xem từng bước và ôn kiến thức.

Bản không menu nên dùng khi chỉ cần nhập dữ liệu và nhận kết luận cuối cùng.

---

*Tài liệu phục vụ mục đích học tập và nghiên cứu.*
