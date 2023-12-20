#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <unordered_map>

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

void rollWest(grid &in)
{
    for (unsigned i=1; i<in[0].size(); i++)
    {
        for (unsigned j=0; j<in.size(); j++)
        {
            if (in[j][i] == 'O')
            {
                in[j][i] = '.';
                unsigned end=0;
                for (unsigned k=i; k>0; k--)
                {
                    if (in[j][k-1] != '.')
                    {
                        end = k;
                        break;
                    }
                }
                in[j][end] = 'O';
            }
        }
    }
}

void rollSouth(grid &in)
{
    for (unsigned i=in.size()-1; i-->0;)
    {
        for (unsigned j=0; j<in[0].size(); j++)
        {
            if (in[i][j] == 'O')
            {
                in[i][j] = '.';
                unsigned end=in.size()-1;
                
                for (unsigned k=i; k<in.size()-1; k++)
                {
                    if (in[k+1][j] != '.')
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

void rollEast(grid &in)
{
    for (unsigned i=in[0].size()-1; i-->0;)
    {
        for (unsigned j=0; j<in.size(); j++)
        {
            if (in[j][i] == 'O')
            {
                in[j][i] = '.';
                unsigned end=in[0].size()-1;
                for (unsigned k=i; k<in[0].size()-1; k++)
                {
                    if (in[j][k+1] != '.')
                    {
                        end = k;
                        break;
                    }
                }
                in[j][end] = 'O';
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

std::vector<bool> hashGrid(const grid &in)
{
    std::vector<bool> out;
    for (unsigned i=0; i<in.size(); i++)
    {
        for (unsigned j=0; j<in[0].size(); j++)
        {
            if (in[i][j]=='O')
            {
                out.push_back(true);
            }
            else
            {
                out.push_back(false);
            }
        }
    }
    return out;
}

void spin(grid &in)
{
    rollNorth(in);
    rollWest(in);
    rollSouth(in);
    rollEast(in);
}

int main()
{
    grid test;
    std::ifstream ifs;
    ifs.open("data.txt", std::ios::in);
    
    uint64_t total=0;
    readGrid(ifs, test);
    
    std::unordered_map<std::vector<bool>, unsigned> stepCache;

    unsigned step=0;
    while (true)
    {
        std::vector<bool> hash = hashGrid(test);

        if (stepCache.count(hash))
        {
            unsigned remaining = (1000000000 - stepCache[hash])%(step - stepCache[hash]);
            std::cout << "remaining: " << remaining << '\n';
            std::cout << "steps: " << step << '\n';
            std::cout << "loop size: " << step-stepCache[hash] << '\n';

            for (unsigned i=0; i<remaining; i++)
            {
                spin(test);
            }
            break;
        }

        stepCache[hash] = step;
        spin(test);
        step++;
    }

    total = countWeight(test);

    std::cout << "size: " << test.size() << ',' << test[0].size() << '\n';

    std::cout << "weight: " << total << '\n';
}
