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
		this->data = data;//ChainNode �����
		this->link = link;//�U�@��node��}
	}
};

////////////////////////////Chain//////////////////////////////////////
template<class T>
class Chain
{
public:
	Chain() { first = &head; first->link = &head; itemCount = 0; }
	~Chain() {}
	bool IsEmpty()const { return first == head.link; }//�P�_�O�_����
	void sethead(const T& e) { head.data = e; head.link = &head; first = &head; }//�]�w��0��
	class ChainIterator //Chain ���N��
	{
	public:
		friend class ChainNode<T>;
		ChainIterator(ChainNode<T>* startNode = 0) { current = startNode; };
		T& operator*() const { return current->data; }//�h��*
		T* operator->() const { return &current->data; }//�h��->
		ChainIterator& operator++(); // preincrement
		ChainIterator& operator++(int); // postincrement
		bool operator!=(const ChainIterator right) const   //�h��*
		{
			return current != right.current;
		}
		bool operator==(const ChainIterator right) const  //�h��==
		{
			return current == right.current;
		}
		bool operator&&(const ChainIterator right)//�h��&&
		{
			return current == nullptr && right.current == nullptr;
		}
	private:
		ChainNode<T>* current;
	};
	Chain<T>::ChainIterator begin() const;//�^�ǰ_�l����m
	Chain<T>::ChainIterator end() const;//�^�ǳ̫ᶵ��m
	void InsertBack(const T& e);//�b�̫ᴡ�J�@��
	int getCount()const { return itemCount; }//�^�Ƕ���
private:
	ChainNode<T>* first;//��0��������
	ChainNode<T> head;//��0��
	int itemCount;//����
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
	while (p->link != first)//��̫�@��
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
class Term {//�w�qTerm����
	friend Polynomial;
	friend ostream& operator<<(ostream& os, Polynomial& p);//�h����X�B��l
	friend istream& operator>>(istream& os, Polynomial& p);//�h����J�B��l
	Term Set(float c, int e) { coef = c; exp = e; return *this; };
private:
	float coef;//�Y�� 
	int exp;//���Ʀ��� 
};

class Polynomial {//�w�qPolynomial���O
private:
	Chain<Term> poly;
public:
	Polynomial();
	Polynomial(const Polynomial&);
	~Polynomial();
	void newPoly(float c, int e);//�s�W�@��
	Polynomial operator+(const Polynomial& b) const;//�h��+
	Polynomial operator-(const Polynomial& b) const;//�h��-
	Polynomial operator*(const Polynomial& b) const;//�h��*
	float Evaluate(float x)const;//�D��
	friend ostream& operator<<(ostream& os,  Polynomial& p);//�h����X�B��l
	friend istream& operator>>(istream& os,  Polynomial& p);//�h����J�B��l
};

Polynomial::Polynomial()
{
	Term tmp; 
	poly.sethead(tmp.Set(-1, -1));//�Npoly��l��
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
	while (1)//����ۦP�Y�Ƭۥ[
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
		else if (ai->exp < bi->exp)//poly1����<poly2����Apoly2�[�J���G�h������
		{
			c.poly.InsertBack(tmp.Set(bi->coef,bi->exp));
			bi++;
		}
		else//poly1����>poly2����Apoly1�[�J���G�h������
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
		else if (ai->exp < bi->exp)//poly1����<poly2����Apoly2�[�J���G�h������
		{
			c.poly.InsertBack(tmp.Set(-bi->coef, bi->exp));
			bi++;
		}
		else//poly1����>poly2����Apoly1�[�J���G�h������
		{
			c.poly.InsertBack(tmp.Set(ai->coef, ai->exp));
			ai++;
		}
	}
	return c;
}
int hash_function(int x, int t, int* u)//�p��u�ʫ��ƨ��
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
	int use_cap = poly.getCount()*b.poly.getCount(); //�w�ιL������X�а}�C�j�p
	bool* use = new bool[use_cap]();//�Ω��ˬd�O�_�����Ʀ���ƪ���
	int* hash = new int[use_cap]();//�������ƪ���������
	Chain<Term>::ChainIterator ai = poly.begin(), bi = b.poly.begin();
	for (int i = 0; i < poly.getCount(); i++)//�C���@�@�ۭ�
	{
		bi = b.poly.begin();
		for (int j = 0; j < b.poly.getCount(); j++)
		{
			int mult_exp = ai->exp+bi->exp;//�ۭ��᪺����	
			if (use[hash_function(mult_exp, use_cap, hash)])//�w�ιL�A�N�ۭ��������ϥΥ[�k�\��[�J�h����
			{
				Term tmp;
				tmp.coef = ai->coef * bi->coef;
				tmp.exp = mult_exp;
				Polynomial temp;
				temp.poly.InsertBack(tmp);
				res = res+temp;
			}
			else//���ιL�A�N�ۭ��������[�J�h����
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
ostream& operator<<(ostream& os,  Polynomial& p) {//�h����X�B��l
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
istream& operator>>(istream& os, Polynomial& p) {//�h����J�B��l
	float coeftmp;//�Ȧscoef
	int exptmp;//�Ȧsexp
	os.ignore();//����(
	bool next = 0;
	while (1)
	{
		os >> coeftmp;//Ū�J�Y��
		os.ignore(2);//���� X^
		os >> exptmp;//Ū�J����
		if(next)
			p.newPoly(-coeftmp, exptmp);
		else
			p.newPoly(coeftmp, exptmp);
		char c;
		os >> c;
		if (c == ')')break;//Ū��A�������X
		else if (c == '-')//�P�_�U�@���O�_���t
		{
			next = 1;
		}
	}
	os.get();//�Y������
	return os;
}

int main() {
	//�ŧi�T�Ӧh��������
	Polynomial poly1;
	Polynomial poly2;
	Polynomial poly3;
	cout << "��Jpoly1:";
	cin >> poly1;
	cout << "��Jpoly2:";
	cin >> poly2;
	//�ۥ[
	poly3 = (poly1 + poly2);
	cout << "(" << poly1 << ") + (" << poly2 << ") = ";
	cout << poly3<<endl;
	//�۴�
	cout << "(" << poly1 << ") - (" << poly2 << ") = ";
	poly3 = (poly1 - poly2);
	cout << poly3<<endl;
	//�ۭ�
	cout << "(" << poly1 << ") * (" << poly2 << ") = ";
	poly3 = (poly1 * poly2);
	cout << poly3 << endl;
	//�D��
	cout << "Evaluate(2):";
	cout << poly3.Evaluate(2)<<endl;

	cout << "poly1=poly2:"<<endl;
	poly1 = poly2;
	cout << poly1 << endl<<poly2;
	return 0;
}