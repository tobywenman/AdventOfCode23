#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>

using grid = std::vector<std::vector<char>>;

bool readGrid(std::ifstream &ifs, grid &out)
{
    out.clear();
    std::string line;

    bool found = false;

    while (std::getline(ifs, line))
    {
        found = true;
        if (line.size() <= 1)
        {
            break;
        }
        std::vector<char> lineVec;
        for (char i : line)
        {
            lineVec.push_back(i);
        }
        out.push_back(lineVec);
    }
    return found;
}

void printGrid(const grid &in)
{
    for (unsigned i=0 ; i<in.size(); i++)
    {
        for (unsigned j=0; j<in[0].size(); j++)
        {
            std::cout << in[i][j];
        }
        std::cout << '\n';
    }
}

void rollNorth(grid &in)
{
    for (unsigned i=1; i<in.size(); i++)
    {
        for (unsigned j=0; j<in[0].size(); j++)
        {
            if (in[i][j] == 'O')
            {
                in[i][j] = '.';
                unsigned end=0;
                for (unsigned k=i; k>0; k--)
                {
                    if (in[k-1][j] != '.')
                    {
                        end = k;
                        break;
                    }
                }
                in[end][j] = 'O';
            }
        }
    }
}

unsigned countWeight(const grid &in)
{
    unsigned weight=0;
    for (unsigned i=0; i<in.size(); i++)
    {
        unsigned weightMult = in.size()-i;
        for (unsigned j=0; j<in[0].size(); j++)
        {
            if (in[i][j] == 'O')
            {
                weight += weightMult;
            }
        }
    }
    return weight;
}

int main()
{
    grid test;
    std::ifstream ifs;
    ifs.open("data.txt", std::ios::in);
    
    uint64_t total=0;
    readGrid(ifs, test);
    rollNorth(test);
    total = countWeight(test);

    std::cout << "weight: " << total << '\n';
}
