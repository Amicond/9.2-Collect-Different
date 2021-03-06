// 9 Collect Third Terms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iomanip>
const int min_op_amount = 2;
using namespace std;

const string delim = "\\";
const string out_res = "results";

void findCos(int **m, int  size, int n, int &da, int &db)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			if (m[i][j] == n)
			{
				da = j;
				db = i;
				break;
			}
		}
	da = da - size / 2;
	db = -db + size / 2;
}




//new structures
struct term
{
	int len;
	int order;
	char ops[10];
	int nums[10];
	double value;

	void decompose(string s, double val)
	{
		int temp = s.find_first_not_of("SJpmz");
		len = temp - 1;
		for (int i = 0; i < len; i++)
			ops[i] = s[i + 1];
		istringstream is;
		char tc;
		is.str(s.substr(temp));
		for (int i = 0; i <len; i++)
		{
			is >> nums[i];
			is >> tc;
		}
		value = val;

	}
	void setOrder(int Ord)
	{
		order = Ord;
	}
	bool operator==(term next)
	{
		if (len != next.len)
			return false;
		for (int i = 0; i < len; i++)
		{
			if (ops[i] != next.ops[i])
				return false;
			if (nums[i] != next.nums[i])
				return false;
		}
		return true;
	}
};

struct a_op
{
	int n;//general length
	vector<char> names; //plus or minus
	vector<int> node; //to calc shift
	double coeff;
	int order;
	bool operator==(a_op sec)
	{
		if (n != sec.n)
			return false;
		for (int i = 0; i < n; i++)
		{
			if (names[i] != sec.names[i])
				return false;
			if (node[i] != sec.node[i])
				return false;
		}
		return true;
	}


};

struct Cos
{
	double factor;
	vector<int> ka, kb;
	bool operator ==(Cos c2)
	{
		int s1, s2;
		s1 = ka.size();
		s2 = c2.ka.size();
		if (s1 != s2)
			return false;
		s1 = kb.size();
		s2 = c2.kb.size();
		if (s1 != s2)
			return false;
		for (int i = 0; i < ka.size(); i++)
		{
			if (ka[i] != c2.ka[i] || kb[i] != c2.kb[i])
				return false;
		}
		return true;
	}
};

struct correction
{
	vector<char> in;
	char out[2];
	vector<Cos> cs;
	bool operator ==(correction c2)
	{
		for (int i = 0; i < in.size(); i++)
		{
			if (in[i] != c2.in[i])
				return false;
		}
		if (out[0] != c2.out[0] || out[1] != c2.out[1])
			return false;
		return true;
	}
	void add(Cos new_cos)
	{
		vector<Cos>::iterator it;
		it = find(cs.begin(), cs.end(), new_cos);
		if (it != cs.end())
		{
			it->factor += new_cos.factor;
		}
		else
			cs.push_back(new_cos);
	}
	void print(ofstream & outF, int num)
	{
		if (num == 1)//for sz terms
		{
			outF << "Subscript[ap,k]*Subscript[a,k]*" << cs[0].factor;
		}

		if (num == 2)  //for pm terms
		{
			if (out[0] == 'p')
				outF << "Subscript[ap,k]*";
			else
				outF << "Subscript[a,k]*";

			if (out[0] == 'p'&&out[1] == 'p')
				outF << "Subscript[ap,-k]";
			if (out[0] == 'p'&&out[1] == 'm')
				outF << "Subscript[a,k]";
			if (out[0] == 'm'&&out[1] == 'p')
				outF << "Subscript[ap,k]";
			if (out[0] == 'm'&&out[1] == 'm')
				outF << "Subscript[a,-k]";

			outF << "*(";
			for (int i = 0; i < cs.size(); i++)
			{
				outF << cs[i].factor << "*Cos[";
				if (out[0] == 'p')
					outF << "Subscript[k,a]*" << cs[i].ka[0] << "+Subscript[k,b]*" << cs[i].kb[0] << "+";
				else
					outF << "-Subscript[k,a]*" << cs[i].ka[0] << "-Subscript[k,b]*" << cs[i].kb[0] << "+";
				///////////////////////////////////////////////////////////////////////////////////////////////
				// cos
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (out[0] == 'p')
					outF << "-Subscript[k,a]*" << cs[i].ka[1] << "-Subscript[k,b]*" << cs[i].kb[1];
				else
					outF << "Subscript[k,a]*" << cs[i].ka[1] << "+Subscript[k,b]*" << cs[i].kb[1];

				outF << "]";
				if (i != cs.size() - 1)
				{
					outF << "+";
				}
			}
			outF << ")";

		}
		if (num == 3)  //for triple terms
		{
			if (in[0] == 'p')
				outF << "Subscript[ap,k1]*";
			else
				outF << "Subscript[a,k1]*";
			if (out[0] == 'p')
				outF << "Subscript[ap,k2]*";
			else
				outF << "Subscript[a,k2]*";
			if (out[1] == 'p')
				outF << "Subscript[ap,k3]*";
			else
				outF << "Subscript[a,k3]*";
			//temp
			//char c=
			//end temp
			outF << "DiracDelta[" << ((in[0] == 'p') ? "k1" : "-k1") << "+" << (out[0] == 'p' ? "k2" : "-k2") << "+" << (out[1] == 'p' ? "k3" : "-k3") << "]*(";
			for (int i = 0; i < cs.size(); i++)
			{
				outF << cs[i].factor << "*Cos[";
				if (in[0] == 'p')
					outF << "Subscript[k1,a]*" << cs[i].ka[0] << "+Subscript[k1,b]*" << cs[i].kb[0] << "+";
				else
					outF << "Subscript[k1,a]*" << -cs[i].ka[0] << "+Subscript[k1,b]*" << -cs[i].kb[0] << "+";
				if (out[0] == 'p')
					outF << "Subscript[k2,a]*" << cs[i].ka[1] << "+Subscript[k2,b]*" << cs[i].kb[1] << "+";
				else
					outF << "Subscript[k2,a]*" << -cs[i].ka[1] << "+Subscript[k2,b]*" << -cs[i].kb[1] << "+";
				if (out[1] == 'p')
					outF << "Subscript[k3,a]*" << cs[i].ka[1] << "+Subscript[k3,b]*" << cs[i].kb[1];
				else
					outF << "Subscript[k3,a]*" << -cs[i].ka[1] << "+Subscript[k3,b]*" << -cs[i].kb[1];
				outF << "]";
				if (i != cs.size() - 1)
				{
					outF << "+";
				}
			}
			outF << ")";

		}
		if (num == 4)  //for quatro terms
		{
			if (in[0] == 'p'&&in[1] == 'p')
				outF << "Subscript[Fp,k]";
			if (in[0] == 'm'&&in[1] == 'm')
				outF << "Subscript[Fm,k]";
			if (in[0] == 'p'&&in[1] == 'm')
				outF << "Subscript[G,k]";
			if (in[0] == 'm'&&in[1] == 'p')
				outF << "Subscript[G,k]";
			outF << "*";
			if (out[0] == 'p'&&out[1] == 'p')
				outF << "Subscript[Fp,q]";
			if (out[0] == 'm'&&out[1] == 'm')
				outF << "Subscript[Fm,q]";
			if ((out[0] == 'p'&&out[1] == 'm') || (out[0] == 'm'&&out[1] == 'p'))
				outF << "Subscript[G,q]";
			outF << "*(";
			for (int i = 0; i < cs.size(); i++)
			{
				outF << cs[i].factor << "*Cos[";
				outF << "ka*" << cs[i].ka[0] << +"+kb*" << cs[i].kb[0] << "+";
				if (in[1] == in[0])
					outF << "ka*" << cs[i].ka[1] << +"+kb*" << cs[i].kb[1];
				else
					outF << "ka*" << -cs[i].ka[1] << +"+kb*" << -cs[i].kb[1];
				outF << "+";
				outF << "qa*" << cs[i].ka[2] << +"+qb*" << cs[i].kb[2];
				outF << "+";
				if (out[1] == out[2])
					outF << "qa*" << cs[i].ka[3] << +"+qb*" << cs[i].kb[3];
				else
					outF << "qa*" << -cs[i].ka[3] << +"+qb*" << -cs[i].kb[3];
				outF << "]";
				if (i != cs.size() - 1)
				{
					outF << "+";
				}
			}
			outF << ")";
		}


	}
};

int mask4[6][4] = { { 1, 1, 0, 0 }, { 1, 0, 1, 0 }, { 1, 0, 0, 1 }, { 0, 1, 1, 0 }, { 0, 1, 0, 1 }, { 0, 0, 1, 1 } };
int mask3[3][3] = { { 1, 1, 0 }, { 1, 0, 1 }, { 0, 1, 1 } };
int out_pair[6][3] = { { 0, 1, 2 }, { 1, 0, 2 }, { 1, 2, 0 }, { 0, 2, 1 }, { 2, 0, 1 }, { 2, 1, 0 } };
string green_func[4] = { "G", "GP", "F", "FP" };
string momenta_names[3] = { "k", "k1", "k2" };
class converter
{

	int **m;
	int matrix_size;
	vector<term> shorterTerms;
	vector<a_op> a_ops_l;
	vector<correction> cors;
	double factor;
	int a_amount;
public:
	void set(double F, int A_amount, int **M, int Matrix_size)
	{
		factor = F;
		a_amount = A_amount;
		m = M;
		matrix_size = Matrix_size;
	}
	void decomposeTerm(term in)
	{
		if (a_amount == 1)
			decomposeTerm1(in);
		if (a_amount == 2)
			decomposeTerm2(in);
		if (a_amount == 3)
			decomposeTerm3(in);
		if (a_amount == 4)
			decomposeTerm4(in);
	}
private:
	void insertShortTerm(term cur)
	{
		vector<term>::iterator it;
		it = find(shorterTerms.begin(), shorterTerms.end(), cur);
		if (it != shorterTerms.end())
			it->value += cur.value;
		else
			shorterTerms.push_back(cur);
	}
	void decomposeTerm1(term in)
	{
		term cur;
		cur.len = 1;
		double coeff = 1;
		for (int i = 0; i < in.len - 1; i++)
			coeff *= factor;
		cur.order = in.order;
		cur.value = in.value*coeff;
		cur.ops[0] = in.ops[0];
		cur.nums[0] = 0;
		insertShortTerm(cur);

	}
	void decomposeTerm2(term in)
	{
		term cur;
		double coeff = 1;
		for (int i = 0; i < in.len - 2; i++)
			coeff *= factor;
		cur.len = 2;
		cur.order = in.order;
		cur.value = in.value*coeff / cur.len;
		cur.ops[0] = in.ops[0];
		cur.nums[0] = in.nums[0];
		for (int i = 1; i < in.len; i++) //���� ������ pm ��������
			if (in.ops[i] != 'z')
			{
				cur.nums[1] = in.nums[i];
				cur.ops[1] = in.ops[i];
				insertShortTerm(cur);
				break;
			}

	}
	void decomposeTerm3(term in)
	{
		term cur;
		cur.len = -1;
		double coeff = 1;

		int pm = 0;
		for (int i = 0; i < in.len; i++)
			if (in.ops[i] != 'z')
				pm++;
		if (pm == 1)
		{
			for (int i = 0; i < in.len - 2; i++)
				coeff *= factor;
			cur.len = 2;
			cur.order = in.order;
			cur.value = in.value*coeff / cur.len;
			cur.ops[0] = in.ops[0];
			cur.nums[0] = in.nums[0];
			if (in.ops[0] == 'z')
			{
				for (int i = 1; i < in.len; i++) //���� ������ pm ��������
					if (in.ops[i] != 'z')
					{
						cur.nums[1] = in.nums[i];
						cur.ops[1] = in.ops[i];
						insertShortTerm(cur);
						break;
					}
			}
			else //�������� ��� ��������� z ���������
			{
				for (int i = 1; i < in.len; i++)
				{
					cur.ops[1] = in.ops[1];
					cur.nums[1] = in.nums[1];
					insertShortTerm(cur);
				}
			}
		}
		if (pm == 3)
		{
			for (int i = 0; i < in.len - 3; i++)
				coeff *= factor;
			cur.len = 3;
			cur.order = in.order;
			cur.value = in.value*coeff / cur.len;
			int index = 0;
			for (int i = 0; i < in.len; i++)
			{
				if (in.ops[i] != 'z')
				{
					cur.ops[index] = in.ops[i];
					cur.nums[index] = in.nums[i];
					index++;
				}
			}
			insertShortTerm(cur);
		}

	}
	void decomposeTerm4(term in)
	{
		term cur;
		cur.len = -1;
		double coeff = 1;

		int pm = 0;
		for (int i = 0; i < in.len; i++)
			if (in.ops[i] != 'z')
				pm++;
		if (pm == 0)
		{
			for (int i = 0; i < in.len - 2; i++)
				coeff *= factor;
			cur.len = 2;
			cur.value = coeff*in.value / cur.len;
			cur.order = in.order;
			for (int i = 1; i < in.len; i++)//�������� ��� ������ sz, ��������� ��������� ������ i
			{

				cur.ops[0] = 'z';
				cur.ops[1] = 'z';
				cur.nums[0] = in.nums[0];
				cur.nums[1] = in.nums[i];

				//���������
				insertShortTerm(cur);
			}
		}
		if (pm == 2)
		{
			for (int i = 0; i < in.len - 3; i++)
				coeff *= factor;
			cur.len = 3;
			cur.value = coeff*in.value / cur.len;
			cur.order = in.order;
			cur.nums[0] = in.nums[0];
			cur.ops[0] = in.ops[0];
			if (cur.ops[0] == 'z')//��������� ������ pm-����� �� ����������
			{
				int index = 1;
				for (int i = 1; i < in.len; i++)
				{
					if (in.ops[i] != 'z')
					{
						cur.ops[index] = in.ops[i];
						cur.nums[index] = in.nums[i];
						index++;
					}
				}
				//���������
				insertShortTerm(cur);
			}
			else //�� ������� �������� ��� z-�����
			{
				int index = 0;
				for (int i = 1; i < in.len; i++) //���� ������ pm ��������
					if (in.ops[i] != 'z')
					{
						index = i;
						break;
					}
				for (int i = 1; i < in.len; i++)
				{
					if (index < i)
					{
						cur.ops[1] = in.ops[index];
						cur.ops[2] = in.ops[i];
						cur.nums[1] = in.nums[index];
						cur.nums[2] = in.nums[i];
					}
					if (index > i)
					{
						cur.ops[1] = in.ops[i];
						cur.ops[2] = in.ops[index];
						cur.nums[1] = in.nums[i];
						cur.nums[2] = in.nums[index];
					}
					//���������
					if (i != index)
						insertShortTerm(cur);
				}
			}

		}

		if (pm == 4)
		{
			for (int i = 0; i < in.len - 4; i++)
				coeff *= factor;
			cur.len = 4;
			cur.order = in.order;
			cur.value = in.value*coeff / cur.len;
			int index = 0;
			for (int i = 0; i < in.len; i++)
			{
				if (in.ops[i] != 'z')
				{
					cur.ops[index] = in.ops[i];
					cur.nums[index] = in.nums[i];
					index++;
				}
			}
			//���������
			insertShortTerm(cur);
		}
	}
public:
	void convertToAop()
	{
		int pm;
		a_op cur;
		for (int i = 0; i < shorterTerms.size(); i++)
		{
			pm = 0;
			cur.names.clear();
			cur.node.clear();

			for (int j = 0; j < shorterTerms[i].len; j++)
			{
				if (shorterTerms[i].ops[j] == 'z')
				{
					cur.names.push_back('p');
					cur.names.push_back('m');
					cur.node.push_back(shorterTerms[i].nums[j]);
					cur.node.push_back(shorterTerms[i].nums[j]);
				}
				else//pm-case
				{
					cur.names.push_back(shorterTerms[i].ops[j]);
					cur.node.push_back(shorterTerms[i].nums[j]);
				}
			}

			//vector<a_op>::iterator it = find(a_ops_l.begin(), a_ops_l.end(), cur);
			//if (it != a_ops_l.end())
			cur.coeff = shorterTerms[i].value;
			cur.order = shorterTerms[i].order;
			a_ops_l.push_back(cur);
		}
	}

	void convertToCorrections()
	{
		correction cur;
		int index, index2;
		if (a_amount == 1)//��� Sz
		{
			for (int i = 0; i < a_ops_l.size(); i++)
			{
				cur.cs.clear();
				cur.out[0] = a_ops_l[i].names[0];
				cur.out[1] = a_ops_l[i].names[1];
				Cos cur_cos;
				cur_cos.factor = a_ops_l[i].coeff;

				cur_cos.ka.push_back(0);
				cur_cos.kb.push_back(0);

				cur_cos.ka.push_back(0);
				cur_cos.kb.push_back(0);
				vector<correction>::iterator it = find(cors.begin(), cors.end(), cur);
				if (it != cors.end())
				{
					it->cs.push_back(cur_cos);
				}
				else
				{
					cur.cs.push_back(cur_cos);
					cors.push_back(cur);
				}
			}
		}
		if (a_amount == 2)
		{
			for (int i = 0; i < a_ops_l.size(); i++)
			{
				cur.cs.clear();
				Cos cur_cos;
				cur_cos.factor = a_ops_l[i].coeff;
				int da, db;
				cur.out[0] = a_ops_l[i].names[0];
				cur.out[1] = a_ops_l[i].names[1];

				findCos(m, matrix_size, a_ops_l[i].node[0], da, db);
				cur_cos.ka.push_back(da);
				cur_cos.kb.push_back(db);
				findCos(m, matrix_size, a_ops_l[i].node[1], da, db);
				cur_cos.ka.push_back(da);
				cur_cos.kb.push_back(db);


				//end eval
				vector<correction>::iterator it = find(cors.begin(), cors.end(), cur);
				if (it != cors.end())
				{
					it->cs.push_back(cur_cos);
				}
				else
				{
					cur.cs.push_back(cur_cos);
					cors.push_back(cur);
				}

			}
		}
		if (a_amount == 3)
		{
			for (int i = 0; i < a_ops_l.size(); i++)
			{

				for (int j = 0; j < 3; j++) //3 ������� ������� ������� ��������
				{
					cur.cs.clear();
					cur.in.clear();
					index = 0;
					int nums[3];
					for (int k = 0; k < 3; k++)//��������� ����� �������� ����� �������
					{
						if (mask3[j][k] == 0)//��������� � in
						{
							cur.in.push_back(a_ops_l[i].names[k]);
							nums[0] = k;
						}
						else
						{
							nums[1 + index] = k;
							cur.out[index++] = a_ops_l[i].names[k];
						}
					}
					//eval cos for cur term
					Cos cur_cos;
					cur_cos.factor = a_ops_l[i].coeff;
					int da, db;

					findCos(m, matrix_size, a_ops_l[i].node[nums[0]], da, db);
					cur_cos.ka.push_back(da);
					cur_cos.kb.push_back(db);
					findCos(m, matrix_size, a_ops_l[i].node[nums[1]], da, db);
					cur_cos.ka.push_back(da);
					cur_cos.kb.push_back(db);
					findCos(m, matrix_size, a_ops_l[i].node[nums[2]], da, db);
					cur_cos.ka.push_back(da);
					cur_cos.kb.push_back(db);

					//end eval
					vector<correction>::iterator it = find(cors.begin(), cors.end(), cur);
					if (it != cors.end())
					{
						it->cs.push_back(cur_cos);
					}
					else
					{
						cur.cs.push_back(cur_cos);
						cors.push_back(cur);
					}

				}
			}
		}
		if (a_amount == 4)
		{
			for (int i = 0; i < a_ops_l.size(); i++)
			{

				for (int j = 0; j < 6; j++)
				{
					index = 0;
					index2 = 0;
					cur.cs.clear();
					cur.in.clear();
					int nums[4];
					for (int k = 0; k < 4; k++)
					{
						if (mask4[j][k] == 0)
						{
							cur.in.push_back(a_ops_l[i].names[k]);
							nums[index2++] = k;
						}
						else
						{
							nums[2 + index] = k;
							cur.out[index++] = a_ops_l[i].names[k];
						}
					}
					//eval cos for cur term
					Cos cur_cos;
					cur_cos.factor = a_ops_l[i].coeff;
					int da, db;

					findCos(m, matrix_size, a_ops_l[i].node[nums[0]], da, db);
					cur_cos.ka.push_back(da);
					cur_cos.kb.push_back(db);
					findCos(m, matrix_size, a_ops_l[i].node[nums[1]], da, db);
					cur_cos.ka.push_back(da);
					cur_cos.kb.push_back(db);
					findCos(m, matrix_size, a_ops_l[i].node[nums[2]], da, db);
					cur_cos.ka.push_back(da);
					cur_cos.kb.push_back(db);
					findCos(m, matrix_size, a_ops_l[i].node[nums[3]], da, db);
					cur_cos.ka.push_back(da);
					cur_cos.kb.push_back(db);

					//end eval
					vector<correction>::iterator it = find(cors.begin(), cors.end(), cur);
					if (it != cors.end())
					{
						it->cs.push_back(cur_cos);
					}
					else
					{
						cur.cs.push_back(cur_cos);
						cors.push_back(cur);
					}
				}
			}

		}
	}

	void set_signs(char op1, char op2, int &s1, int &s2, int &type)
	{
		if (op1 == 'm'&&op2 == 'p') //G_k, ��� ����� ����
		{
			s1 *= 1;
			s2 *= 1;
			type = 0;
		}
		if (op1 == 'p'&&op2 == 'm') //Gp_k, ��� ����� �����
		{
			s1 *= -1;
			s2 *= -1;
			type = 1;
		}
		if (op1 == 'm'&&op2 == 'm') //F_k, + -
		{
			s1 *= 1;
			s2 *= -1;
			type = 2;
		}
		if (op1 == 'p'&&op2 == 'p') //F_k, - +
		{
			s1 *= -1;
			s2 *= 1;
			type = 3;
		}

	}

	void combine(ofstream &outF)
	{
		int signs1[3], signs2[3];
		int min; //����� � �������� ������� k1
		int max; //����� � �������� ������� k2
		int da, db;
		int type[3];
		bool if_empty, total_empty1, total_empty2;
		ostringstream cos2;
		if (a_amount == 3)
		{
			for (int i = 0; i < a_ops_l.size(); i++)//���������� ���� ���� ������ � ������
			{
				for (int j = 0; j<a_ops_l.size(); j++) //��������� ���� ��������� �������
				{
					for (int k = 0; k < 3; k++) //���������� ������� ������� �� ������ ���� ����������
					{
						for (int l = 0; l < 6; l++) //������ ����� �����������, �������� ������� ��������� � ����� ���������� �� ������ ������ ����� ���������
						{
							for (int mm = 0; mm < 3; mm++) //������ ����� ����� ������ ��������� �� ���� ����������
							{
								if (a_ops_l[i].names[mm] == 'p')
									signs1[mm] = 1;
								else
									signs1[mm] = -1;
								if (a_ops_l[j].names[mm] == 'p')
									signs2[mm] = 1;
								else
									signs2[mm] = -1;
							}

							for (int mm = 0; mm < 3; mm++)
							{
								set_signs(a_ops_l[i].names[mm], a_ops_l[j].names[out_pair[l][mm]], signs1[mm], signs2[out_pair[l][mm]], type[mm]);
							}
							cos2.str("");
							outF << "+" << a_ops_l[i].coeff*a_ops_l[j].coeff;
							//������� 2 ������ ������� �����
							for (int mm = 0; mm < 2; mm++)
							{
								outF << "*" << green_func[type[(k + mm) % 3]] << "_" << momenta_names[mm] ;
							}
							//��������� ������ ������� ����� ������ ���
							outF << "*" << green_func[type[(k + 2) % 3]] << "_(";
							if (signs1[(k + 2) % 3] == 1)
							{
								if (-signs1[k] == -1)
									outF << "-" << momenta_names[0];
								else
									outF  << momenta_names[0];
								if (-signs1[(k + 1) % 3]==-1)
									outF<< "-" << momenta_names[1] << ")";
								else
									outF << "+" << momenta_names[1] << ")";
							}
							else
							{ 
								if (signs1[k] == -1)
									outF << "-" << momenta_names[0];
								else
									outF << momenta_names[0];
								if (signs1[(k + 1) % 3] == -1)
									outF << "-" << momenta_names[1] << ")";
								else
									outF << "+" << momenta_names[1] << ")";
							}
								

							//

							outF << "*DiracDelta[";
							for (int mm = 0; mm < 3; mm++)
							{
								outF << "+(" << signs1[(k + mm) % 3] << ")*" << momenta_names[mm] << "a";
							}
							outF << "]*DiracDelta[";
							for (int mm = 0; mm < 3; mm++)
							{
								outF << "+(" << signs1[(k + mm) % 3] << ")*" << momenta_names[mm] << "b";
							}
							/*outF << "]*DiracDelta[";
							for (int mm = 0; mm < 3; mm++)
							{
								outF << "+(" << signs1[(k + mm) % 3] << ")*" << momenta_names[mm];
							}*/

							outF << "]*Cos[";
							cos2 << "*Cos[";
							total_empty1 = true;
							total_empty2 = true;
							for (int mm = 0; mm<3; mm++)
							{
								int temp_n = (k + mm) % 3;
								findCos(m, matrix_size, a_ops_l[i].node[(k + mm) % 3], da, db);
								if (da != 0 || db != 0)
								{
									total_empty1 = false;
									if (signs1[(k + mm) % 3] == 1)//���� ����
										outF << "+(";
									else
										outF << "-(";
									if_empty = true;
									if (da != 0)
									{
										outF << momenta_names[mm] << "a*" << da;
										if_empty = false;
									}
									if (db != 0)
									{
										if (!if_empty)//���� ���� � da
											outF << "+" << momenta_names[mm] << "b*" << db << ")";
										else//��� ����� � da, + �� �����
											outF << momenta_names[mm] << "b*" << db << ")";
									}
									else
									{
										if (!if_empty)
											outF << ")";
										else
											outF << "0)";
									}
								}


								findCos(m, matrix_size, a_ops_l[j].node[out_pair[l][(k + mm) % 3]], da, db);
								//cos2 << "+" << signs2[out_pair[l][(k + mm) % 3]] << "*(" << momenta_names[mm] << "a*" << da << "+" << momenta_names[mm] << "b*" << db << ")";
								if (da != 0 || db != 0)
								{
									total_empty2 = false;
									if (signs2[out_pair[l][(k + mm) % 3]] == 1)//���� ����
										cos2 << "+(";
									else
										cos2 << "-(";
									if_empty = true;
									if (da != 0)
									{
										cos2 << momenta_names[mm] << "a*" << da;
										if_empty = false;
									}
									if (db != 0)
									{
										if (!if_empty)//���� ���� � da
											cos2 << "+" << momenta_names[mm] << "b*" << db << ")";
										else//��� ����� � da, + �� �����
											cos2 << momenta_names[mm] << "b*" << db << ")";
									}
									else
									{
										if (!if_empty)
											cos2 << ")";
										else
											cos2 << "0)";
									}
								}
							}
							if (total_empty1)
								outF << "0";
							outF << "]";
							outF << cos2.str();
							if (total_empty2)
								outF << "0";
							outF << "]";

						}
					}
				}
			}
		}
	}

	void clearTerms()
	{
		shorterTerms.clear();
		a_ops_l.clear();
		cors.clear();
	}
	bool PrintAll(ofstream &F)
	{

		for (int i = 0; i < cors.size(); i++)
		{
			cors[i].print(F, a_amount);
			if (i != cors.size() - 1)
				F << "+";
		}
		if (cors.size() == 0)
			return false;
		else
			return true;
	}
};

//end new class





int** matrix;

void fillMatrix(int **matrix, int NNN)
{
	int dir = 0;
	int d = max(1 + ((NNN) / 2) * 2, 1 + (NNN - 2) * 2);
	int s = d / 2;
	int cx, cy, ct, cb, cl, cr;
	dir = 0;//0- �������, 1-�����, 2- ������, 3- ����
	ct = cb = cl = cr = cx = cy = s;
	for (int curn = 0; curn<d*d; curn++)
	{
		matrix[cy][cx] = curn;
		switch (dir)
		{
		case 0:
			if (cx == cr)
			{
				cr++;
				cl--;
				cx++;
				dir = 1;
			}
			else
			{
				cx++;
			}
			break;
		case 1:
			if (cy == ct)
			{
				ct--;
				cb++;
				cy--;
				dir = 2;
			}
			else
			{
				cy--;
			}
			break;
		case 2:
		{
			cx--;
			if (cx == cl)
			{
				dir = 3;
			}
		}
		break;
		case 3:
		{
			cy++;
			if (cy == cb)
			{
				dir = 0;
			}
		}
		break;
		}
	}
	//!------------Matrix output--------------------
	for (int i = 0; i<d; i++)
	{
		for (int j = 0; j<d; j++)
		{
			cout << setw(3) << matrix[i][j] << " ";
		}
		cout << "\n";
	}
	//!-----------Output end
	ofstream nodenums("nodenums.txt", ios::out);

	nodenums << "{";
	for (int i = 0; i<d; i++)
	{
		nodenums << "{";
		for (int j = 0; j<d; j++)
		{
			if (j == d - 1)
				nodenums << matrix[i][j] << "}";
			else
				nodenums << matrix[i][j] << ",";
		}
		if (i<d - 1)
			nodenums << ",";
	}
	nodenums << "}";
	nodenums.close();


}





int _tmain(int argc, _TCHAR* argv[])
{


	vector<string> points;
	ifstream config("config.txt", ios::in);
	int num_points, min_order, max_order, cur_term_len_min, cur_term_len_max;
	string tmp;
	int a_amount;
	getline(config, tmp);
	config >> num_points;
	getline(config, tmp);
	getline(config, tmp);
	config >> min_order >> max_order;
	getline(config, tmp);
	getline(config, tmp);
	config >> a_amount;

	///init matrix
	int size = max(1 + ((max_order) / 2) * 2, 1 + (max_order - 2) * 2);
	int** m;
	m = new int*[size];
	for (int i = 0; i < size; i++)
		m[i] = new int[size];
	fillMatrix(m, max_order);
	//end init


	ifstream fpoints("points.txt", ios::in);
	string s, tmp_s;
	for (int i = 0; i < num_points; i++)
	{
		getline(fpoints, s);
		points.push_back(s);
	}
	fpoints.close();


	ostringstream fname;
	double temp_val;

	for (int i = 0; i < num_points; i++)
	{
		cout << i << "\n";
		double factor;
		if (stof(points[i]) - 0.551 > 0)
			factor = 0.5;
		else
			factor = -0.5;
		converter conv1;
		term t1;

		ofstream out;

		fname.str("");
		fname << "Results\\res_" << points[i] << "_" << max_order << "_" << a_amount << ".txt";
		out.open(fname.str(), ios::out);
		out << "{";
		conv1.set(factor, a_amount, m, size);

		for (int j = min_order; j <= max_order; j++)
		{
			cout << "Order " << j << "\n";
			conv1.clearTerms();
			for (int k = min_op_amount; k <= j; k++)
			{
				cout << "SubOrder: " << k << "\n";
				fname.str("");
				fname << "d:\\Andrew\\Practice\\!!!_Last Set\\Temp1\\5 SumNum Term\\5 SumNum Term\\results\\" << points[i] << "\\" << j << "_results_" << points[i] << "_" << k << ".txt";

				ifstream cur(fname.str(), ios::in);

				t1.setOrder(j);

				while (!cur.eof())
				{
					getline(cur, s);
					if (s.length() > 0)
					{
						istringstream iss;
						iss.str(s);
						iss >> tmp_s >> temp_val;

						t1.decompose(tmp_s, temp_val);

						conv1.decomposeTerm(t1);

					}
				}
				cur.close();
			}
			conv1.convertToAop();
			if (a_amount != 3)
			{
				conv1.convertToCorrections();

				if (!conv1.PrintAll(out))
					out << "0";
				if (j != max_order)
					out << ",";
			}
			else
			{
				conv1.combine(out);
				if (j != max_order)
					out << ",";
			}
		}
		out << "}";

		out.close();
	}

	return 0;
}

