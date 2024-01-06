#include<iostream>
#include <time.h>
#include "Chain.h"
using namespace std;


////////////////////////////ChainNode//////////////////////////////////////
template<class T>
class Chain;
template<class T>
class ChainNode
{
	friend class Chain<T>;
private:
	T data;
	ChainNode<T>* link;
public:
	ChainNode() {}
	ChainNode(const T& data)
	{
		this->data = data;
	}
	ChainNode(const T& data, ChainNode<T>* link)
	{
		this->data = data;//ChainNode 的資料
		this->link = link;//下一個node位址
	}
};

////////////////////////////Chain//////////////////////////////////////
template<class T>
class Chain
{
public:
	Chain() { first = &head; first->link = &head; itemCount = 0; }
	~Chain() {}
	bool IsEmpty()const { return first == head.link; }//判斷是否為空
	void sethead(const T& e) { head.data = e; head.link = &head; first = &head; }//設定第0項
	class ChainIterator //Chain 迭代器
	{
	public:
		friend class ChainNode<T>;
		ChainIterator(ChainNode<T>* startNode = 0) { current = startNode; };
		T& operator*() const { return current->data; }//多載*
		T* operator->() const { return &current->data; }//多載->
		ChainIterator& operator++(); // preincrement
		ChainIterator& operator++(int); // postincrement
		bool operator!=(const ChainIterator right) const   //多載*
		{
			return current != right.current;
		}
		bool operator==(const ChainIterator right) const  //多載==
		{
			return current == right.current;
		}
		bool operator&&(const ChainIterator right)//多載&&
		{
			return current == nullptr && right.current == nullptr;
		}
	private:
		ChainNode<T>* current;
	};
	Chain<T>::ChainIterator begin() const;//回傳起始項位置
	Chain<T>::ChainIterator end() const;//回傳最後項位置
	void InsertBack(const T& e);//在最後插入一項
	int getCount()const { return itemCount; }//回傳項數
private:
	ChainNode<T>* first;//第0項的指標
	ChainNode<T> head;//第0項
	int itemCount;//項數
};

template<class T>
typename Chain<T>::ChainIterator Chain<T>::begin() const
{
	return typename Chain<T>::ChainIterator(first->link);
}

template<class T>
typename Chain<T>::ChainIterator Chain<T>::end() const
{
	ChainNode<T>* p = first;
	while (p->link != first)//找最後一項
	{
		p = p->link;
	}
	return p->link;
}



template <class T>
typename Chain<T>::ChainIterator& Chain<T>::ChainIterator::operator++()
{
	current = current->link;
	return *this;
}

template <class T>
typename Chain<T>::ChainIterator& Chain<T>::ChainIterator::operator++(int)
{
	ChainIterator old = *this;
	current = current->link;
	return old;
}

template <class T>
void Chain<T>::InsertBack(const T& e)
{
	itemCount++;
	if (!IsEmpty())
	{
		ChainNode<T>* p = first;
		while (p->link != first)
		{
			p = p->link;
		}
		p->link = new ChainNode<T>(e, first);
	}
	else
		first->link = new ChainNode<T>(e, first);
}


////////////////////////////Polynomial//////////////////////////////////////
class Polynomial;
class Term {//定義Term物件
	friend Polynomial;
	friend ostream& operator<<(ostream& os, Polynomial& p);//多載輸出運算子
	friend istream& operator>>(istream& os, Polynomial& p);//多載輸入運算子
	Term Set(float c, int e) { coef = c; exp = e; return *this; };
private:
	float coef;//係數 
	int exp;//指數次方 
};

class Polynomial {//定義Polynomial類別
private:
	Chain<Term> poly;
public:
	Polynomial();
	Polynomial(const Polynomial&);
	~Polynomial();
	void newPoly(float c, int e);//新增一項
	Polynomial operator+(const Polynomial& b) const;//多載+
	Polynomial operator-(const Polynomial& b) const;//多載-
	Polynomial operator*(const Polynomial& b) const;//多載*
	float Evaluate(float x)const;//求值
	friend ostream& operator<<(ostream& os,  Polynomial& p);//多載輸出運算子
	friend istream& operator>>(istream& os,  Polynomial& p);//多載輸入運算子
};

Polynomial::Polynomial()
{
	Term tmp; 
	poly.sethead(tmp.Set(-1, -1));//將poly初始化
}
Polynomial::~Polynomial()
{
}
Polynomial::Polynomial(const Polynomial& a):poly(a.poly){}
void Polynomial::newPoly(float c,int e)
{
	Term tmp;
	poly.InsertBack(tmp.Set(c,e));
}

Polynomial Polynomial::operator+(const Polynomial& b) const
{
	Term tmp;
	Chain<Term>::ChainIterator ai = poly.begin(), bi = b.poly.begin();
	Polynomial c;
	while (1)//次方相同係數相加
	{
		if (ai->exp == bi->exp)
		{
			if (ai->exp == -1)return c;
			float sum = ai->coef + bi->coef;
			if (sum)
			{
				c.poly.InsertBack(tmp.Set(sum, ai->exp));
			}
			ai++;
			bi++;
		}
		else if (ai->exp < bi->exp)//poly1次方<poly2次方，poly2加入結果多項式中
		{
			c.poly.InsertBack(tmp.Set(bi->coef,bi->exp));
			bi++;
		}
		else//poly1次方>poly2次方，poly1加入結果多項式中
		{
			c.poly.InsertBack(tmp.Set(ai->coef, ai->exp));
			ai++;
		}
	}
	return c;
}
Polynomial Polynomial::operator-(const Polynomial& b) const
{
	Term tmp;
	Chain<Term>::ChainIterator ai = poly.begin(), bi = b.poly.begin();
	Polynomial c;
	while (1)
	{
		if (ai->exp == bi->exp)
		{
			if (ai->exp == -1)return c;
			float sum = ai->coef + (-1* bi->coef);
			if (sum)
			{
				c.poly.InsertBack(tmp.Set(sum, ai->exp));
			}
			ai++;
			bi++;
		}
		else if (ai->exp < bi->exp)//poly1次方<poly2次方，poly2加入結果多項式中
		{
			c.poly.InsertBack(tmp.Set(-bi->coef, bi->exp));
			bi++;
		}
		else//poly1次方>poly2次方，poly1加入結果多項式中
		{
			c.poly.InsertBack(tmp.Set(ai->coef, ai->exp));
			ai++;
		}
	}
	return c;
}
int hash_function(int x, int t, int* u)//計算線性哈希函數
{
	if (u[x % t] == 0 || u[x % t] - 1 == x)
	{
		u[x % t] = x + 1;
		return x % t;
	}
	else
	{
		for (int i = 1; i < t; i++)
		{
			if (u[(x + i) % t] == 0)
			{
				u[(x + i) % t] = x + 1;
				return (x + i) % t;
			}
		}
	}
}
Polynomial Polynomial::operator*(const Polynomial& b) const
{
	Polynomial res;
	int use_cap = poly.getCount()*b.poly.getCount(); //已用過的次方旗標陣列大小
	bool* use = new bool[use_cap]();//用於檢查是否有重複次方數的項
	int* hash = new int[use_cap]();//紀錄哈希表中對應的值
	Chain<Term>::ChainIterator ai = poly.begin(), bi = b.poly.begin();
	for (int i = 0; i < poly.getCount(); i++)//每項一一相乘
	{
		bi = b.poly.begin();
		for (int j = 0; j < b.poly.getCount(); j++)
		{
			int mult_exp = ai->exp+bi->exp;//相乘後的次方	
			if (use[hash_function(mult_exp, use_cap, hash)])//已用過，將相乘完的項使用加法功能加入多項式
			{
				Term tmp;
				tmp.coef = ai->coef * bi->coef;
				tmp.exp = mult_exp;
				Polynomial temp;
				temp.poly.InsertBack(tmp);
				res = res+temp;
			}
			else//未用過，將相乘完的項加入多項式
			{
				Term tmp;
				tmp.coef = ai->coef * bi->coef;
				tmp.exp = mult_exp;
				res.poly.InsertBack(tmp);
				use[mult_exp] = true;
			}
			bi++;
		}
		ai++;
	}
	return res;
}

float Polynomial:: Evaluate(float x)const
{
	Chain<Term>::ChainIterator ai = poly.begin();
	float total=0;
	while (ai != poly.end())
	{
		total += ai->coef * powf(x, ai->exp);
		ai++;
	}
	return total;
}
ostream& operator<<(ostream& os,  Polynomial& p) {//多載輸出運算子
	Chain<Term>::ChainIterator o = p.poly.begin();
	while(o!=p.poly.end())
	{
		if (o->exp == 0)
			os << abs(o->coef);
		else if (o->exp == 1)
			os <<abs( o->coef) << "X";
		else
			os << abs(o->coef) << "X^" << o->exp;
		if (++o != p.poly.end())
			if (o->coef < 0)
				os << " - ";
			else
				os << " + ";
	}
	return os;
}
istream& operator>>(istream& os, Polynomial& p) {//多載輸入運算子
	float coeftmp;//暫存coef
	int exptmp;//暫存exp
	os.ignore();//忽略(
	bool next = 0;
	while (1)
	{
		os >> coeftmp;//讀入係數
		os.ignore(2);//忽略 X^
		os >> exptmp;//讀入次方
		if(next)
			p.newPoly(-coeftmp, exptmp);
		else
			p.newPoly(coeftmp, exptmp);
		char c;
		os >> c;
		if (c == ')')break;//讀到括號尾跳出
		else if (c == '-')//判斷下一項是否為負
		{
			next = 1;
		}
	}
	os.get();//吃掉換行
	return os;
}

int main() {
	//宣告三個多項式物件
	Polynomial poly1;
	Polynomial poly2;
	Polynomial poly3;
	cout << "輸入poly1:";
	cin >> poly1;
	cout << "輸入poly2:";
	cin >> poly2;
	//相加
	poly3 = (poly1 + poly2);
	cout << "(" << poly1 << ") + (" << poly2 << ") = ";
	cout << poly3<<endl;
	//相減
	cout << "(" << poly1 << ") - (" << poly2 << ") = ";
	poly3 = (poly1 - poly2);
	cout << poly3<<endl;
	//相乘
	cout << "(" << poly1 << ") * (" << poly2 << ") = ";
	poly3 = (poly1 * poly2);
	cout << poly3 << endl;
	//求值
	cout << "Evaluate(2):";
	cout << poly3.Evaluate(2)<<endl;

	cout << "poly1=poly2:"<<endl;
	poly1 = poly2;
	cout << poly1 << endl<<poly2;
	return 0;
}