#include"dprfMM.h"

dprfMM::dprfMM(int _n_size,std::string _init_key, std::string _init_value):
    n_size(_n_size), init_key(_init_key), init_value(_init_value) {
    RAND_bytes(KD,ENC_KEY_SIZE);
    RAND_bytes(KE,ENC_KEY_SIZE);

    std::cout << "init_dprfMM" <<std::endl;
    std::cout << n_size <<std::endl;
    
    n_per_size = n_size;
    
}

void dprfMM::Setup(std::vector<std::pair<std::string,std::pair<std::string,std::string>>> YMM){

    

    return;
}