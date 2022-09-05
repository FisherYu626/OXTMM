#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include "config.h"
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <array>
#include <list>
#include <string>
#include <tuple>
#include <set>
#include <utility>
#include <unordered_map>
#include<map>
#include <assert.h>

/* for all sources except OCALL/ECALL */

//const std::string raw_doc_dir= "streaming/"; 
const std::string raw_doc_dir= "streaming02/"; 

#define AESGCM_IV_SIZE 12
static unsigned char gcm_iv[] = {
    0x99, 0xaa, 0x3e, 0x68, 0xed, 0x81, 0x73, 0xa0, 0xee, 0xd0, 0x66, 0x84
};

#define AESGCM_MAC_SIZE 16

#define MAX_FILE_LENGTH 10 

#define ENC_KEY_SIZE 16 // for AES128
#define ENTRY_VALUE_LEN 128 // 1024-bit

#define ENTRY_HASH_KEY_LEN_128 16 // for HMAC-SHA128- bit key
#define BUFLEN 10240 //buffer for enc + dec
#define RAND_LEN 64// 256 // 2048-bit

/************fisher added*********************************/
#define KEYWORD_BIT_LENGTH 14
/*********************************************************/

/************fisher added 2.0*********************************/
#define P 3
#define COMSLICE_LEN 200
/*********************************************************/
typedef struct
{
    size_t content_length;
    unsigned char content[RAND_LEN];
} rand_t; //used to export between ecall and ocall


/* packet related */
typedef struct docIds {
    char *doc_id; 
    size_t id_length;  // length of the doc_id
} docId; 


typedef struct entryKeys {
    char *content; 
    size_t content_length;  // length of the entry_value
} entryKey;

typedef struct entryValues {
    char *message; 
    size_t message_length;  // length of the entry_value
} entryValue;

typedef struct docContents{
    docId id;
    char* content;
    int content_length;
    //std::vector<std::string> wordList;
} docContent;


//fisher added
typedef struct Lvalue {
    unsigned char *ciphertext;
    int ciphertext_length;  // length of the entry_value
} Lvalue;

typedef struct Vvalue {
    unsigned char *message;
    int message_length;  // length of the entry_value
} Vvalue;

typedef struct Gama {
    unsigned char *message;
    int message_length;  // length of the entry_value
} Gama;

typedef struct T {
    unsigned char *message;
    int message_length;  // length of the entry_value
} T;

typedef struct K0 {
    unsigned char *message;
    int message_length;  // length of the entry_value
} K0;

typedef struct Qsgx{
    int vi;
    std::pair<std::string,std::vector<std::string>> LVG;
}Qsgx;



typedef std::pair<entryKey, entryValue> entry;
//fisher added
typedef std::array<int, P> Block;
typedef std::array<int, 2> CT_pair;
typedef std::array<int, 3> VCT_pair;
typedef std::pair<int,CT_pair> VCT;

typedef std::array<int,2> ID_pair;

// yangxu add
// std::set<int> doc_ids;

#endif
