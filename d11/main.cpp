#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

struct puzzleData
{
    struct galaxy
    {
        unsigned row;
        unsigned col;
    };
    
    std::vector<galaxy> galaxies;
    std::vector<unsigned> vertSplits;
    std::vector<unsigned> horiSplits;

    void getData(std::string fileName);
    unsigned long solve(unsigned growth=1);
};

void puzzleData::getData(std::string fileName)
{
    std::ifstream ifs;

    ifs.open(fileName, std::ios::in);

    std::string line;
    std::vector<std::string> data;
    while(std::getline(ifs, line))
    {
        data.push_back(line);
    }

    std::vector<bool> rowSplits;
    std::vector<bool> colSplits;
    rowSplits.resize(data.size(),true);
    colSplits.resize(data[0].size(),true);
    for (unsigned i=0; i<data.size(); i++)
    {
        for (unsigned j=0; j<data[i].size(); j++)
        {
            if (data[i][j] == '#')
            {
                galaxy newGal = {i,j};
                galaxies.push_back(newGal);
                rowSplits[i]=false;
                colSplits[j]=false;
            }
        }
    }

    for (unsigned i=0; i<rowSplits.size(); i++)
    {
        if (rowSplits[i])
        {
            horiSplits.push_back(i);
        }
    }
    for (unsigned i=0; i<colSplits.size(); i++)
    {
        if (colSplits[i])
        {
            vertSplits.push_back(i);
        }
    }
}

unsigned long puzzleData::solve(unsigned growth)
{
    unsigned long totalDist=0;
    for (unsigned i=0; i<galaxies.size()-1; i++)
    {
        for (unsigned j=i+1; j<galaxies.size(); j++)
        {
            unsigned rowRange[2];
            unsigned colRange[2];

            if (galaxies[i].row < galaxies[j].row)
            {
                rowRange[0] = galaxies[i].row+1;
                rowRange[1] = galaxies[j].row;
            }
            else
            {
                rowRange[0] = galaxies[j].row+1;
                rowRange[1] = galaxies[i].row;
            }

            if (galaxies[i].col < galaxies[j].col)
            {
                colRange[0] = galaxies[i].col+1;
                colRange[1] = galaxies[j].col;
            }
            else
            {
                colRange[0] = galaxies[j].col+1;
                colRange[1] = galaxies[i].col;
            }

            totalDist += rowRange[1] - rowRange[0] + 1;
            totalDist += colRange[1] - colRange[0] + 1;

            for (unsigned i=rowRange[0]; i<rowRange[1]; i++)
            {
                if (std::find(horiSplits.begin(), horiSplits.end(), i) != horiSplits.end())
                {
                    totalDist += growth;
                }
            }
            for (unsigned i=colRange[0]; i<colRange[1]; i++)
            {
                if (std::find(vertSplits.begin(), vertSplits.end(), i) != vertSplits.end())
                {
                    totalDist += growth;
                }
            }
        }
    }
    return totalDist;
}

int main()
{
    puzzleData data;

    data.getData("data.txt");
    std::cout << "ans: " << data.solve(999999) << '\n';
}