#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "rsa.h"
#define MIN 8192
#define MAX 16383 //13bit����������ķ�Χ2^13��2^14

using namespace std;

bool judgeprime(int i)//�ж��Ƿ�������
{
	int j;
	for (j = 2; j <= sqrt(i); j++)
		if (i % j == 0)
			break;
	if (j > sqrt(i))
		return true;
	else
		return false;
}
int getprime()//�������min��max�ڵ�����
{
	int a;
	srand((unsigned)time(NULL));
	while (1) {
		a = MIN + (rand() % (MAX - MIN));
		if (judgeprime(a)) {
			return a;
			break;
		}
		else
			continue;
	}
	return   0;
}
int gcd(int a, int b)//��a,b�����Լ��
{
	if (b == 0)
		return a;
	return gcd(b, a % b);
}
long long multiplicative_inverse_element(long long a, long long r)//��aģr����Ԫ
{
	long long rs = 1;
	for (; rs <= r; rs++)
		if ((a * rs) % r == 1)
			break;
	return rs;
}
int get_pk(int w)//
{
	int pk = 2;
	for (pk = 2; pk < w; pk++)
	{
		if (gcd(w, pk) == 1)
			return pk;
	}
	return pk;
}
long long square_multiplication(long long x, long long c, long long r)//ģƽ���㷨 x^c(mod r)
{
	string str = "";
	while (c)
	{
		str = (char)(c % 2 + '0') + str;
		c /= 2;
	}
	long long rs = x;
	int l = str.length();
	for (int i = 1; i < l; i++)
	{
		if (str[i] == '0')
			rs = ((rs % r) * (rs % r)) % r;
		else
			rs = (((rs % r) * (rs % r)) % r * x) % r;
	}
	while (rs < 0)
		rs += r;
	return rs;

}


/*int debug()
{
	pub_key session_pub_key;
	pri_key session_pri_key;
	get_session_key(&session_pub_key, &session_pri_key);
	cout << session_pub_key.n << endl;

	unsigned char aes_key[] = "1qaz2wsx3edc4rfv";
	unsigned char *encrypted_aes_key=new unsigned char[64];
	rsa(aes_key, session_pub_key, encrypted_aes_key);
	unsigned char* key = new unsigned char[16];
	dersa(encrypted_aes_key, session_pri_key,key);
    for (int i = 0; i < 16; i++) {

		//cout << key[i] << " ";
		printf("%c ", key[i]);
		//printf("%x ", encrypted_aes_key[i]);
	}
	return 0;
}*/
