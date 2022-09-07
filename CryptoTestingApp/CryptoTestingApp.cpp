#include <string>
#include "stdio.h"
#include "stdlib.h"

#include <snappy.h>

#include "sgx_urts.h"
#include "CryptoEnclave_u.h"

#include "../common/data_type.h"
#include "Server.h"
#include "Client.h"
#include "Utils.h"

//fsiher added!
#include<iostream>
#include<map>
#include<unordered_map>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/cmac.h>
#include<math.h>


//for measurement
#include <cstdint>
#include <chrono>
#include <iostream>
#include <fstream>
#include "dprfMM.h"

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
//end for measurement


#define ENCLAVE_FILE "CryptoEnclave.signed.so"

//fisher added construct DB(V)

std::unordered_map<int,std::vector<int>> DB;

int total_file_no = (int)10;//50000;//100000
int del_no = (int)1;//10000;//10000;

/* 	Note 1: Enclave only recognises direct pointer with count*size, where count is the number of elements in the array, and size is the size of each element
		other further pointers of pointers should have fixed max length of array to eliminate ambiguity to Enclave (by using pointer [max_buf]).
	Note 2: In outcall, passing pointer [out] can only be modified/changed in the direct .cpp class declaring the ocall function.
	Note 3: If it is an int pointer pointing to a number-> using size=sizeof(int) to declare the size of the int pointer. That will be a larger range than using size_t in ocall
	Note 4: ensure when using openssl and sgxcrypto, plaintext data should be more lengthy than 4-5 characters; (each content in raw_doc should have lengthy characters)
			otherwise, random noise/padding will be auto added.
	Note 5: convert to int or length needs to total_filecome with pre-define length;otherwise, following random bytes can occur.

	memory leak note: 
	1-declare all temp variable outside forloop
	2-all func should return void, pass pointer to callee; caller should init mem and free pointer
	3-use const as input parameter in funcs if any variable is not changed 
	4-re-view both client/server in outside regarding above leak,
		 (docContent fetch_data = myClient->ReadNextDoc();, 

			//free memory 
			free(fetch_data.content);
			free(fetch_data.id.doc_id);)
	5-struct should use constructor and destructor (later)
	6-should use tool to check mem valgrind --leak-check=yes to test add function to see whether memory usage/leak before and after
	7-run with prerelease mode
	8-re generate new list test, but without using the list inside
 */

Client *myClient; //extern to separate ocall
Server *myServer; //extern to separate ocall

// void ocall_print_string(const char *str) {
//     printf("%s", str);
// }

// void ocall_transfer_encrypted_entries(const void *_t1_u_arr,
// 									  const void *_t1_v_arr, 
// 									  const void *_t2_u_arr,
// 									  const void *_t2_v_arr,
// 									  int pair_count, int rand_size){

// 	myServer->ReceiveTransactions(
// 								(rand_t *)_t1_u_arr,(rand_t *)_t1_v_arr,
// 								(rand_t *)_t2_u_arr,(rand_t *)_t2_v_arr,
// 								pair_count);

// }

// void ocall_retrieve_encrypted_doc(const char *del_id, size_t del_id_len, 
//                                   unsigned char *encrypted_content, size_t maxLen,
//                                   int *length_content, size_t int_size){
// 	// std::cout<<"yes"<<std::endl;						  
// 	std::string del_id_str(del_id,del_id_len);	
// 	std::string encrypted_entry = myServer->Retrieve_Encrypted_Doc(del_id_str);
	
//     *length_content = (int)encrypted_entry.size();

// 	//later double check *length_content exceeds maxLen
//     memcpy(encrypted_content, (unsigned char*)encrypted_entry.c_str(),encrypted_entry.size());
// }

// void ocall_del_encrypted_doc(const char *del_id, size_t del_id_len){
// 	std::string del_id_str(del_id,del_id_len);
// 	myServer->Del_Encrypted_Doc(del_id_str);
// }

// void ocall_retrieve_M_c(unsigned char * _u_prime, size_t _u_prime_size,
//                               unsigned char *_v_prime, size_t maxLen,
//                               int *_v_prime_size, size_t int_len){

// 	std::string u_prime_str((char*)_u_prime,_u_prime_size);
// 	std::string v_prime_str = myServer->Retrieve_M_c(u_prime_str);

// 	*_v_prime_size = (int)v_prime_str.size(); 
// 	memcpy(_v_prime,(unsigned char*)v_prime_str.c_str(),v_prime_str.size());

// }

// void ocall_del_M_c_value(const unsigned char *_u_prime, size_t _u_prime_size){

// 	std::string del_u_prime((char*)_u_prime,_u_prime_size);
// 	myServer->Del_M_c_value(del_u_prime);
// }

// void ocall_query_tokens_entries(const void *Q_w_u_arr,
//                                const void *Q_w_id_arr,
//                                int pair_count, int rand_size){
	
// 	std::vector<std::string> Res;
// 	Res = myServer->retrieve_query_results(
// 								(rand_t *)Q_w_u_arr,(rand_t *)Q_w_id_arr,
// 								pair_count);
	
// 	//give to Client for decryption
// 	myClient->DecryptDocCollection(Res);
// }

// void ocall_retrieve_VGama(unsigned char * L_text,int L_length,
//         unsigned char * V_text,int V_length,
//         unsigned char * Gama_Plain,int Gama_Plain_length){

// 	myServer->RetrieveVGama(L_text,L_length,
//         V_text,V_length,
//         Gama_Plain,Gama_Plain_length);

// 	return;
// }

// void ocall_receive_VxGamaX(unsigned char * vx_text,int vx_length,
//                     unsigned char * gamax_plain,int gamax_plain_len,
// 					int vi){

// 	myClient->receive_vxGamaX(vx_text,vx_length,
//                     gamax_plain,gamax_plain_len,
// 					vi);
// 	return;
// }

// void ocall_receive_R(unsigned char *R,int R_len){
// 	printf("here is R content:\n");
// 	print_bytes(R,36);

// 	myClient->DecryptR2Ids(R,R_len);
// 	return;
// }

// void ocall_sendLVGAMA(unsigned char * L2,int L2_len,
// unsigned char *V2,int V2_len,
// unsigned char *gama_X2_plain,int gama_X2_len){

// 	Lvalue * L = (Lvalue *)malloc(sizeof(Lvalue));
// 	Vvalue * V = (Vvalue *)malloc(sizeof(Vvalue));
// 	Gama * gama_plain = (Gama *)malloc(sizeof(Gama));

// 	gama_plain->message = (unsigned char *)malloc(P*sizeof(int));
// 	gama_plain->message_length = P*sizeof(int);

// 	L->ciphertext = (unsigned char *)malloc((AESGCM_MAC_SIZE+ AESGCM_IV_SIZE+P*4)*sizeof(unsigned char));
// 	L->ciphertext_length = AESGCM_MAC_SIZE+ AESGCM_IV_SIZE+P*4;

// 	V->message = (unsigned char *)malloc((AESGCM_MAC_SIZE+ AESGCM_IV_SIZE+P*4)*sizeof(unsigned char));
// 	V->message_length = (AESGCM_MAC_SIZE+ AESGCM_IV_SIZE+P*4)*sizeof(unsigned char);

// 	memcpy(L->ciphertext,L2,L2_len);
// 	L->ciphertext_length = L2_len;

// 	memcpy(V->message,V2,V2_len);
// 	V->message_length = V2_len;

// 	memcpy(gama_plain->message,gama_X2_plain,gama_X2_len);
// 	gama_plain->message_length = gama_X2_len;


// 	myServer->ReceiveLVR(L,V,gama_plain);
	
// 	free(gama_plain->message);
// 	free(gama_plain);
// 	free(L->ciphertext);		
// 	free(L);
// 	free(V->message);
// 	free(V);

// }


// void ocall_retrieve_PKi(unsigned char *Addr,int addr_len,unsigned char * PKi,int PKi_len){

// 	myServer->RetreivePKi(Addr,addr_len,PKi,PKi_len);
// 	// printf("%s",PKi);

// 	return;
// }

// void ocall_transfer_uv_pairs(const void *u_arr,
// 							const void *v_arr, 
// 							int pair_count, int rand_size){

// 	myServer->Receive_uv_pairs((rand_t *)u_arr,
// 								(rand_t *)v_arr,
// 								pair_count);

// }


// void ocall_Retrieve_V_FromT1(unsigned char * u, size_t u_len,unsigned char *v, size_t v_len,int *content_length, size_t int_len){

// 	printf("now starting retreiveing *********************\n");
// 	print_bytes(v,v_len);
// 	myServer->Receive_V_FromT1(u, u_len,v,content_length);
// 	print_bytes(v,*content_length);
// 	printf("now ending retreiveing *********************\n");
// 	return;
// }

// void ocall_transfer_V(const void *v_arr, int pair_count, int rand_size){

// 	std::map<int,std::vector<std::string>> res;

// 	res = myServer->Receive_V((rand_t *)v_arr,pair_count);


// 	printf("res size is %d\n",res.size());
	
// 	myClient->DecryptPKs(res);



// 	return;
// }

int main()
{
	/* Setup enclave */
	sgx_enclave_id_t eid;
	sgx_status_t ret;
	sgx_launch_token_t token = { 0 };
	int token_updated = 0;
	
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &token_updated, &eid, NULL);
	if (ret != SGX_SUCCESS)
	{
		printf("sgx_create_enclave failed: %#x\n", ret);
		return 1;
	}

	/* Setup Protocol*/
	//Client
	myClient= new Client();

	//Enclave
	unsigned char KIvalue[ENC_KEY_SIZE];
	unsigned char KZvalue[ENC_KEY_SIZE];
	unsigned char KXvalue[ENC_KEY_SIZE];
	unsigned char KHvalue[ENC_KEY_SIZE];
	myClient->getKFValues(KIvalue,KZvalue,KXvalue,KHvalue);


	//fisher altered!
	// ecall_init(eid,KIvalue,KZvalue,KXvalue,KHvalue,(size_t)ENC_KEY_SIZE);


	/**************************Build Process **************************************/
	myServer= new Server();

	printf("Adding doc\n");
	std::vector<std::string> KT;
	std::vector<std::pair<std::string, std::string>> Stash;

	//cuckoo filter table size
	size_t tableSize = 40000;

	myClient->ReadInvertedIndex();

	myClient->Setup(KT,Stash);

	int n = myClient->YMM.size();
	
	dprfMM * myDprfMM = new dprfMM(n,5000,5000);
	myDprfMM->Setup(myClient->YMM);
	
	dprfMM * OXTMM = new dprfMM(n,5000,5000);
	OXTMM->Setup(myClient->MM,myClient->xlist);
	//myClient->dprffSetup();
	
	/*** Saving the V to DB(v)*************************************/
/* 		for(int i=1;i <= 3; i++){  
		
		docContent *fetch_data;
		std::string CompressData;
		std::string Enc_data;
		
		std::vector<std::string> ComSlices;

		std::vector<std::pair<std::string,std::string>> M;

		fetch_data = (docContent *)malloc(sizeof( docContent));
		myClient->ReadNextDoc(fetch_data);

		printf("*******************Compressing *****************\n");
		//std::cout<<fetch_data->content<<std::endl;
		std::cout<<"源文件流大小"<<fetch_data->content_length<<std::endl;

		

		snappy::Compress(fetch_data->content,(unsigned long)fetch_data->content_length,&CompressData);
		std::cout<<"压缩后流大小"<<CompressData.size()<<std::endl;

		//printf("*******************压缩后 加密前******************\n");
		//print_bytes((unsigned char *)CompressData.c_str(),CompressData.size());

		
		//填充压缩后文件至200字节整数倍
		myClient->PaddingCompressdata(CompressData);
		std::cout<<"填充后加密前流大小 "<<CompressData.size()<<std::endl;

		printf("*******************Encrypting ******************\n");
		Enc_data =  myClient->EncryptDoc(CompressData);
		std::cout<<"加密后流大小 "<<Enc_data.size()<<std::endl;

		printf("*******************压缩后 加密后 ******************\n");
		print_bytes((unsigned char *)Enc_data.c_str(),Enc_data.size());
		//std::cout<<CompressData<<std::endl;
	

		ID_pair id_pair;
		
		//8 == 2 int
		unsigned char * addr = (unsigned char *)malloc(8+AESGCM_MAC_SIZE+ AESGCM_IV_SIZE);
		unsigned char * ID = (unsigned char *)malloc(8);
		
		std::cout<<"************Slicing part**********"<<std::endl;
		for(int j = 0;j<Enc_data.size()/COMSLICE_LEN; j++){
			id_pair[0] = i;
			id_pair[1] = j;
			
			memcpy(ID,&id_pair[0],4);
			memcpy(ID+4,&id_pair[1],4);
			
			print_bytes(ID,8);
			//printf("%s\n",t);
			
			//std::cout<<"i and j is "<<id_pair[0]<<" "<<id_pair[1]<<std::endl;
			//print_bytes(ID,8);

			enc_aes_gcm(ID,2*sizeof(int),KF1value,addr);
			
			
			// print_bytes(addr,8+AESGCM_MAC_SIZE+ AESGCM_IV_SIZE);
			// printf("\n");
			
			//验证消息验证码mac
			//ecall_verifyIDEnc(eid,ID,(size_t)8); 
			
			
			//获取addr的前16个字节的消息验证码

			std::string Addr;
			for(int k = 0;k<AESGCM_MAC_SIZE;k++){
				Addr.push_back(addr[k]);
			}
			
			//获取当前加密字串的分片
			std::string PK;
			PK = Enc_data.substr(j*COMSLICE_LEN,COMSLICE_LEN);

			M.push_back(std::pair<std::string,std::string> {Addr,PK});
			
		}
			
		//sending {op,{id||N}} to enclave;
		int N = Enc_data.size()/COMSLICE_LEN;
		id_pair[0] = i;
		id_pair[1] = N;
		
		memcpy(ID,&id_pair[0],4);
		memcpy(ID+4,&id_pair[1],4);
		
		print_bytes(ID,8);

		// //send M to server;
		myServer->ReceiveM(M);

		ecall_SendOpIdN(eid,0,ID,2*sizeof(int));



		free(addr);
		free(ID);

		free(fetch_data->content);
		free(fetch_data->id.doc_id);
		free(fetch_data);
	}




	for(int del_index=1; del_index <=del_no; del_index++){
		docId delV_i;
		//printf("->%s",delV_i[del_index].doc_id);

		myClient->Del_GivenDocIndex(del_index, &delV_i);

		ecall_delDoc(eid,delV_i.doc_id,delV_i.id_length);
	}


	myClient->SetS(0);

	std::string s_keyword[2]= {"100,105","0,1"};
	// std::string s_keyword[2]= {"0,2000","4800,5000"};  

	for (int s_i = 0; s_i < 2; s_i++){
		printf("\nSearching ==> %s\n", s_keyword[s_i].c_str());

		std::string TKq = myClient->Generate_Token(s_keyword[s_i]);

		//myServer->doc_ids.clear();
		//std::cout << timeSinceEpochMillisec() << std::endl;

		ecall_search_tkq(eid,(unsigned char *)TKq.c_str(), TKq.size());	

		// std::cout << timeSinceEpochMillisec() << std::endl;
		

	} 
	
*/
	
	

	
	
	delete myClient;
	delete myServer;
	delete myDprfMM;
	delete OXTMM;
	
	return 0;
}

