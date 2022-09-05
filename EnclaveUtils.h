#ifndef ENCLAVE_UTILS_H
#define ENCLAVE_UTILS_H

#include "stdlib.h"
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <iterator>
#include <vector>
#include <cstring>
#include "../common/data_type.h"
#include <sstream>


void printf( const char *fmt, ...);
void print_bytes(uint8_t *ptr, uint32_t len);
int  cmp(const uint8_t *value1, const uint8_t *value2, uint32_t len);
void clear(uint8_t *dest, uint32_t len);
std::vector<std::string>  wordTokenize(char *content,int content_length);

void enc_aes_gcm(const void *key, const void *plaintext, size_t plaintext_len, void *ciphertext, size_t ciphertext_len);
void dec_aes_gcm(const void *key, const void *ciphertext, size_t ciphertext_len, void *plaintext, size_t plaintext_len);
int hash_SHA128(const void *key, const void *msg, int msg_len, void *value);
int hash_SHA128_key(const void *key, int key_len, const void *msg, int msg_len, void *value);
/*********************fisher added************************/
void itoa(int value, char *s, int radix);
std::string String2bit(std::string word,int bitLength);
std::string Int2bit(int a,int bitLength);
bool StringIsSmaller(std::string am,std::string bm);
int BitString2Ten(std::string s);
std::string stringAdd(std::string am,int num,int len_res);
std::vector<std::string> GetBRCm(int a,int b);
std::vector<int> split(std:: string s,const char flag);

void Enclave_Generate_L(Lvalue * L ,unsigned char * KF1Value,const int & vi,const int & c,const int &ti);
void Enclave_Generate_Vx(unsigned char * vx,unsigned char * gamax_cipher,
    unsigned char *v,unsigned char * gama_cipher,int gama_cipher_len );

void Enclave_Generate_Vx2(unsigned char * vx,unsigned char * gamax_cipher,
    unsigned char *v,unsigned char * gama_cipher);
bool QincludesVi(std::vector<Qsgx *> & QsgxCache,int vi);

std::string DecryptDoc(const std::string enc_data,unsigned char * KFvalue);

/********************************************************/
//improved
//void prf_F_improve(const void *key,const void *plaintext,size_t plaintext_len, entryKey *k );
//void prf_Enc_improve(const void *key,const void *plaintext,size_t plaintext_len, entryValue *v);
//void prf_Dec_Improve(const void *key,const void *ciphertext,size_t ciphertext_len, entryValue *value );

#endif
