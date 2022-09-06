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

class dprfMM{
    typedef long long ll;
	typedef unsigned int uint;

	public:
    dprfMM();
    dprfMM(int _n_size,std::string _init_key, std::string _init_value);
    void Setup(std::vector<std::pair<std::string,std::pair<std::string,std::string>>> YMM);


	std::vector<std::pair<std::string, std::string> > T1;
	std::vector<std::pair<std::string, std::string> > T2;
	std::map<std::string, std::string> stash;
	int n_size;
    int n_count;
	int n_per_size;
	std::string init_value;
	std::string init_key;
    unsigned char KD[ENC_KEY_SIZE];
    unsigned char KE[ENC_KEY_SIZE];
	double _loadfactor;



};
    
#endif