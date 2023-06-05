#include <fstream>
#include <iostream>
#include <string>
#include <algorithm> 
//https://www.online-cpp.com/
typedef unsigned int   BASE;
typedef unsigned long long int  DBASE;
#define BASE_SIZE (sizeof(BASE)*8)
using namespace std;
class bignumber {
protected:
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
		for (int i = 0; i < len; i++) {coef[i] = c.coef[i];}
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
		if (len == 0) { len++; }
		coef = new BASE[len];
		for (int i = 0; i < len; i++) { this->coef[i] = 0; }
		maxlen = len;
		for (int i = start; i < l; i++) {
			bignumber t(1);
			t.coef[0] = (BASE)str[i] - (BASE)'0';
			*this = (*this * (BASE)c) + t;
		}
	}
	void Output(BASE c = 10) {
		string str;
		bignumber u = *this;
		int j = 0;
		while (u != 0) {
			BASE t = u % c;
			t = (BASE)t + (BASE)'0';
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
	bignumber& operator = (const bignumber c) {
		if (this != &c) {
			len = c.len;
			maxlen = c.maxlen;
			coef = new BASE[1];
			delete[] coef;
			coef = new BASE[len];
			for (int i = 0; i < len; i++) { coef[i] = 0; }
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
		while (j < t) {
			tmp = (DBASE)coef[j] + (DBASE)v.coef[j] + (DBASE)k;
			w.coef[j] = (BASE)tmp;
			tmp = tmp >> BASE_SIZE;
			k = (BASE)(tmp);
			j++;
		}
		while (j < len) {
			tmp = (DBASE)coef[j] + (DBASE)k;
			w.coef[j] = (BASE)tmp;
			tmp = tmp >> BASE_SIZE;
			k = (BASE)(tmp);
			j++;
		}
		while (j < v.len) {
			tmp = (DBASE)v.coef[j] + (DBASE)k;
			w.coef[j] = (BASE)tmp;
			tmp = tmp >> BASE_SIZE;
			k = (BASE)(tmp);
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
		if (*this < v) { cout << "Отрицательная разность"<<endl; }
		else {
			int j = 0;
			int k = 0;
			while (j < v.len)
			{	
				tmp = 1;
				tmp = tmp << BASE_SIZE;
				tmp = (tmp | coef[j]);
				tmp = tmp - v.coef[j] - k;
				w.coef[j] = (BASE)tmp;
				k = !(tmp >> BASE_SIZE);
				j++;
			}
			while (j < len) {
				tmp = 1;
				tmp = tmp << BASE_SIZE;
				tmp = (tmp | coef[j]);
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
		if (*this == 1 || v == 1) { w = *this; return w; }
		int j = 0;
		int k = 0;
		DBASE tmp;
		while (j < len) {
			tmp = ((DBASE)coef[j] * (DBASE)v) + (DBASE)k;
			w.coef[j] = (BASE)tmp;
			k = (BASE)(tmp >> BASE_SIZE);
			j++;
		}
		w.coef[j] = (BASE)k;
		w.len_norm();
		return w;
	}
	bignumber& operator *= (BASE v) {
		*this = *this * v;
		return *this;
	}
	bignumber operator / (BASE v) {
		bignumber q(len);
		if (v <= 0) { cout << "Деление на 0!!!"; }
		else if (*this == 1) { return q; }
		else if (v == 1) { q = *this;  return q; }
		else {
			int j = 0;
			DBASE r = 0;
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
		DBASE r = 0;
		if (v <= 0) { cout << "Деление на 0!!!"; }
		else if (*this == 1) { return 1; }
		else if (v == 1) { return 0; }
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
		if (*this == 0 || v == 0) { return w; }
		if (*this == 1) { w = v; return w; }
		if (v == 1) { return w = *this; return w; }
		//for (int i = 0; i < w.maxlen; i++) { w.coef[i] = 0; }
		int m = len;
		int n = v.len;
		//BASE k = 0;
		DBASE tmp = 0;
		for (int j = 0; j < n; j++) {
			if (v.coef[j] != 0) {
				BASE k = (BASE)0;
				for (int i = 0; i < m; i++) {
					tmp = (DBASE)coef[i] * (DBASE)v.coef[j] + (DBASE)w.coef[i + j] + (DBASE)k;
					w.coef[i + j] = (BASE)tmp;
					tmp = (DBASE)tmp >> BASE_SIZE;
					k = (BASE)tmp;
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
		if (*this < v) { bignumber q(this->len); return q; }
		if (*this == v) { bignumber q(this->len);q.coef[0]=1; return q; }
		bignumber v1 = v;
		DBASE b = 1;
		b = ((DBASE)b << (BASE_SIZE));//b-основание
		bignumber u1 = *this;
		if (v.len == 1) { return u1 / v1.coef[0]; }

		int n = v1.len;
		int m = u1.len - v1.len;//размерность частного
		DBASE tmp = (DBASE)v1.coef[n - 1] + 1;//нормализация
		tmp = b / tmp;
		BASE d = (BASE)tmp;
		u1 = u1 * d;
		v1 = v1 * d;
		if(u1.len==m+n){ u1.coef[len] = 0; }//если размерность после нормализации 
		int j = m;                           // не изменилась
		bignumber w(m + 1);
		//for (int i1 = 0; i1 < n; i1++) {
		//	cout << u1.coef[i1]<<" ";
		//}
		//cout<< endl;
		DBASE q = 0;
		BASE jjj;
		DBASE r = 0;
		DBASE tmp1 = 0;
		DBASE tmp2 = 0;
		while (j >= 0) {
			q = 0;
			r = 0;
			q = ((DBASE)u1.coef[j + n] * b + (DBASE)u1.coef[j + n - 1]) / (DBASE)v1.coef[n - 1];
			//cout << (int)q; cout << endl;

			r = ((DBASE)u1.coef[j + n] * b + (DBASE)u1.coef[j + n - 1]) % (DBASE)v1.coef[n - 1];
			while (r < b) {
				tmp1 = (DBASE)b * (DBASE)r + (DBASE)u1.coef[j + n - 2];
				tmp2 = (DBASE)v1.coef[n - 2] * q;
				if ((q == b) || (tmp2 > tmp1)) {
					q = q - 1;
					r = (DBASE)r + (DBASE)v1.coef[n - 1];
				}
				else break;
			}
			BASE q1 = (BASE)q;//очередной разряд частного
			int i = 0;
			BASE k2 = 0;
			BASE k1 = 0;
			tmp1 = 0;
			tmp2 = 0;
			while (i < n)
			{//умножаем делимое на разряд
				tmp1 = (DBASE)v1.coef[i] * (DBASE)q1 + (DBASE)k1;
				jjj = (BASE)tmp1;
				tmp1 = tmp1 >> BASE_SIZE;
				k1 = (BASE)(tmp1);//вычитаем
				tmp2 = (b | (DBASE)u1.coef[i + j]) - (DBASE)jjj - (DBASE)k2;
				u1.coef[j + i] = (BASE)tmp2;
				tmp2 = tmp2 >> BASE_SIZE;
				k2 = !(tmp2);
				i++;
            }
			// разбираемся с последним
			tmp = ((DBASE)b | (DBASE)u1.coef[n + j]) - (DBASE)k1 - (DBASE)k2;
			u1.coef[n + j] = (BASE)tmp;
			tmp = tmp >> BASE_SIZE;
			k2 = !(tmp);
			tmp = 0;
			if (k2 == 1) {// если правильный раряд на 1 меньше
				k2 = 0;
				q1--;
				i = 0;
				/*for (int i1 = 0; i1 < n + m + 1; i1++) {
					cout << (int)u1.coef[i1]<<" ";
				}
				cout << endl;*/
				while (i < n) {
					tmp = (DBASE)u1.coef[j + i] + (DBASE)v1.coef[i] + (DBASE)k2;
					u1.coef[j + i] = (BASE)tmp;
					tmp = tmp >> BASE_SIZE;
					k2 = (BASE)(tmp);
					i++;
				}
				u1.coef[j + n] = u1.coef[j + n] + k2;
				/*for (int i1 = 0; i1 < n + m + 1; i1++) {
					cout << (int)u1.coef[i1]<<" ";
				}
				cout << endl;*/
			}
			w.coef[j] = q1;
			//cout << "g  " << q << "  q1  " << q1 << endl;
			//for (int i1 = 0; i1 < n + m + 1; i1++) {
			//	cout << u1.coef[i1]<<" ";
			//}
			//cout<< endl;
			j--;
		}
		w.len_norm();

		return w;

	}
	bignumber operator % (bignumber v) {
		if (*this < v) { bignumber q = *this; return q; }
		if (*this == v) { bignumber q(len); return q; }
		bignumber v1 = v;
		DBASE b = 1;
		b = ((DBASE)b << (BASE_SIZE));
		bignumber u1 = *this;
		if (v1.len == 1) {
			v1.coef[0] = u1 % v1.coef[0];
			return v1;
		}
		int n = v1.len;
		int m = u1.len - v1.len;
		DBASE tmp = (DBASE)v1.coef[n - 1] + 1;
		tmp = b / tmp;
		BASE d = (BASE)tmp;
		u1 = u1 * d;
		v1 = v1 * d;
		if (u1.len == m + n) { u1.coef[len] = 0; }
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
			q = ((DBASE)u1.coef[j + n] * b + (DBASE)u1.coef[j + n - 1]) / (DBASE)v1.coef[n - 1];
			r = ((DBASE)u1.coef[j + n] * b + (DBASE)u1.coef[j + n - 1]) % (DBASE)v1.coef[n - 1];
			while (r < b) {
				tmp1 = (DBASE)b * (DBASE)r + (DBASE)u1.coef[j + n - 2];
				tmp2 = (DBASE)v1.coef[n - 2] * q;
				if ((q == b) || (tmp2 > tmp1)) {
					q = q - 1;
					r = r + (DBASE)v1.coef[n - 1];
				}
				else break;

			}
			BASE q1 = (BASE)q;
			int i = 0;
			BASE k2 = 0;
			BASE k1 = 0;
			tmp1 = 0;
			tmp2 = 0;
			while (i < n)
			{
				tmp1 = (DBASE)v1.coef[i] * q1 + k1;
				jjj = (BASE)tmp1;
				tmp1 = tmp1 >> BASE_SIZE;
				k1 = (BASE)(tmp1);
				tmp2 = (b | (DBASE)u1.coef[i + j]) - jjj - k2;
				u1.coef[j + i] = (BASE)tmp2;
				tmp2 = tmp2 >> BASE_SIZE;
				k2 = !(tmp2);
				i++;
			}

			tmp = ((DBASE)b | (DBASE)u1.coef[n + j]) - (DBASE)k1 - (DBASE)k2;
			u1.coef[n + j] = (BASE)tmp;
			tmp = tmp >> BASE_SIZE;
			k2 = !(tmp);
			tmp = 0;
			if (k2 == 1) {
				k2 = 0;
				q1--;
				i = 0;
				while (i < n) {
					tmp = (DBASE)u1.coef[j + i] + v1.coef[i] + k2;
					u1.coef[j + i] = (BASE)tmp;
					tmp = tmp >> BASE_SIZE;
					k2 = (BASE)(tmp);
					i++;
				}
				u1.coef[j + n] = u1.coef[j + n] + k2;
			}
			w.coef[j] = q1;
			j--;
		}

		//w.len_norm();
		u1.len_norm();
		//u1.Output();
		u1 = u1 / d; 
		return u1;

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
	setlocale(LC_ALL, "Russian");
	/*
	bignumber a;
	bignumber b;
	bignumber c;
	bignumber d;
	a.Input("345743578237437346457437637");
	b.Input("345345745457");
	//c.Input("1875252885408677288903146384940492984870274286550455880847888785903332612864770411");
	c = a/b;
	d = a % b;
	//d.Input("19441077529047");
	c.Output();
	d.Output();
	cout << "(A == ((C * B) + D)) = " << (a == ((c * b) + d)) << endl;
	cout << "((A - D) == (C * B)) = " << ((a - d) == (c * b)) << endl;
	cout << "(D < B) = " << (d < b) << endl;
	return 0;
	*/
	
	int M = 1000;
	int T = 1000;
	int n=0;
	int m=0;
	bignumber A, B, C, D;
	for (int i = 0; i < 10; i++) {
		T = 1000;
		do {
			//cout << T << endl;
			int n = rand() % M + 1;
			int m = rand() % M + 1;
			A = bignumber(n, 1);
			B = bignumber(m, 1);
			C = A / B;
			D = A % B;

			//cout << "A- "; A.Output(); cout << endl;
			//cout << "B- "; B.Output(); cout << endl;
			//cout << "C- "; C.Output(); cout << endl;
			//cout << "D- "; D.Output(); cout << endl;
			//cout << "(A == ((C * B) + D)) = " << (A == ((C * B) + D)) << endl;
			//cout << "((A - D) == (C * B)) = " << ((A - D) == (C * B)) << endl;
			//cout << "(D < B) = " << (D < B) << endl;
			

		} while ((A == ((C * B) + D)) && ((A - D) == (C * B)) && (D < B) && (--T));
		cout <<"тест - "<<i+1<<"  " << (1000 - T) <<" " << "итераций обработано без ошибок" << endl;
	}
}
