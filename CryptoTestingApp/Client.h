/***
 * Demonstrate Client
 * maintain a current Kf
 * read documents in a given directory and give one by one to App.cpp with <fileId, array of words>
 * develop utility to enc and dec file with a given key kf
 * issue a random update operation (op,in) to App
 * issue a random keyword search
 */
#ifndef CLIENT_H
#define CLIENT_H

#include "../common/data_type.h"
#include "Utils.h"
#include <vector>
#include "cuckoo_filter.h"
#include <pbc/pbc.h>

class Client{
    public:
        Client();

        void ReadNextDoc(docContent *fetch_data);
        void ReadInvertedIndex(); 
        void Setup(std::vector<std::string>& KT,std::vector<std::pair<std::string, std::string>>& Stash,
        CuckooFilter<size_t, 4, 16, uint16_t>& T1,CuckooFilter<size_t, 4, 16, uint16_t>& T2);
        //fisher added
        //std::vector<std::string> Client::EncSlice(docContent *content,unsigned char * KFvalue);

        // void Del_GivenDocIndex(const int del_index, docId* delV_i);
        // void Del_GivenDocArray(const int * del_arr, docId* delV, int n);
        // void getKFValue(unsigned char * outKey);
        // //fisher altered!
        void getKFValues(unsigned char * outKey1,unsigned char * outKey2,unsigned char * outKey3,unsigned char * outKey4);
        // void G_AesEncrypt(Lvalue * L ,unsigned char * KF1Value,const int & v,CT_pair & CT);
        // void Generate_V(Vvalue * V,Block & block,const Gama * gama_cipher);
        // T * Generate_Token(unsigned char * KF1Value,int v,int cmp,int q);
        // std::string Generate_Token(std::string keyword);
        // int GetS();
        // bool AddS();
        // bool SetS(int num);

        // //void EncryptDoc(const docContent *data, entry *encrypted_doc );
        // //fisher overload
        // std::string EncryptDoc(const std::string data);
        // std::string DecryptDoc(const std::string enc_data);
        // void PaddingCompressdata(std::string & CompressData);

        // void DecryptDocCollection(std::vector<std::string> Res);
        // void receive_vxGamaX(const unsigned char * vx_text,int vx_length,
        //         const unsigned char * gamax_plain,int gamax_plain_len,
        //         int vi);
        // void DecryptR2Ids(unsigned char * R,int R_len);
        // void PrintIds();

        // void DecryptPKs(std::map<int,std::vector<std::string>> res);
        ~Client() {pairing_clear(pairing);}
        
        pairing_t pairing;
        std::map<std::string,std::vector<std::string>> MM;

        // std::string DDOC;

    private:
        //fisher altered!
        unsigned char KI[ENC_KEY_SIZE];
        unsigned char KZ[ENC_KEY_SIZE];
        unsigned char KX[ENC_KEY_SIZE];
        unsigned char KH[ENC_KEY_SIZE];
        int s;
        int file_reading_counter;


        // std::multimap<int,std::vector<std::string>> ViVxGamaX;
        // std::unordered_map<int,std::vector<int>> Qresult;
};
 
#endif