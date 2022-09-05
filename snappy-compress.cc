#include <snappy.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <chrono>

using namespace std;

//g++ -o example ./snappy-compress.cc -I. -L. -lsnappy
//fisher code
uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

string readNextDoc(int file_reading_counter){
    string DocDir = "/home/node10/compress/dataset/100000/";
    ifstream inFile;
    stringstream strStream;

    string filename = to_string(file_reading_counter);

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

string readNextBinaries(int file_reading_counter){
    string rawDocDir = "/home/node10/compress/snaTmp/dataset/100000/a";

    ifstream inFile;
    stringstream strStream;

    string filename = to_string(file_reading_counter);

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
    ofstream outFile;
    stringstream strStream(output);

    string outDocDir = "/home/node10/compress/snaTmp/dataset/100000/";

    string outDocfile = outDocDir+to_string(file_reading_counter)+".txt";

    outFile.open(outDocfile,ofstream::out);
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
    ofstream outFile;
    stringstream strStream(output);

    string outDocDir = "/home/node10/compress/snaTmp/dataset/100000/";

    string outDocfile = outDocDir+"a"+to_string(file_reading_counter)+".txt";

    outFile.open(outDocfile,ofstream::out);
    //C++流对象函数rdbuf()简介:
    //rdbuf()可以实现一个流对象指向的内容用另一个流对象来输出
    outFile<<strStream.rdbuf();
    outFile.close();

    strStream.clear();
    isOk = true;

    return isOk;
}




int main() {


    string output;
    string output2;
    string input;
    string output_uncom;

    vector<int> inputSizes;
    vector<int> outputSizes;

    int file_reading_counter = 1;

    WriteNextBinaries(2,"helloworls");

    cout << "input size  ---  output size  ---  compress rate"<<endl;

    // while(file_reading_counter<=100){
    //     input = readNextDoc(file_reading_counter);

    //     snappy::Compress(input.data(), input.size(), &output);

    //     WriteNextBinaries(file_reading_counter,output);

    //     output2 = readNextBinaries(file_reading_counter);

    //     if(output2!=output){
    //         cout<<"Read ERROR!"<<endl;
    //         break;
    //     }

    //     cout<< "   "<<input.size() <<"   ---      "<< output.size()<<"   ---     "<< (double)output.size()/input.size() << endl;

    //     snappy::Uncompress(output2.data(), output2.size(), &output_uncom);

    //     WriteNextDoc(file_reading_counter,output_uncom); 
        
    //     if (input != output_uncom) {
    //         cout << "ERROR: not equal" << endl;
    //     }

    //     file_reading_counter++;
    // }
	uint64_t start_add_time2 =  timeSinceEpochMillisec(); 

    while(file_reading_counter<=100000){
        input = readNextDoc(file_reading_counter);

        snappy::Compress(input.data(), input.size(), &output);

        inputSizes.push_back(input.size());

        WriteNextBinaries(file_reading_counter,output);

        file_reading_counter++;
    }

    uint64_t end_add_time2 =  timeSinceEpochMillisec(); //插入操作结束时间
	std::cout << "********Time for Compressing********" << std::endl;
	std::cout << "Total time:" << end_add_time2-start_add_time2 << " ms" << std::endl;
    
    file_reading_counter = 1;

    uint64_t start_add_time =  timeSinceEpochMillisec(); 

    while(file_reading_counter<=100000){

        output2 = readNextBinaries(file_reading_counter);

        outputSizes.push_back(output2.size());

        snappy::Uncompress(output2.data(), output2.size(), &output_uncom);

        WriteNextDoc(file_reading_counter,output_uncom); 
        
        file_reading_counter++;
    }



    // for(int i = 0;i<100000;i++){
        
    //     cout<< "   "<<inputSizes[i] <<"   ---      "<< outputSizes[i]<<"   ---     "<< ((double)outputSizes[i]/inputSizes[i]) << endl;

    // }
    
    uint64_t end_add_time =  timeSinceEpochMillisec(); //插入操作结束时间
	std::cout << "********Time for Compressing********" << std::endl;
	std::cout << "Total time:" << end_add_time-start_add_time << " ms" << std::endl;

    return 0;
}