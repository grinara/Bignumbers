#include <fstream>
#include <iostream>
#include <string>
#include <algorithm> 
typedef unsigned  char BASE;
typedef unsigned short int DBASE;
#define BASE_SIZE (sizeof(BASE)*8)
using namespace std;
class bignumber {
public:
	BASE* coef;
	int len;
	int maxlen;
public:

	bignumber() {
		len = 1;
		maxlen = len;
		coef = new BASE[len];
		coef[0] = 0;
	}
	bignumber(int l, int t = 0) {
		if (t == 0) {
			len = 1;
			maxlen = l;
			if (l == 0) { maxlen = len; }
			coef = new BASE[maxlen];
			for (int i = 0; i < maxlen; i++) { coef[i] = 0; }

		}
		else {
			len = l;
			maxlen = len;
			coef = new BASE[len];
			BASE x = 0;
			int j = 0;
			int k = 0;
			for (int i = l - 1; i >= 0; i--) {
				x = BASE(rand());
				if (x == 0 && i == l - 1) { i = l - 1; }
				else { coef[i] = x; }
			}
		}
	}
	bignumber(string str) {
		int l = str.size();
		int start = 0;
		while (str[start] == '0') { start++; }
		len = ((l - start - 1) / (BASE_SIZE / 4)) + 1;
		coef = new BASE[len];
		maxlen = len;
		for (int i = 0; i < len; i++) { coef[i] = 0; }
		BASE x;
		int j = 0;
		int k = 0;
		for (int i = l - 1; i >= start; i--) {
			if (str[i] >= '0' && str[i] <= '9') x = str[i] - '0';
			else if (str[i] >= 'a' && str[i] <= 'f') x = str[i] - 'a' + 10;
			else if (str[i] >= 'A' && str[i] <= 'F') x = str[i] - 'A' + 10;
			else break;
			coef[j] = ((x << (k * 4)) | coef[j]);
			x = 0; k++;
			if (k >= BASE_SIZE / 4) { k = 0; j++; }
		}
	}
	bignumber(const bignumber& c) {
		len = c.len;
		maxlen = c.maxlen;
		coef = new BASE[len];
		for (int i = 0; i < len; i++) {
			coef[i] = c.coef[i];
		}
	};
	~bignumber() {
		coef = new BASE[1];
		delete[] coef;
		len = 0;
		maxlen = 0;
	}
	int compare(bignumber b) {
		if (len > b.len) return 1;
		if (len < b.len) return -1;
		for (int i = len - 1; i >= 0; i--) {
			if (coef[i] > b.coef[i]) {
				return 1;
			}
			else if (coef[i] < b.coef[i]) {
				return -1;
			}
		}
		return 0;
	}
	void len_norm() {
		len = maxlen;
		while (coef[len - 1] == 0 && len > 0) {
			len--;
		}
		if (len == 0) { len++; };
	}
	void Input(string str, BASE c = 10) {
		int l = str.size();
		int start = 0;
		while (str[start] == '0') { start++; }
		len = ((l - start - 1) / (BASE_SIZE / 4)) + 1;
		coef = new BASE[len];
		for (int i = 0; i < len; i++) { this->coef[i] = 0; }
		maxlen = len;
		for (int i = start; i < l; i++) {
			bignumber t(1);
			t.coef[0] = str[i] - '0';
			*this = (*this * c) + t;
		}
	}
	void Output(BASE c = 10) {
		string str;
		bignumber u = *this;
		int j = 0;
		while (u != 0) {
			BASE t = u % c;
			t = t + '0';
			u = u / c;
			str += t;
		}
		reverse(str.begin(), str.end());
		cout << str;
		if (str.size() == 0)cout << '0';
		cout << endl;
	}
	bool operator >(const bignumber& c) {
		if (this->compare(c) == 1) return true;
		return false;
	}
	bool operator <(const bignumber& c) {
		if (this->compare(c) == -1) return true;
		return false;
	}
	bool operator >=(const bignumber& c) {
		if (this->compare(c) == 1 || this->compare(c) == 0) return true;
		return false;
	}
	bool operator <=(const bignumber& c) {
		if (this->compare(c) == -1 || this->compare(c) == 0) return true;
		return false;
	}
	bool operator ==(const bignumber& c) {
		if (this->compare(c) == 0) return true;
		return false;
	}
	bool operator !=(const bignumber& c) {
		if (this->compare(c) != 0) return true;
		return false;
	}
	bignumber& operator = (const bignumber& c) {
		if (this != &c) {
			len = c.len;
			maxlen = c.maxlen;
			delete[] coef;
			coef = new BASE[len];
			for (int i = 0; i < len; i++) { coef[i] = c.coef[i]; }
		}
		return *this;
	}
	bignumber operator + (const bignumber& v) {
		int l = max(len, v.len) + 1;
		int t = min(len, v.len);
		int j = 0;
		int k = 0;

		DBASE tmp;
		bignumber w(l);
		//w.len = l;
		while (j < t) {
			tmp = coef[j] + v.coef[j] + k;
			w.coef[j] = (BASE)tmp;
			k = (BASE)(tmp >> BASE_SIZE);
			j++;
		}
		while (j < len) {
			tmp = coef[j] + k;
			w.coef[j] = (BASE)tmp;
			k = (BASE)(tmp >> BASE_SIZE);
			j++;
		}
		while (j < v.len) {
			tmp = v.coef[j] + k;
			w.coef[j] = (BASE)tmp;
			k = (BASE)(tmp >> BASE_SIZE);
			j++;
		}
		w.coef[j] = k;
		w.len_norm();
		return w;
	}
	bignumber& operator += (const bignumber& v) {
		*this = *this + v;
		return *this;
	}
	bignumber operator -(const bignumber& v) {
		DBASE tmp;
		bignumber w(len);
		if (*this < v) { cout << "Отрицательная разность"; }
		else {
			int j = 0;
			int k = 0;
			while (j < v.len)
			{
				tmp = (1 << BASE_SIZE | coef[j]);
				tmp = tmp - v.coef[j] - k;
				w.coef[j] = (BASE)tmp;
				k = !(tmp >> BASE_SIZE);
				j++;
			}
			while (j < len) {
				tmp = (1 << BASE_SIZE | coef[j]);
				tmp -= k;
				w.coef[j] = (BASE)tmp;
				k = !(tmp >> BASE_SIZE);
				j++;
			}
		}
		w.len_norm();
		return w;
	}
	bignumber& operator -=(const bignumber& v) {
		*this = *this - v;
		return *this;
	}
	bignumber operator * (BASE v) {
		bignumber w(len + 1);
		if (*this == 0 || v == 0) { return w; }
		int j = 0;
		int k = 0;
		DBASE tmp;
		while (j < len) {
			tmp = coef[j] * v + k;
			w.coef[j] = (BASE)tmp;
			k = (BASE)(tmp >> BASE_SIZE);
			j++;
		}
		w.coef[j] = k;
		w.len_norm();
		return w;
	}
	bignumber& operator *= (BASE v) {
		int j = 0;
		int k = 0;
		DBASE tmp;
		bignumber w(len);
		while (j < len) {
			tmp = coef[j] * v + k;
			w.coef[j] = (BASE)tmp;
			k = (BASE)(tmp >> BASE_SIZE);
			j++;
		}
		w.coef[j] = k;
		*this = w;
		return w;
	}
	bignumber operator / (BASE v) {
		bignumber q(len);
		if (v <= 0) { cout << "Деление на 0!!!"; }
		else if (v == 1) { return *this; }
		else {
			int j = 0;
			BASE r = 0;
			DBASE tmp;
			while (j < len) {
				tmp = ((r << BASE_SIZE) + coef[len - 1 - j]);
				q.coef[len - 1 - j] = (BASE)(tmp / v);
				r = (BASE)(tmp % v);
				j++;
			}
			q.len_norm();
		}
		return q;

	}
	BASE operator % (BASE v) {
		BASE r = 0;
		if (v <= 0) { cout << "Деление на 0!!!"; }
		else {
			int j = 0;
			DBASE tmp;
			bignumber q(len);
			while (j < len) {
				tmp = ((r << BASE_SIZE) + coef[len - 1 - j]);
				q.coef[len - 1 - j] = (BASE)(tmp / v);
				r = (BASE)(tmp % v);
				j++;
			}
		}
		return r;
	}
	bignumber operator * (bignumber v) {
		bignumber w(len + v.len); // v.len = n(V), len = m(U)
		for (int i = 0; i < w.maxlen; i++) { w.coef[i] = 0; }
		int m = len;
		int n = v.len;
		int k = 0;
		DBASE tmp = 0;
		for (int j = 0; j < n; j++) {
			if (v.coef[j] != 0) {
				k = 0;
				for (int i = 0; i < m; i++) {
					tmp = (DBASE)coef[i] * (DBASE)v.coef[j] + (DBASE)w.coef[i + j] + (DBASE)k;
					w.coef[i + j] = (BASE)tmp;
					k = (BASE)(tmp >> BASE_SIZE);
				}
				w.coef[j + m] = (BASE)k;

			}
		}
		w.len_norm();
		return w;
	}
	bignumber& operator *= (bignumber v) {
		*this = *this * v;
		return *this;
	}
	bignumber operator / (bignumber v) {
		bignumber v1 = v;
		DBASE b = (1 << BASE_SIZE);
		bignumber u1 = *this;
		int n = v1.len;
		int m = u1.len - v1.len;
		DBASE tmp = v1.coef[n - 1] + 1;
		tmp = b / tmp;
		BASE d = (BASE)tmp;
		//int n = v1.len;
		//int m = u1.len - v1.len;
		//tmp = 0;
		if (d != 1) {
			u1 = u1 * d;
			v1 = v1 * d;
		}
		else {
			u1.coef[len] = 0;
		}
		int j = m;
		bignumber w(m + 1);
		DBASE q = 0;
		BASE jjj;
		DBASE r = 0;
		DBASE tmp1 = 0;
		DBASE tmp2 = 0;
		while (j >= 0) {
			q = 0;
			r = 0;
			q = (u1.coef[j + n] * b + u1.coef[j + n - 1]) / v1.coef[n - 1];
			r = (u1.coef[j + n] * b + u1.coef[j + n - 1]) % v1.coef[n - 1];
			while (r < b) {
				tmp1 = b * r + u1.coef[j + n - 2];
				tmp2 = v1.coef[n - 2] * q;
				if ((q == b) || (tmp2 > tmp1)) {
					q = q - 1; r = r + v1.coef[n - 1];
				}
				else break;

			}

			cout << q;
			cout << endl;
			BASE q1 = (BASE)q;
			int i = 0;
			int k2 = 0;
			int k1 = 0;
			tmp1 = 0;
			tmp2 = 0;
			while (i < n)
			{
				tmp1 = v1.coef[i] * q1 + k1;
				jjj = (BASE)tmp1;
				k1 = (BASE)(tmp1 >> BASE_SIZE);
				tmp2 = (b | u1.coef[i + j]) - jjj - k2;;
				u1.coef[j + i] = (BASE)tmp2;
				k2 = !(tmp2 >> BASE_SIZE);
				i++;

			}

			tmp = 0;
			if (k2 == 1) {
				k2 = 0;
				q1--;
				while (i < n) {
					tmp = u1.coef[j + i] + v1.coef[i] + k2;
					u1.coef[j + i] = (BASE)tmp;
					k2 = (BASE)(tmp >> BASE_SIZE);
					i++;
				}
			}
			w.coef[j] = q1;
			cout << "j" << j << " " << "q1" << " " << (int)q1 << "  ";
			j--;
		}

		//u1 = u1/ d; //остаток
		w.len_norm();
		return w;

	}
	friend ostream& operator<<(ostream&, const bignumber& c);
	friend istream& operator>>(istream&, bignumber& c);
};
ostream& operator<<(ostream& out, const bignumber& c)
{
	cout << hex << (int)c.coef[c.len - 1];
	for (int j = c.maxlen - 2; j >= 0; j--) {
		cout.width(BASE_SIZE / 4);
		cout.fill('0');
		cout << hex << (int)c.coef[j];
	}
	return out;
}
istream& operator>>(istream& in, bignumber& c)
{
	string str;
	in >> str;
	c = bignumber(str);
	return in;
}
int main() {
	srand(time(NULL));
	bignumber a;
	bignumber b;
	bignumber c;
	a.Input("9898913589077890");
	b.Input("27867");
	c = a / b;
	//for (int i = 0; i < a.maxlen; i++) { cout << a.coef[i]<<" "; }
	cout << endl;
	//for (int i = 0; i < c.maxlen; i++) { cout << c.coef[i] << " "; }
	c.Output();
}