/***
 * Demonstrate dprfMM
 * maintain a current Kf
 * read documents in a given directory and give one by one to App.cpp with <fileId, array of words>
 * develop utility to enc and dec file with a given key kf
 * issue a random update operation (op,in) to App
 * issue a random keyword search
 */
#ifndef dprfMM_H
#define dprfMM_H

#include "../common/data_type.h"
#include "Utils.h"
#include <vector>
#include <pbc/pbc.h>
#include <openssl/rand.h>
#include <cmath>

class dprfMM{
    typedef long long ll;
	typedef unsigned int uint;

	public:
    dprfMM();
    dprfMM(int _n_size,uint _init_key, uint _init_value);
    void Setup(std::vector<std::pair<std::string,std::pair<std::string,std::string>>>& YMM);
    void Setup(std::map<std::string,std::vector<std::string>>& MM,
    std::vector<std::vector<std::string>>& xlist);
    unsigned int hash_fun1(char *str);
    unsigned int hash_fun2(char *str);


	std::vector<std::pair<int, int> > T1;
	std::vector<std::pair<int, int> > T2;
	std::map<std::string, std::string> stash;
	uint n_size;
    uint n_count;
	uint init_value;
	uint init_key;
    uint fn=100;
    unsigned char KD[ENC_KEY_SIZE];
    unsigned char KE[ENC_KEY_SIZE];
	double _loadfactor;
};
    
#endif