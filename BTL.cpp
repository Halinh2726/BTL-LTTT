#include <bits/stdc++.h>
using namespace std;

class Bit
{
public:
    int val;
    Bit(int v = 0) : val(v & 1) {}
    Bit operator+(Bit b) const
    {
        return val ^ b.val;
    }
    Bit operator*(Bit b) const
    {
        return val & b.val;
    }
    Bit &operator+=(Bit b)
    {
        val ^= b.val;
        return *this;
    }
};

class BinaryVector
{
public:
    vector<Bit> bits;
    int len;
    BinaryVector(int n = 0)
    {
        len = n;
        bits.assign(n, 0);
    }

    Bit &operator[](int i)
    {
        return bits[i];
    }
    const Bit &operator[](int i) const
    {
        return bits[i];
    }

    BinaryVector operator+(const BinaryVector &o) const
    {
        BinaryVector res(len);
        for (int i = 0; i < len; i++)
            res[i] = bits[i] + o[i];
        return res;
    }
    bool operator<(const BinaryVector &o) const
    {
        for (int i = 0; i < len; i++)
        {
            if (bits[i].val != o.bits[i].val)
                return bits[i].val < o.bits[i].val;
        }
        return false;
    }
    bool operator==(const BinaryVector &o) const
    {
        for (int i = 0; i < len; i++)
        {
            if (bits[i].val != o.bits[i].val)
                return false;
        }
        return true;
    }

    BinaryVector cyclicShiftRight() const
    {
        BinaryVector res(len);
        for (int i = 0; i < len; i++)
            res[(i + 1) % len] = bits[i];
        return res;
    }

    bool isZero() const
    {
        for (Bit b : bits)
            if (b.val)
                return false;
        return true;
    }
    friend ostream &operator<<(ostream &out, const BinaryVector &bv)
    {
        for (int i = 0; i < bv.len; i++)
        {
            out << bv.bits[i].val;
        }
        out << "\n";
        return out;
    }

    void print() const
    {
        for (int i = 0; i < len; i++)
        {
            cout << bits[i].val;
        }
        cout << "\n";
    }
};

class Polynomial
{
public:
    vector<Bit> coeffs;
    Polynomial(int deg = 0)
    {
        coeffs.assign(deg + 1, 0);
    }
    Polynomial(const vector<int> &c)
    {
        for (int x : c)
            coeffs.push_back(x);
        trim();
    }

    void trim()
    {
        while (coeffs.size() > 1 && coeffs.back().val == 0)
            coeffs.pop_back();
    }
    int degree() const
    {
        return coeffs.size() - 1;
    }

    bool isZero() const
    {
        for (Bit b : coeffs)
            if (b.val)
                return false;
        return true;
    }

    Polynomial mod(const Polynomial &div) const
    {
        Polynomial rem = *this;
        rem.trim();
        while (rem.degree() >= div.degree() && !rem.isZero() && rem.coeffs.back().val != 0)
        {
            int shift = rem.degree() - div.degree();
            for (int i = 0; i <= div.degree(); i++)
                rem.coeffs[i + shift] += div.coeffs[i];
            rem.trim();
        }
        return rem;
    }

    static Polynomial xnMinus1(int l)
    {
        Polynomial p(l);
        p.coeffs[0] = 1;
        p.coeffs[l] = 1;
        return p;
    }
};

class CyclicCode
{
public:
    int l, k, d0, r, J;
    Polynomial hx;
    vector<BinaryVector> H, dual_codewords;

    CyclicCode(int l, int k, int d0, Polynomial h)
    {
        this->l = l;
        this->k = k;
        this->d0 = d0;
        this->r = l - k;
        this->hx = h;
        this->J = d0 - 1;
        BinaryVector h_rev(l);
        for (int i = 0; i <= h.degree(); i++)
        {
            h_rev[h.degree() - i] = h.coeffs[i];
        }

        H.push_back(h_rev);
        for (int i = 1; i < r; i++)
        {
            H.push_back(H.back().cyclicShiftRight());
        }
    }

    void parityCheckMatrixPrint()
    {

        cout << "\n--- MA TRAN KIEM TRA H (Kich thuoc " << r << " x " << l << ") ---" << endl;
        for (int i = 0; i < r; i++)
        {
            cout << H[i];
        }
    }

    void printCyclic()
    {
        cout << "\n--- THONG SO MA VONG ---\n";
        cout << "Chieu dai tu ma (l)        : " << l << "\n";
        cout << "Do dai ban tin (k)         : " << k << "\n";
        cout << "Khoang cach thiet ke (d0)  : " << d0 << "\n";
        cout << "So bit kiem tra (r = l - k): " << r << "\n";
        cout << "Da thuc kiem tra h(x)      : ";
        bool first = true;
        for (int i = 0; i <= hx.degree(); i++)
        {
            if (hx.coeffs[i].val == 1)
            {
                if (!first)
                    cout << " + ";
                if (i == 0)
                    cout << "1";
                else if (i == 1)
                    cout << "x";
                else
                    cout << "x^" << i;
                first = false;
            }
        }
        if (first)
            cout << "0";
        cout << "\n";
        parityCheckMatrixPrint();

        cout << "\n------------------------\n";
    }

    void generateDualCodewords()
    {
        if (!dual_codewords.empty())
            return;

        BinaryVector w(l);
        dual_codewords.push_back(w);
        for (long long i = 1; i < (1LL << r); i++)
        {
            int changed_bit_index = __builtin_ctzll(i);
            w = w + H[changed_bit_index];
            dual_codewords.push_back(w);
        }
    }

    bool findOrthogonal(int start, vector<int> &chosen, vector<int> &usage, int J, const vector<BinaryVector> &cands, int pos)
    {
        if (chosen.size() == J)
            return true;
        if (J - chosen.size() > cands.size() - start)
            return false;

        for (int i = start; i < cands.size(); i++)
        {
            bool ok = true;
            for (int bit = 0; bit < l; bit++)
                if (bit != pos && cands[i][bit].val == 1 && usage[bit] >= 1)
                {
                    ok = false;
                    break;
                }
            if (!ok)
                continue;

            chosen.push_back(i);
            for (int bit = 0; bit < l; bit++)
                if (bit != pos && cands[i][bit].val == 1)
                    usage[bit]++;

            if (findOrthogonal(i + 1, chosen, usage, J, cands, pos))
                return true;

            chosen.pop_back();
            for (int bit = 0; bit < l; bit++)
                if (bit != pos && cands[i][bit].val == 1)
                    usage[bit]--;
        }
        return false;
    }

    bool checkFullyOrthogonalizable(bool show_steps)
    {
        if (show_steps)
        {
            cout << "\n--- BUOC 1: Kiem tra dieu kien J va so hang r ---" << endl;
            cout << "J = d0 - 1 = " << J << "\n";
            cout << "r = l - k  = " << r << "\n";
        }

        if (J >= l - k)
        {
            cout << "That bai: J >= l - k\n";
            return false;
        }

        if (show_steps)
        {
            cout << "\n--- BUOC 2: Sinh cac tu ma doi ngau ---" << endl;
        }

        vector<BinaryVector> cands;
        generateDualCodewords();

        int pos = l - 1;
        for (int i = 0; i < dual_codewords.size(); i++)
        {
            if (dual_codewords[i][pos].val == 1)
                cands.push_back(dual_codewords[i]);
        }
        if (show_steps)
        {
            cout << "+ So luong tu ma doi ngau co bit tai vi tri " << pos << " bang 1 (truoc khi loc): " << cands.size() << "\n";
            cout << "\n--- BUOC 3: Loc cac tu ma doc lap ---" << endl;
        }

        sort(cands.begin(), cands.end());
        cands.erase(unique(cands.begin(), cands.end()), cands.end());

        if (show_steps)
        {
            cout << "+ So luong tu ma sau khi loc trung: " << cands.size() << "\n";
        }

        if (cands.size() < J)
        {
            if (show_steps)
                cout << "=> That bai: Khong du " << J << " tu ma ung cu vien de xet truc giao.\n";
            return false;
        }

        if (show_steps)
        {
            cout << "\n--- BUOC 4: Kiem tra " << J << " phuong trinh truc giao (Backtracking) ---" << endl;
        }

        vector<int> chosen, usage(l, 0);
        if (findOrthogonal(0, chosen, usage, J, cands, pos))
        {
            if (show_steps)
            {
                cout << "=> TIM THAY HET THONG TRUC GIAO! Cac vector gom:\n";
                for (int idx : chosen)
                {
                    cout << "   ";
                    // cands[idx].print();
                    cout << cands[idx];
                }
            }
            return true;
        }

        if (show_steps)
            cout << "=> That bai: Quet toan bo khong tim thay tap truc giao nao.\n";
        return false;
    }

    void playQuiz()
    {
        cout << "\n========== CAU HOI KIEM TRA KIEN THUC ==========\n";
        cout << "Vui long chon muc do:\n";
        cout << "1. De (Kich thuoc ma tran & Ly thuyet sua loi)\n";
        cout << "2. Trung binh (Tim Bo truc giao & Dich vong ngau nhien)\n";
        cout << "3. Kho (Thuc hanh Giai ma Da so 1 buoc)\n";
        cout << "Nhap lua chon (1, 2 hoac 3): ";

        int level;
        if (!(cin >> level))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Loi nhap lieu!\n";
            return;
        }
        auto ask_question = [](string question, vector<pair<string, bool>> &opts, string explanation)
        {
            cout << "\n" << question << "\n";

            random_device rd;
            mt19937 g(rd());
            shuffle(opts.begin(), opts.end(), g);

            char correct_char = 'A';
            for (int i = 0; i < 4; ++i)
            {
                cout << char('A' + i) << ". " << opts[i].first << "\n";
                if (opts[i].second)
                    correct_char = char('A' + i);
            }

            cout << "=> Nhap dap an cua ban (A/B/C/D): ";
            string ans;
            cin >> ans;

            if (ans.length() > 0 && toupper(ans[0]) == correct_char)
            {
                cout << "-> CHINH XAC!\n";
            }
            else
            {
                cout << "-> SAI! Dap an dung la " << correct_char << ".\n";
            }

            if (!explanation.empty())
            {
                cout << "[Giai thich]:\n" << explanation << "\n";
            }
        };
        if (level == 1)
        {
            cout << "\n[MUC DO: DE]\n";
            string q1 = "Cau 1: Kich thuoc cua ma tran kiem tra H doi voi ma vong (" + to_string(l) + ", " + to_string(k) + ") hien tai la bao nhieu?";
            vector<pair<string, bool>> opts1 = {
                {to_string(r) + " hang, " + to_string(l) + " cot", true},
                {to_string(k) + " hang, " + to_string(l) + " cot", false},
                {to_string(l) + " hang, " + to_string(r) + " cot", false},
                {to_string(r) + " hang, " + to_string(k) + " cot", false}
            };
            string exp1 = "Ma tran kiem tra H luon co kich thuoc (r x l), voi r = l - k = " + to_string(r) + " hang va l = " + to_string(l) + " cot.";
            ask_question(q1, opts1, exp1);
            string q2 = "Cau 2: De thuat toan Giai ma da so 1 buoc sua duoc thanh cong toi da t loi (t errors), so luong phuong trinh truc giao J phai thoa man dieu kien nao duoi day?";
            vector<pair<string, bool>> opts2 = {
                {"J >= 2t", true},
                {"J >= t", false},
                {"J >= 2t + 1", false},
                {"J = t", false}
            };

            stringstream exp2;
            exp2 << "De sua duoc t loi, khoang cach thiet ke d0 phai dap ung: d0 >= 2t + 1.\n";
            exp2 << "Vi he truc giao day du yeu cau J = d0 - 1, ta suy ra J >= (2t + 1) - 1 => J >= 2t.\n";
            exp2 << "(Hieu don gian: Can it nhat 2t phuong trinh de neu co t phuong trinh bi nhieu (do t loi), thi cac phuong trinh dung van khong bi ap dao).";

            ask_question(q2, opts2, exp2.str());
        }
        else if (level == 2)
        {
            cout << "\n[MUC DO: TRUNG BINH]\n";
            generateDualCodewords();
            int pos = l - 1;
            vector<BinaryVector> cands;
            for (auto &w : dual_codewords)
                if (w[pos].val == 1)
                    cands.push_back(w);
            sort(cands.begin(), cands.end());
            cands.erase(unique(cands.begin(), cands.end()), cands.end());
            vector<int> chosen, usage(l, 0);

            bool found = findOrthogonal(0, chosen, usage, J, cands, pos);

            string q1 = "Cau 1: Tong kiem tra truc giao nao la phu hop voi bai (tai vi tri e_" + to_string(pos) + ")?";
            vector<pair<string, bool>> opts1;

            if (found)
            {
                stringstream ss_true;
                ss_true << "Bo " << J << " phuong trinh (tu ma) truc giao gom:\n";
                for (int i = 0; i < J; i++)
                {
                    ss_true << "     ";
                    for (int bit = 0; bit < l; bit++)
                        ss_true << cands[chosen[i]][bit].val;
                    if (i != J - 1)
                        ss_true << "\n";
                }
                stringstream ss_fake1;
                ss_fake1 << "Bo " << J << " phuong trinh (tu ma) truc giao gom:\n";
                for (int i = 0; i < J; i++)
                {
                    ss_fake1 << "     ";
                    for (int bit = 0; bit < l; bit++)
                    {
                        int val = cands[chosen[i]][bit].val;
                        if (i == 0 && bit == 0 && bit != pos)
                            val = 1 - val; // Lật bit
                        ss_fake1 << val;
                    }
                    if (i != J - 1)
                        ss_fake1 << "\n";
                }
                stringstream ss_fake2;
                ss_fake2 << "Bo " << J << " phuong trinh (tu ma) truc giao gom:\n";
                int fake_bit_idx = (l > 1) ? (l - 2) : 0; 
                for (int i = 0; i < J; i++)
                {
                    ss_fake2 << "     ";
                    for (int bit = 0; bit < l; bit++)
                    {
                        int val = cands[chosen[i]][bit].val;
                        if (i == J - 1 && bit == fake_bit_idx && bit != pos)
                            val = 1 - val; // Lật bit
                        ss_fake2 << val;
                    }
                    if (i != J - 1)
                        ss_fake2 << "\n";
                }

                opts1 = {
                    {ss_true.str(), true},
                    {ss_fake1.str(), false},
                    {ss_fake2.str(), false},
                    {"Khong ton tai bo truc giao day du cho ma nay.", false}
                };
            }
            else
            {
                stringstream ss_fake;
                ss_fake << "Bo " << J << " phuong trinh (tu ma) truc giao gom:\n";
                for (int i = 0; i < min(J, (int)dual_codewords.size()); i++)
                {
                    ss_fake << "     ";
                    for (int bit = 0; bit < l; bit++)
                        ss_fake << dual_codewords[i][bit].val;
                    if (i != J - 1)
                        ss_fake << "\n";
                }

                opts1 = {
                    {"Khong ton tai bo truc giao day du cho ma nay.", true},
                    {ss_fake.str(), false}, 
                    {"Bo " + to_string(J) + " vector duoc chon ngau nhien tu khong gian ma.", false},
                    {"Bo " + to_string(J) + " vector toan so 1.", false}
                };
            }

            string exp1 = "De la mot bo truc giao day du tai e_" + to_string(pos) + ", cac vector phai thoa man:\n"
                          "1) La tu ma doi ngau hop le.\n"
                          "2) Luon co bit 1 tai vi tri e_" + to_string(pos) + ".\n"
                          "3) KHONG duoc phep co bit 1 chung o bat ky vi tri nao khac.";
            ask_question(q1, opts1, exp1);
            int target_x = (l > 2) ? (l - 2) / 2 : 0;
            int shift_right = (target_x + 1) % l;
            int shift_left = (l - 1) - target_x;
            int min_shift = min(shift_right, shift_left);

            int fake1 = min_shift + 1;
            int fake2 = (min_shift - 1 <= 0) ? min_shift + 2 : min_shift - 1;
            int fake3 = l;

            string q2 = "Cau 2: Voi bo kiem tra truc giao day du tai e_" + to_string(pos) + ", can dich it nhat bao nhieu nhip de tro thanh bo kiem tra tai vi tri e_" + to_string(target_x) + "?";
            vector<pair<string, bool>> opts2 = {
                {to_string(min_shift) + " nhip", true},
                {to_string(fake1) + " nhip", false},
                {to_string(fake2) + " nhip", false},
                {to_string(fake3) + " nhip", false}
            };

            stringstream exp2;
            exp2 << "De dua tu vi tri e_" << pos << " ve e_" << target_x << " tren chieu dai l = " << l << ":\n";
            exp2 << "  - Neu dich PHAI xoay vong, ban can: (x + 1) % l = (" << target_x << " + 1) % " << l << " = " << shift_right << " nhip.\n";
            exp2 << "  - Neu dich TRAI xoay vong, ban can: (l - 1) - x = (" << l << " - 1) - " << target_x << " = " << shift_left << " nhip.\n";
            exp2 << "  -> Vay so nhip dich it nhat (Min) phai la: " << min_shift << " nhip.";

            ask_question(q2, opts2, exp2.str());
        }
        else if (level == 3)
        {
            cout << "\n[MUC DO: KHO]\n";
            generateDualCodewords();
            int pos = l - 1;
            vector<BinaryVector> cands;
            for (auto &w : dual_codewords)
                if (w[pos].val == 1)
                    cands.push_back(w);
            sort(cands.begin(), cands.end());
            cands.erase(unique(cands.begin(), cands.end()), cands.end());
            vector<int> chosen, usage(l, 0);

            bool found = findOrthogonal(0, chosen, usage, J, cands, pos);

            if (!found)
            {
                cout << "[THONG BAO]: Ma vong hien tai KHONG co he truc giao day du.\n";
                cout << "=> Ban khong the thuc hien thuat toan Giai ma da so tren bo ma nay.\n";
                cout << "=> Meo: Hay thu nhap Ma Hamming (7, 4) voi d0 = 3, h(x) = 1 + x + x^3 (He so: 1 1 0 1) de test tinh nang nay!\n";
            }
            else
            {
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<> distrib(0, 1);

                BinaryVector R(l);
                stringstream ss_R;
                for (int i = 0; i < l; i++)
                {
                    R[i].val = distrib(gen);
                    ss_R << R[i].val;
                }

                int count_1 = 0;
                stringstream exp3;
                exp3 << "Qua trinh nhan cham (XOR) tu ma nhan R voi cac phuong trinh truc giao:\n";
                for (int i = 0; i < J; i++)
                {
                    int idx = chosen[i];
                    int dot = 0;
                    exp3 << "  - P" << i + 1 << " = ";
                    for (int b = 0; b < l; b++)
                    {
                        dot ^= (R[b].val & cands[idx][b].val);
                        exp3 << cands[idx][b].val;
                    }
                    exp3 << " ---> Tong A_" << i + 1 << " = " << dot << "\n";
                    if (dot == 1)
                        count_1++;
                }

                bool is_majority = (count_1 > J / 2);
                int error_bit = is_majority ? 1 : 0;

                exp3 << "\nTong ket: Co " << count_1 << "/" << J << " phuong trinh cho ra tong A_j = 1.\n";
                if (is_majority)
                {
                    exp3 << "=> So luong A_j = 1 da QUA BAN ( > " << J / 2 << " ).\n=> Suy ra bit loi e_" << pos << " = 1.";
                }
                else
                {
                    exp3 << "=> So luong A_j = 1 CHUA QUA BAN ( <= " << J / 2 << " ).\n=> Suy ra bit loi e_" << pos << " = 0.";
                }

                string q3 = "Cau 1: Gia su ban nhan duoc vector R = [" + ss_R.str() + "].\nAp dung giai ma da so voi he " + to_string(J) + " phuong trinh truc giao tai e_" + to_string(pos) + " ban da tim duoc o Level 1.\nHay tinh cac tong A_j va xac dinh gia tri cua bit loi e_" + to_string(pos) + "?";

                string reason_true = is_majority ? "Da qua ban" : "Chua qua ban";
                string reason_false = !is_majority ? "Da qua ban" : "Chua qua ban";
                int fake_count = (count_1 == J) ? count_1 - 1 : count_1 + 1; 

                vector<pair<string, bool>> opts3 = {
                    {"e_" + to_string(pos) + " = " + to_string(error_bit) + " (Co " + to_string(count_1) + "/" + to_string(J) + " tong bang 1 -> " + reason_true + ")", true},
                    {"e_" + to_string(pos) + " = " + to_string(1 - error_bit) + " (Co " + to_string(count_1) + "/" + to_string(J) + " tong bang 1 -> " + reason_false + ")", false},
                    {"e_" + to_string(pos) + " = " + to_string(error_bit) + " (Co " + to_string(fake_count) + "/" + to_string(J) + " tong bang 1 -> " + reason_true + ")", false},
                    {"e_" + to_string(pos) + " = " + to_string(1 - error_bit) + " (Co " + to_string(fake_count) + "/" + to_string(J) + " tong bang 1 -> " + reason_false + ")", false}
                };

                ask_question(q3, opts3, exp3.str());
            }
        }
        else
        {
            cout << "Muc do khong hop le!\n";
        }
        cout << "================================================\n";
    }
};

int main()
{
    unique_ptr<CyclicCode> code = nullptr;
    bool has_data = false;

    while (true)
    {
        cout << "\n================= MENU =================\n";
        cout << "1. Nhap cac he so l, k, d0 va va he so cua h(x)\n";
        cout << "2. Kiem tra cac he so l, k, d0 va va he so cua h(x)\n";
        cout << "3. In ket qua (Co truc giao doc lap khong?)\n";
        cout << "4. In chi tiet cac buoc giai\n";
        cout << "5. Cau hoi\n";
        cout << "6. Thoat chuong trinh\n";
        cout << "========================================\n";
        cout << "Nhap lua chon cua ban: ";

        int choice;
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Lua chon khong hop le!\n";
            continue;
        }

        if (choice == 1)
        {
            int l, k, d0;
            cout << "Nhap l, k, d0: ";
            cin >> l >> k >> d0;

            vector<int> hcoeffs(k + 1);
            cout << "Nhap " << k + 1 << " he so cua da thuc h(x) (Tu bac 0 den bac k): ";
            for (int i = 0; i <= k; i++)
                cin >> hcoeffs[i];

            Polynomial Hx(hcoeffs);

            if (Hx.degree() != k)
            {
                cout << "[LOI] Bac cua h(x) phai bang k = " << k << ". Bac hien tai: " << Hx.degree() << "\n";
                has_data = false;
            }
            else if (!Polynomial::xnMinus1(l).mod(Hx).isZero())
            {
                cout << "[LOI] (x^" << l << " + 1) khong chia het cho h(x). Ma khong hop le!\n";
                has_data = false;
            }

            else
            {
                code = make_unique<CyclicCode>(l, k, d0, Hx);
                has_data = true;
                cout << "[THANH CONG] Da luu du lieu ma vong!\n";
            }
        }
        else if (choice == 2)
        {

            if (!has_data)
            {
                cout << "[CANH BAO] Ban chua nhap du lieu! Vui long chon Menu 1 truoc.\n";
                continue;
            }
            code->printCyclic();
        }
        else if (choice == 3)
        {
            if (!has_data)
            {
                cout << "[CANH BAO] Ban chua nhap du lieu! Vui long chon Menu 1 truoc.\n";
                continue;
            }
            bool result = code->checkFullyOrthogonalizable(false);
            if (result)
                cout << "\n=> KET LUAN: Ma vong CO kha nang truc giao day du.\n";
            else
                cout << "\n=> KET LUAN: Ma vong KHONG co kha nang truc giao day du.\n";
        }
        else if (choice == 4)
        {
            if (!has_data)
            {
                cout << "[CẢNH BÁO] Ban chua nhap du lieu! Vui long chon Menu 1 truoc.\n";
                continue;
            }
            bool result = code->checkFullyOrthogonalizable(true);
            cout << "\n----------------------------------------\n";
            if (result)
                cout << "=> KET LUAN CUOI CUNG: Ma vong CO kha nang truc giao day du.\n";
            else
                cout << "=> KET LUAN CUOI CUNG: Ma vong KHONG co kha nang truc giao day du.\n";
        }
        else if (choice == 5)
        {
            if (!has_data)
            {
                cout << "[CANH BAO] Ban chua nhap du lieu! Vui long chon Menu 1 truoc.\n";
                continue;
            }

            code->playQuiz();
        }
        else if (choice == 6)
        {
            cout << "Thoat chuong trinh...\n";
            break;
        }
        else
        {
            cout << "Lua chon khong hop le! Vui long chon tu 1 - 4.\n";
        }
    }

    return 0;
}
