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
    vector<BinaryVector> H;

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
        if (start + J - chosen.size() > cands.size())
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

    void checkFullyOrthogonalizable()
    {
        int J = d0 - 1;
        vector<BinaryVector> dual_codewords;
        for (long long mask = 1; mask < (1LL << r); mask++)
        {
            BinaryVector w(l);
            for (int i = 0; i < r; i++)
                if ((mask >> i) & 1)
                    w = w + H[i];
            dual_codewords.push_back(w);
        }

        for (int pos = 0; pos < l; pos++)
        {
            vector<BinaryVector> cands;
            for (int i = 0; i < dual_codewords.size(); i++)
            {
                if (dual_codewords[i][pos].val == 1)
                    cands.push_back(dual_codewords[i]);
            }
            if (cands.size() < J)
                continue;

            vector<int> chosen, usage(l, 0);
            if (findOrthogonal(0, chosen, usage, J, cands, pos))
            {
                cout << "=> KET LUAN: Ma vong CO kha nang truc giao day du" << pos << "\n";
                return;
            }
        }
        cout << "=> KET LUAN: Ma vong KHONG co kha nang truc giao day du.\n";
    }
};

int main()
{
    int l, k, d0;
    cin >> l >> k >> d0;
    vector<int> hcoeffs(k + 1);
    for (int i = 0; i <= k; i++)
        cin >> hcoeffs[i];

    Polynomial Hx(hcoeffs);
    if (Hx.isZero())
    {
        cout << "[LOI] h(x) la da thuc toan 0\n";
        return 1;
    }

    if (Hx.degree() != k)
    {
        cout << "[LOI] Bac cua h(x) phai bang k = " << k << ". Bac hien tai: " << Hx.degree() << "\n";
        return 1;
    }
    if (!Polynomial::xnMinus1(l).mod(Hx).isZero())
    {
        cout << "[LOI] (x^l + 1) khong chia het cho h(x). Ma khong hop le!\n";
        return 1;
    }

    CyclicCode code(l, k, d0, Hx);
    code.checkFullyOrthogonalizable();
    return 0;
}