#include <iostream>
#include <fstream>
#include <string>

uint8_t hash(std::string str)
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

int main()
{
    std::ifstream ifs;
    ifs.open("data.txt", std::ios::in);

    std::string str;
    unsigned total=0;
    while (std::getline(ifs, str, ','))
    {
        total += hash(str);
        // std::cout << "hashVal: " << (unsigned)hash(str) << '\n';
    }
    std::cout << "total: " << total << '\n';
}