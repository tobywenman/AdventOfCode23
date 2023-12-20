#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "main2.hpp"

uint8_t hashMap::hash(std::string str)
{
    unsigned long hashVal=0;
    for (char i : str)
    {
        hashVal += i;
        hashVal *= 17;
        hashVal %= 256;
    }
    return (uint8_t)hashVal;
}

void hashMap::add(std::string str, uint8_t val)
{
    uint8_t hashVal = hash(str);
    bool found = false;
    for (auto it=data[hashVal].begin(); it!=data[hashVal].end(); it++)
    {
        if (str == std::get<0>(*it))
        {
            found = true;
            std::get<1>(*it) = val;
            break;
        }
    }
    if (!found)
    {
        data[hashVal].push_back(std::make_tuple(str,val));
    }
}

void hashMap::sub(std::string str)
{
    uint8_t hashVal = hash(str);
    for (auto it=data[hashVal].begin(); it!=data[hashVal].end(); it++)
    {
        if (str == std::get<0>(*it))
        {
            data[hashVal].erase(it);
            break;
        }
    }
}

unsigned hashMap::sum()
{
    unsigned total=0;
    for (unsigned i=0; i<data.size(); i++)
    {
        unsigned slot=1;
        for (auto it=data[i].begin(); it!=data[i].end(); it++)
        {
            unsigned addTotal = (i+1)*slot*std::get<1>(*it);
            total += addTotal;
            slot++;
        }
    }
    return total;
}

int main()
{
    std::ifstream ifs;
    ifs.open("data.txt", std::ios::in);

    std::string str;
    hashMap values;

    while (std::getline(ifs, str, ','))
    {
        if (str[str.size()-1] == '-')
        {
            values.sub(str.substr(0,str.size()-1));
        }
        else
        {
            values.add(str.substr(0,str.size()-2), str[str.size()-1]-'0');
        }
    }
    std::cout << "total: " << values.sum() << '\n';
}