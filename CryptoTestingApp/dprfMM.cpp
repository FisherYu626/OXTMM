#include "dprfMM.h"

dprfMM::dprfMM(int _n_size, uint _init_key, uint _init_value) : n_size(_n_size), init_key(_init_key), init_value(_init_value)
{
    RAND_bytes(KD, ENC_KEY_SIZE);
    RAND_bytes(KE, ENC_KEY_SIZE);

    std::cout << "init_dprfMM" << std::endl;
    std::cout << n_size << std::endl;

    T1.resize(3000, std::pair<int, int>{(uint)init_key, (uint)init_value});
    T2.resize(3000, std::pair<int, int>{(uint)init_key, (uint)init_value});
}
// SDBMHash
unsigned int dprfMM::hash_fun1(char *str)
{
    unsigned int hash = 0;

    while (*str)
    {

        hash = 65599 * hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }
    return (hash & 0x7FFFFFFF) % n_size;
}
unsigned int dprfMM::hash_fun2(char *str)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str)
    {

        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash & 0x7FFFFFFF) % n_size;
}

void dprfMM::Setup(std::vector<std::pair<std::string, std::pair<std::string, std::string>>>& YMM)
{

    return;
}

void dprfMM::Setup(std::map<std::string, std::vector<std::string>> &MM,
                   std::vector<std::vector<std::string>> &xlist)
{
    int i_index = 0;

    double d = 1;

    for (auto i : MM)
    {
        // if(i_index>5) break;
        for (int j = 0; j < i.second.size(); j++)
        {
            int count = 0;
            bool tmp = false;
            int i0 = i_index;
            int j0 = j;
            // std::cout<<i.first<<" ";
            // std::cout<<T1.size()<<"T2.size()"<<T2.size()<<"\n";
            while (count < 11)
            {
                
                for (int t = 1; t <= 2; t++)
                {   
                    unsigned char * xlistAT =(unsigned char *)malloc(xlist[i0][j0].size()+sizeof(int));
                    memcpy(xlistAT,xlist[i0][j0].c_str(),xlist[i0][j0].size());
                    memcpy(xlistAT+xlist[i0][j0].size(),&t,4);
                    //print_bytes(xlistAT,xlist[i0][j0].size()+sizeof(int));
                    // std::string xlistAndT(xlist[i0][j0]);
                    // xlistAndT = xlistAndT + std::to_string(t);
                    // std::cout<<"st4r len is"<<xlistAndT.size()<<"\n";
                    // uint pos = hash_fun2((char *)xlistAT.c_str());
                    uint pos1 = hash_fun1((char *)xlistAT);
                    uint pos2 = hash_fun2((char *)xlistAT);

                    std::cout << "i is" << i0 << " j is" << j0 << "\n";
                    std::cout <<"now is insert t"<<t<< " pos is " << pos1 << "\n";
                    std::cout <<"now is insert t"<<t<< " pos is " << pos2 << "\n";
                    //std::cout << "count is " << count << "\n";
                    
                    //count++;
                    //std::cout << "T1.size is " << T1.size() << "\n";

                    /*
                    terminate called after throwing an instance of 'std::logic_error'
                    what():  basic_string::_M_construct null not valid
                    已放弃 (核心已转储)
                    */
                    if (t == 1)
                    {   
                        // std::cout<<"init key is"<<init_key<<"\n";
                        // std::cout << "T1[pos].first is" << T1[pos].first;
                        if (T1[pos1].first == init_key)
                        {
                            T1[pos1].first = i0;
                            // std::cout << "T1[pos].first is" << T1[pos].first<<"\n";
                            T1[pos1].second = j0;
                            // std::cout << " T1[pos].second is " << T1[pos].second<<"\n";
                            tmp = true;
                            break;
                        }
                        else
                        {
                            int i1 = T1[pos1].first;
                            int j1 = T1[pos1].second;
                            T1[pos1].first = i0;
                            T1[pos1].second = j0;
                            i0 = i1;
                            j0 = j1;
                            count++;
                        }
                    }
                    else
                    {
                        if (T2[pos2].first == init_key)
                        {
                            T2[pos2].first = i0;
                            // std::cout << "T2[pos].first is" << T2[pos].first<<"\n";
                            T2[pos2].second = j0;
                            // std::cout << " T2[pos].second is " << T2[pos].second<<"\n";
                            tmp = true;
                            break;
                        }
                        else
                        {
                            int i1 = T2[pos2].first;
                            int j1 = T2[pos2].second;
                            T2[pos2].first = i0;
                            T2[pos2].second = j0;
                            i0 = i1;
                            j0 = j1;
                            count++;
                        }
                    }
                }
                if (tmp == true){
                    std::cout<<"break once \n";
                    break;
                }
                
            }
            if (count == 11)
            {
                std::cout<<"stash is inserting!!!"<<"\n";
                stash.insert(std::pair<std::string, std::string>{i.first, i.second[j]});
            }
        }
        i_index++;
    }

    if (stash.size() > fn)
    {
        std::cout << "stash out of memory!!!!"
                  << "\n";
        exit;
    }

    return;
}
