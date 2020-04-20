#pragma once
#ifndef MY_RSA_H
#define MY_RSA_H
extern "C" {
bool judgeprime(int i);
int getprime();
int gcd(int a, int b);
long long multiplicative_inverse_element(long long a, long long r);
int get_pk(int w);
long long square_multiplication(long long x, long long c, long long r);
}
#endif
