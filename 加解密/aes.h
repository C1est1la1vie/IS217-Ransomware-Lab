#pragma once
#ifndef MY_AES_H
#define MY_AES_H

extern "C"  //��Ҫ���õ�C++����Ͱ�����д�����extern "C"��Χ�У�
{
void ScheduleKey(unsigned char* inkey, unsigned char* outkey, int Nk, int Nr);//��Կ��չ
void AesEncrypt(unsigned char* blk, unsigned char* expansionkey, int Nr); //AES�����ܺ���
void Contrary_AesEncrypt(unsigned char* blk, unsigned char* expansionkey, int Nr);//AES �����ܺ���
}
#endif
