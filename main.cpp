#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class BigNum {
private:
    string value;
    bool isNegative = 0;
public:

    BigNum(string v) {
        if (v[0] == '-') {
            isNegative = 1;
            this->value = v.substr(1, v.size());
        } else {
            isNegative = 0;
            this->value = v;
        }
    }

    string complement(string n, int len) {
        for (int i = 0; i < n.size(); i++) {
            n[i] = '9' - n[i] + '0';
        }
        for (int i = n.size(); i < len; i++) {
            n = "9" + n;
        }
        return n;
    }

    int isBigger(string v, string n) {
        if (v.size() > n.size()) {
            return true;
        } else if (v.size() < n.size()) {
            return false;
        } else {
            for (int i = 0; i < v.size(); i++) {
                if (v[i] > n[i])
                    return true;
                else if (v[i] < n[i])
                    return false;
            }
        }
        return 2;
    }

    void strip(string &s, char c) {
        int i = 0;
        for (; i < s.size(); i++) {
            if (s[i] != c)
                break;
        }
        s = s.substr(i, s.size());
    }

    string getValue() {
        return value;
    }

    bool getIsNegative() {
        return this->isNegative;
    }

    string operator=(string v) {
        if (v[0] == '-') {
            isNegative = 1;
            this->value = v.substr(1, v.size());
        } else {
            isNegative = 0;
            this->value = v;
        }
        return this->value;
    }

    string add(string v, string n, bool isC) {
        int lenV = v.size(), lenN = n.size();
        string result = "";
        int s = 0, c = isC;
        char ch = 0;
        while (lenN > 0 || lenV > 0) {
            lenN--;
            lenV--;
            if (lenN < 0) {
                s = v[lenV] - '0' + c;
            } else if (lenV < 0) {
                s = n[lenN] - '0' + c;
            } else {
                s = v[lenV] - '0' + n[lenN] - '0' + c;
            }
            c = s / 10;
            s = s % 10;
            ch = char(s + '0');
            result = ch + result;
        }
        if (c != 0) {
            result = "1" + result;
        }
        return result;
    }

    string sub(string v, string n) {
        int lenV = v.size(), lenN = n.size();
        string result = "";
        int s = 0, c = 0;// 结果 借位
        char ch = 0;

        string nStr;
        int isBigger = this->isBigger(v, n);

        if (isBigger == 2)
            return "0";

        nStr = complement(n, max(lenV, lenN)); // 取反
        result = this->add(v, nStr, 1); // 反码加1 再加补码

        result = result.substr(result.size() - max(lenV, lenN), result.size());

        if (!isBigger) {
            result = complement(result, result.size());
            result = this->add(result, "1", 0);
            this->strip(result, '0');

            result = "-" + result;
        } else {
            this->strip(result, '0');
        }
        return result;
    }

    string extend(string s, int n) {
        string re = "";
        for (int i = 0; i < n; i++) {
            re = re + s;
        }
        return re;
    }

    string multipy(string v, string n) {
        if (v.size() + n.size() > 10 && v.size() && n.size()) {
            int x = v.size() / 2;
            bool isNe = 0;
            string a = v.substr(0, x), b = v.substr(x, v.size());
            string c = n.substr(0, x), d = n.substr(x, n.size());

            string re1, re2, re3;

            re1 = this->multipy(a, c);

            re3 = this->multipy(b, d);


            string re2_A = this->sub(a, b),
                    re2_B = this->sub(d, c);

            if (re2_A[0] == '-' && re2_B[0] == '-') {

            } else if (re2_A[0] != '-' && re2_B[0] != '-') {

            } else {
                isNe = 1;
            }
            this->strip(re2_A, '-');
            this->strip(re2_B, '-');
            re2 = (isNe ? "-" : "") + this->multipy(re2_A, re2_B);

            BigNum n1 = re1, n2 = re2, n3 = re3;
            re2 = n1 + n2;
            n2 = re2;
            re2 = n2 + n3;

            re1 = re1 + extend("0", (v.size() - x) * 2);
            re2 = re2 + extend("0", v.size() - x);
            return this->add(this->add(re1, re2, 0), re3, 0);
        } else {
            int v_i = 0, n_i = 0;
            if (v.size()) {
                v_i = stoi(v);
            }
            if (n.size()) {
                n_i = stoi(n);
            }
            int result = v_i * n_i;
            if (result < 0)
                result = -result;
            return to_string(result);
        }


    }

    string operator+(BigNum n) {
        string result = "";
        if (this->isNegative == n.getIsNegative()) {
            result = add(this->value, n.getValue(), 0);
            if (this->isNegative) {
                result = "-" + result;
            }
        } else if (this->isNegative && !n.getIsNegative()) {
            result = this->sub(n.getValue(), this->value);
        } else {
            result = this->sub(this->value, n.getValue());
        }
        return result;
    }

    string operator-(BigNum n) {
        string result = "";
        if (this->isNegative && n.getIsNegative()) {
            result = sub(n.getValue(), this->value);
        } else if (this->isNegative && !n.getIsNegative()) {
            result = this->add(n.getValue(), this->value, 0);
            result = "-" + result;
        } else if (!this->isNegative && !n.getIsNegative()) {
            result = this->sub(this->value, n.getValue());
        } else {
            result = this->add(n.getValue(), this->value, 0);
        }
        return result;
    }

    string operator*(BigNum n) {
        string result = "";
        bool isNe = n.getIsNegative() ^this->isNegative;

        result = (isNe ? "-" : "") + this->multipy(this->value, n.value);

        return result;
    }

};

string multiply(string num1, string num2) {
    int length1 = num1.size(), length2 = num2.size(), value = length1 + length2;
    if (num1 == "0" || num2 == "0")
        return "0";

    int num1_int[length1], num2_int[length2], num3_int[value];
    for (int i = 0; i < value; i++) {
        num3_int[i] = 0;
    }
    for (int i = 0; i < length1; i++) {
        num1_int[i] = num1[i] - '0';
    }
    for (int i = 0; i < length2; i++) {
        num2_int[i] = num2[i] - '0';
    }

    for (int i = 0; i < length1; i++) {
        for (int j = 0; j < length2; j++) {
            num3_int[i + j + 1] += num1_int[i] * num2_int[j];
        }
    }
    for (int i = value - 1; i > 0; i--) {
        if (num3_int[i] >= 10) {
            num3_int[i - 1] += num3_int[i] / 10;
            num3_int[i] %= 10;
        }
    }
    string ans = "";
    int i = 0;
    if (num3_int[0] == 0)
        i = 1;
    for (; i < value; i++) {
        ans += to_string(num3_int[i]);
    }
    return ans;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
//    cout << multiply("5", "12") << endl;
//    cout << multiply("0", "6") << endl;
//    cout << multiply("9999999999999", "9") << endl;
    BigNum n1("999999999"), n2("99999999999999999999999");

    n1 = "-8999999999999999999999999";
    n2 = "9000000000000000000001231";

    cout << n1 + n2 << endl;
    cout << n1 - n2 << endl;
    n1 = "-12345678910";
    n2 = "-72345678910";
    cout << n1 * n2 << endl;
    cout << multiply(n1.getValue(), n2.getValue()) << endl;
    return 0;
}