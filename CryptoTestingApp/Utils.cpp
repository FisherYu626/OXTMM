#include "Utils.h"
#include <sstream>
#include <vector>
#include <fstream>
 
using std::string;
using std::vector;

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}


int enc_aes_gcm(const unsigned char *plaintext, int plaintext_len,
                const unsigned char *key,
                unsigned char *ciphertext)
{
  
    unsigned char output[AESGCM_MAC_SIZE+ AESGCM_IV_SIZE + plaintext_len*2] = {0};
    memcpy(output+AESGCM_MAC_SIZE,gcm_iv,AESGCM_IV_SIZE);
    

    int ciphertext_len=0, final_len=0;
  
    EVP_CIPHER_CTX *ctx= EVP_CIPHER_CTX_new();
    EVP_EncryptInit(ctx, EVP_aes_128_gcm(),key, gcm_iv);




    // 加密来自缓冲区的inl字节in并将加密版本写入out,实际写入字节数存于outl
    EVP_EncryptUpdate(ctx, output+ AESGCM_MAC_SIZE+ AESGCM_IV_SIZE, &ciphertext_len, plaintext, plaintext_len);


    //对密文进行填充，填充大小写入final_len
    EVP_EncryptFinal(ctx, output+ AESGCM_MAC_SIZE+ AESGCM_IV_SIZE + ciphertext_len, &final_len);

    //std::cout<<std::endl<<"此次加密填充长度为"<<final_len<<std::endl;
    
    // printf("AESGCM_MAC2 is \n");
    // for(int i = 0;i<AESGCM_MAC_SIZE;i++){
    //     printf("%x",*(output+i));
    // }
    // printf("\n");

    //对上下文ctx执行特定于密码的控制操作,对称算法控制函数，它调用了用户实现的ctrl回调函数。 对秘钥进行消息验证码操作
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, AESGCM_MAC_SIZE, output);
    //从密码上下文中清除所有信息并释放与其关联的所有已分配内存，包括ctx本身。此函数应在使用密码的所有操作完成后调用，以免敏感信息保留在内存中。
    EVP_CIPHER_CTX_free(ctx);

    ciphertext_len = AESGCM_MAC_SIZE+ AESGCM_IV_SIZE + ciphertext_len + final_len;
    memcpy(ciphertext,output,ciphertext_len);

    // printf("AESGCM_MAC3 is \n");
    // for(int i = 0;i<AESGCM_MAC_SIZE;i++){
    //     printf("%x ",*(output+i));
    // }
    // printf("\n");


    return ciphertext_len;
    
}

int dec_aes_gcm(unsigned char *ciphertext, int ciphertext_len,
                const unsigned char *key,
                unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int plaintext_len=0, final_len=0;
    
    EVP_DecryptInit(ctx, EVP_aes_128_gcm(), key, gcm_iv);
    EVP_DecryptUpdate(ctx, plaintext, &plaintext_len, 
                      ciphertext+AESGCM_MAC_SIZE+AESGCM_IV_SIZE, 
                      ciphertext_len - AESGCM_MAC_SIZE - AESGCM_IV_SIZE);

    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AESGCM_MAC_SIZE, ciphertext);
    EVP_DecryptFinal(ctx, plaintext + plaintext_len, &final_len);
    EVP_CIPHER_CTX_free(ctx);
    plaintext_len = plaintext_len + final_len;

    return plaintext_len;
}

void print_bytes(uint8_t *ptr, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    printf("%x", *(ptr + i));
    printf(" - ");
  }

  printf("\n");
}


std::vector<std::string> split(std::string s, char c)
{
    s += c;
    std::vector<std::string> ret;
    std::string t;
    for(int i = 0; i < s.length(); i ++ )
    {
        if(s[i] == c)
            ret.push_back(t), t = "";
        else t += s[i];
    }
    return ret;
}

std::string readNextDoc(int file_reading_counter){
    std::string rawDocDir = "/home/node10/compress/dataset/100000/";
    std::ifstream inFile;
    std::stringstream strStream;

    string filename = std::to_string(file_reading_counter);

    filename = rawDocDir +filename+".txt";

    //cout<<"filename is "<<filename<<endl;

    inFile.open(filename);
    strStream << inFile.rdbuf();
    inFile.close();

    string res = strStream.str();
    //cout<<"now the doc"<<filename<<endl;
    //cout<<res<<endl;
    
    strStream.clear();

    return res;
}

std::string readNextBinaries(int file_reading_counter){
    std::string rawDocDir = "/home/node10/compress/snaTmp/dataset/100000/a";

    std::ifstream inFile;
    std::stringstream strStream;

    string filename = std::to_string(file_reading_counter);

    filename = rawDocDir +filename+".txt";

    //cout<<"filename is "<<filename<<endl;

    inFile.open(filename);
    strStream << inFile.rdbuf();
    inFile.close();

    string res = strStream.str();
    //cout<<"now the doc"<<filename<<endl;
    //cout<<res<<endl;
    
    strStream.clear();

    return res;
}

bool WriteNextDoc(int file_reading_counter,string output){
    bool isOk;
    std::ofstream outFile;
    std::stringstream strStream(output);

    string outDocDir = "/home/node10/compress/snaTmp/dataset/100000/";

    string outDocfile = outDocDir+std::to_string(file_reading_counter)+".txt";

    outFile.open(outDocfile,std::ofstream::out);
    //C++流对象函数rdbuf()简介:
    //rdbuf()可以实现一个流对象指向的内容用另一个流对象来输出
    outFile<<strStream.rdbuf();
    outFile.close();

    strStream.clear();
    isOk = true;

    return isOk;
}

bool WriteNextBinaries(int file_reading_counter,string output){
    bool isOk;
    std::ofstream outFile;
    std::stringstream strStream(output);

    string outDocDir = "/home/node10/compress/snaTmp/dataset/100000/";

    string outDocfile = outDocDir+"a"+std::to_string(file_reading_counter)+".txt";

    outFile.open(outDocfile,std::ofstream::out);
    //C++流对象函数rdbuf()简介:
    //rdbuf()可以实现一个流对象指向的内容用另一个流对象来输出
    outFile<<strStream.rdbuf();
    outFile.close();

    strStream.clear();
    isOk = true;

    return isOk;
}

void itoa(int value, char *s, int radix)
{
    assert(s != NULL);              //s不能为空

    // if(radix<2 || radix>36)
    // {
    //     std::cout<<"Radix wrong!Radix should be in [2,36]."<<std::endl;
    //     return;
    // }

    bool flg = false;               //false表示字符串不添加负号
    unsigned uValue;                //用来保存value转化的无符号数
    char *tmp = s;                  //声明一个遍历指针
    char table[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//table用来表示进制

    if(value<0 && radix==10)        //只有十进制数才区分正负，其它进制直接转换
    {
        flg = true;
        uValue = (unsigned)-value;
    }
    else
    {
        uValue = (unsigned)value;
    }

    while(uValue > 0)               //逆序保存每位数
    {
        *tmp = table[uValue % radix];
        uValue = uValue/radix;
        tmp++;
    }

    if(flg)                         //添加负号(如果需要)和结尾符
    {
        *tmp = '-';
    }
    *tmp = '\0';
    tmp--;

    char ch;
    while(tmp > s)                  //翻转字符串
    {
        ch = *s;
        *s = *tmp;
        *tmp = ch;
        tmp--;
        s++;
    }
}\
