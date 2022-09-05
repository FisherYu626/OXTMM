#include "CryptoEnclave_t.h"

//#include "EnclaveUtils.h"

//fisher added
#include<set>


#include "sgx_trts.h"
#include "sgx_tcrypto.h"
#include "stdlib.h"
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <vector>
#include <list>
#include "../common/data_type.h"

#include "BloomFilter.h"
// change to malloc for tokens, run ulimit -s 65536 to set stack size to 
// 65536 KB in linux 


// local variables inside Enclave
unsigned char KW[ENC_KEY_SIZE] = {0};
unsigned char KC[ENC_KEY_SIZE] = {0};
unsigned char KI[ENC_KEY_SIZE] = {0};
unsigned char KZ[ENC_KEY_SIZE] = {0};
unsigned char KX[ENC_KEY_SIZE] = {0};
unsigned char KH[ENC_KEY_SIZE] = {0};

std::unordered_map<std::string, int> ST;
//std::unordered_map<std::string, std::vector<std::string>> D;

std::vector<std::string> D;

int w_min = __INT_MAX__;
int w_max = 0;


//generate key for BF
unsigned char K_BF[ENC_KEY_SIZE] = {0};
BloomFilter *myBloomFilter1;

BloomFilter *myBloomFilter2;





/*** setup */
void ecall_init(unsigned char *keyF1,unsigned char *keyF2,unsigned char *keyF3,unsigned char *keyF4, size_t len){ 
	D.reserve(750000);
    //fisher altered 2.0 将client中的kf1 kf2传入enclave
    memcpy(KI,keyF1,len);
    memcpy(KZ,keyF2,len);
    memcpy(KX,keyF3,len);
    memcpy(KH,keyF4,len);
    //此处生产2个长度为16字节的随机数
    sgx_read_rand(KW, ENC_KEY_SIZE);
    sgx_read_rand(KC, ENC_KEY_SIZE);

    //init Bloom
    sgx_read_rand(K_BF, ENC_KEY_SIZE); //初始化k_{BF}
    uint64_t vector_size = 35000000;//4mb hold up to 1.5 million key,value pairs 初始化b
    uint8_t numHashs = 23; // 初始化h
    myBloomFilter1 = new BloomFilter(vector_size,numHashs); //初始化Bloom filter
    myBloomFilter2 = new BloomFilter(vector_size,numHashs); //初始化Bloom filter


}

// void ecall_printHelloWorld(){
//     printf("helloworld");
//     return ;
// }

// /*** update with op=add */
// void ecall_addDoc(char *doc_id, size_t id_length,char *content,int content_length){

//     //fisher added
//     std::set<std::string> stored_w;

//     //parse content to keywords splited by comma
//     std::vector<std::string> wordList;
//     wordList = wordTokenize(content,content_length);

//     //fisher added show the words comes from the file
//     for(auto i : wordList){
//         printf("<%s>",i.c_str());
//     }
//     //yangxu altered!
//     //////////////////////////////////////////////////////
//     int pair_no = 0;
//      for(std::vector<std::string>::iterator it = wordList.begin(); it != wordList.end(); ++it) {
      
//         std::string word = (*it);

//         std::string wordBits = String2bit(word,KEYWORD_BIT_LENGTH);
        
//         printf("<%s>",wordBits.c_str());
//         printf("<%d>",wordBits.size());

//         for(int i = 0;i<wordBits.length();i++){
//             std::string _w = wordBits.substr(0,i+1);
//             if(stored_w.count(_w)){
//                 continue;
//             }else{
//                 pair_no += 1;
//                 stored_w.insert(_w);
//             }
//         }
//     }

//     ////////////////////////////////////////////////////////

//     //fisher altered!! or std::bad alloc!
//     //size_t pair_no = wordList.size()*(KEYWORD_BIT_LENGTH+1);
//     // size_t pair_no = 12;
//     rand_t t1_u_arr[pair_no];
//     rand_t t1_v_arr[pair_no];
//     rand_t t2_u_arr[pair_no];
//     rand_t t2_v_arr[pair_no];

//     int index=0;
//     for(auto _w : stored_w){
//         entryKey k_w, k_c;
        
//         k_w.content_length = AESGCM_MAC_SIZE + AESGCM_IV_SIZE + _w.length(); 
//         k_w.content = (char *) malloc(k_w.content_length);
//         enc_aes_gcm(KW,_w.c_str(),_w.length(),k_w.content,k_w.content_length);
        

//         k_c.content_length = AESGCM_MAC_SIZE + AESGCM_IV_SIZE + _w.length();
//         k_c.content = (char *) malloc(k_c.content_length);
//         enc_aes_gcm(KC,_w.c_str(),_w.length(),k_c.content,k_c.content_length);

//         int c=0;

//         std::unordered_map<std::string,int>::const_iterator got = ST.find(_w);
//         if ( got == ST.end()) {
//             c = 0;  
//         }else{
//             c = got->second;
//         }
//         c++;

//         //find k_id
//         unsigned char *k_id =  (unsigned char *) malloc(ENTRY_HASH_KEY_LEN_128); 
//         std::string c_str = std::to_string(c);
//         char const *c_char = c_str.c_str();
//         hash_SHA128(k_w.content,c_char,c_str.length(),k_id);

//         //len is used for hash_SHA128_key multiple times
//         size_t len = ENTRY_HASH_KEY_LEN_128 + k_w.content_length;
        
//         //generate a pair (u,v)
//         unsigned char *_u = (unsigned char *) malloc(len * sizeof(unsigned char));
//         hash_SHA128_key(k_w.content,k_w.content_length, c_char,c_str.length(),_u);
//         memcpy(&t1_u_arr[index].content,_u,len);
//         t1_u_arr[index].content_length = len;


//         size_t message_length = AESGCM_MAC_SIZE + AESGCM_IV_SIZE + id_length;
//         char* message = (char *) malloc(message_length);
            
//         enc_aes_gcm(k_id,doc_id,id_length,message,message_length);
//         memcpy(&t1_v_arr[index].content,(unsigned char*)message,message_length);
//         t1_v_arr[index].content_length = message_length;

//         //generate a pair (u',v')
//         unsigned char *_u_prime = (unsigned char *) malloc(len * sizeof(unsigned char));
//         hash_SHA128_key(k_w.content,k_w.content_length, doc_id,id_length,_u_prime);
//         memcpy(&t2_u_arr[index].content,_u_prime,len);
//         t2_u_arr[index].content_length = len;

//         size_t message_length2 = AESGCM_MAC_SIZE + AESGCM_IV_SIZE + c_str.length();
//         char* message2 = (char *) malloc(message_length2);

//         enc_aes_gcm(k_c.content,c_char,c_str.length(),message2,message_length2);
//         memcpy(&t2_v_arr[index].content,(unsigned char*)message2,message_length2);
//         t2_v_arr[index].content_length = message_length2;

//         //update ST
//         got = ST.find(_w);
//         if( got == ST.end()){
//             ST.insert(std::pair<std::string,int>(_w,c));
//         } else{
//             ST.at(_w) = c;
//         }

//         index++;

//         //free memory
//         free(k_id);
//         free(_u);
//         free(_u_prime);

//         //free k_w, k_c
//         free(k_w.content);
//         free(k_c.content);

//         //free value
//         free(message);
//         free(message2);
//     }
//     // printf("\n");
//     // printf("this part fininshed safely");
//     //call Server to update
//     ocall_transfer_encrypted_entries(t1_u_arr,
//                                      t1_v_arr,
//                                      t2_u_arr,
//                                      t2_v_arr,
//                                      pair_no, sizeof(rand_t));

// }

// /*** update with op=del */
// void ecall_delDoc(char *doc_id, size_t id_length){
//     std::string delId(doc_id,id_length);
//     printf("id length is %d\n",id_length);
//     printf("docid is %s\n",doc_id,id_length);
//     D.push_back(delId);
//     printf("del id is %s\n",delId);
// }

