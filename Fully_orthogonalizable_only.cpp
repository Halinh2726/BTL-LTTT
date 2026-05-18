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
    int l, k, d0, r;
    vector<BinaryVector> H, dual_codewords;

    CyclicCode(int l, int k, int d0, Polynomial h)
    {
        this->l = l;
        this->k = k;
        this->d0 = d0;
        this->r = l - k;

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
    void matrixHPrint()
    {

        cout << "\n--- MA TRAN KIEM TRA H (Kich thuoc " << r << " x " << l << ") ---" << endl;
        for (int i = 0; i < r; i++)
        {
            cout << "H[" << i << "]: ";
            // H[i].print();
            cout << H[i];
        }
    }

    bool checkFullyOrthogonalizable(bool show_steps)
    {
        int J = d0 - 1;

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
};

int main()
{
    // freopen(FILENAMEIN, "r", stdin);
    // freopen(FILENAMEOUT, "w", stdout);
    int l, k, d0;
    cin >> l >> k >> d0;
    vector<int> hcoeffs(k + 1);
    for (int i = 0; i <= k; i++)
        cin >> hcoeffs[i];

    Polynomial Hx(hcoeffs);

    if (Hx.degree() != k)
    {
        cout << "[LOI] Bac cua h(x) phai bang k = " << k << ". Bac hien tai: " << Hx.degree() << "\n";
        return 1;
    }
    if (!Polynomial::xnMinus1(l).mod(Hx).isZero())
    {
        cout << "[LOI] (x^" << l << " + 1) khong chia het cho h(x). Ma khong hop le!\n";
        return 1;
    }

    CyclicCode code(l, k, d0, Hx);
    if (code.checkFullyOrthogonalizable(false))
        cout << "=> KET LUAN CUOI CUNG: Ma vong CO kha nang truc giao day du.\n";
    else
        cout << "=> KET LUAN CUOI CUNG: Ma vong KHONG co kha nang truc giao day du.\n";
    return 0;
}