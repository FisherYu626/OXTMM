#ifndef SERVER_H
#define SERVER_H

#include "../common/data_type.h"
#include "Utils.h"
#include <set>
class Server{
    public:
        Server(); 
        ~Server();
        void ReceiveEncDoc(entry *encrypted_doc);

        //fisher added!!
        void ReceiveLVR(Lvalue * L,Vvalue * V,Gama * gamacipher);

        void ReceiveTransactions(rand_t *t1_u_arr,rand_t *t1_v_arr,
                                 rand_t *t2_u_arr,rand_t *t2_v_arr,
                                 int pair_count);
        std::string Retrieve_Encrypted_Doc(std::string del_id_str);
        std::string Retrieve_M_c(std::string u_prime_str);

        void RetrieveVGama(unsigned char * L_text,int L_length,
        unsigned char * V_text,int V_length,
        unsigned char * Gama_text,int Gama_length);
        
        void Del_Encrypted_Doc(std::string del_id_str);
        void Del_M_c_value(std::string del_u_prime);

        void Display_Repo();
        void Display_M_I();
        void Display_M_c();

        std::vector<std::string> retrieve_query_results(
								rand_t *Q_w_u_arr,rand_t *Q_w_id_arr,
								int pair_count);

        void ReceiveM(std::vector<std::pair<std::string,std::string>> & m);
        void RetreivePKi(unsigned char *Addr,int addr_len,unsigned char * PKi,int PKi_len);
        void Receive_uv_pairs(rand_t *u_arr,rand_t *v_arr,int pair_count);
        void Receive_V_FromT1(unsigned char * u, size_t u_len,unsigned char *v,int *content_length);
        std::map<int,std::vector<std::string>> Receive_V(rand_t *v_arr,int pair_count);

        //fisher added
        std::set<int> doc_ids;


    private:
        std::unordered_map<std::string,std::string> M_I;
        std::unordered_map<std::string,std::string> M_c;
        std::unordered_map<std::string,std::string> R_Doc;
        
        std::unordered_map<std::string,std::vector<std::string>> IMM;
        std::unordered_map<std::string,std::string> EDB;
        std::vector<std::pair<std::string,std::string>> M;
        std::unordered_map<std::string,std::string> T1;
        std::unordered_map<std::string,std::string> T2;

};
 
#endif
