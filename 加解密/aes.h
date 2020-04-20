#pragma once
#ifndef MY_AES_H
#define MY_AES_H

extern "C"  //需要调用的C++程序就把声明写到这个extern "C"范围中；
{
void ScheduleKey(unsigned char* inkey, unsigned char* outkey, int Nk, int Nr);//密钥扩展
void AesEncrypt(unsigned char* blk, unsigned char* expansionkey, int Nr); //AES加密总函数
void Contrary_AesEncrypt(unsigned char* blk, unsigned char* expansionkey, int Nr);//AES 解密总函数
}
#endif
