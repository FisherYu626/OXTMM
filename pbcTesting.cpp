/*
 * @Author: Fisheryu 
 * @Date: 2022-08-31 19:59:19 
 * @Last Modified by: Fisheryu
 * @Last Modified time: 2022-09-01 11:18:22
 * 
 * g++ -o main pbcTesting.cpp -lgmp -lpbc
 */
#include <string>
#include "stdio.h"
#include "stdlib.h"

#include <snappy.h>

#include "sgx_urts.h"


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

#include <pbc/pbc.h>
#include <city.h>

using namespace std;


int main(){

    pairing_t pairing;
    char param[1024];
    
    FILE* file = fopen("/home/node10/fisher/F-OXTMM/param/a.param", "r");
    size_t count = fread(param, 1, 1024, file);
    fclose(file);

    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    
    printf("1\n");
    element_t g,xind,z;
    element_t y;
    element_t x1,x2,t;
    element_init_G1(g,pairing);
    element_init_G1(x1,pairing);
    element_init_G1(x2,pairing);
    element_init_G1(t,pairing);
    element_init_Zr(xind,pairing);
    element_init_Zr(z,pairing);
    element_init_Zr(y,pairing);

    element_from_hash(xind,(void *)"aabbcc",6);
    element_from_hash(z,(void *)"acd",3);
    
    printf("2\n");

    
    element_div(y,xind,z);
    printf("2.5\n");

    element_pow_zn(x1,g,xind);
    
    printf("3\n");
    
    element_pow_zn(x2,g,y);
    element_pow_zn(t,x2,z);
    
    printf("4\n");
    
    if(element_cmp(x1,t)==0){
        printf("g^xind == g^(xind*z-1*z)!!!\n");
    }


    element_clear(g);
    element_clear(xind);
    element_clear(z);
    element_clear(y);
    element_clear(x1);
    element_clear(x2);
    element_clear(t);


    
    return 0;
}