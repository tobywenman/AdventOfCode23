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

bool testMirror(const grid &in, bool vertical, unsigned pos, unsigned offset)
{
    unsigned top = pos+1;
    unsigned bot = pos;

    unsigned max = (vertical) ? in[0].size() : in.size();

    while (true)
    {
        char test1 = (vertical) ? in[offset][bot] : in[bot][offset];
        char test2 = (vertical) ? in[offset][top] : in[top][offset];

        if (test1 != test2)
        {
            return false;
        }
        else if (top == max-1 || bot == 0)
        {
            return true;
        }
        else
        {
            bot--;
            top++;
        }
    }
}

bool find(const grid &in ,unsigned &pos, bool &vertical, unsigned oldPos, bool oldVert)
{
    std::vector<bool> vertValid;
    vertValid.resize(in[0].size()-1, true);
    for (unsigned i=0; i<in.size(); i++)
    {
        for (unsigned j=0; j<vertValid.size(); j++)
        {
            if (vertValid[j])
            {
                vertValid[j] = testMirror(in, true, j, i);
            }
        }
    }
    for (unsigned i=0; i<vertValid.size(); i++)
    {
        if (vertValid[i] && (i != oldPos || !oldVert))
        {
            pos = i;
            vertical = true;
            return true;
        }
    }

    std::vector<bool> horiValid;
    horiValid.resize(in.size()-1, true);
    for (unsigned i=0; i<in[0].size(); i++)
    {
        for (unsigned j=0; j<horiValid.size(); j++)
        {
            if (horiValid[j])
            {
                horiValid[j] = testMirror(in, false, j, i);
            }
        }
    }
    for (unsigned i=0; i<horiValid.size(); i++)
    {
        if (horiValid[i] && (oldVert || i != oldPos))
        {
            pos = i;
            vertical = false;
            return true;
        }
    }

    return false;
}

bool findSmudge(grid in, unsigned &pos, bool &vert)
{
    unsigned oldPos;
    bool oldVert;

    assert(find(in, oldPos, oldVert, 99999999, false));

    for (unsigned i=0; i<in.size(); i++)
    {
        for (unsigned j=0; j<in[0].size(); j++)
        {
            bool found;
            if (in[i][j]=='.')
            {
                in[i][j] = '#';
                found = find(in, pos, vert, oldPos, oldVert);
                in[i][j] = '.';
            }
            else
            {
                in[i][j] = '.';
                found = find(in, pos, vert, oldPos, oldVert);
                in[i][j] = '#';
            }
            if (found)
            {
                return true;
            }
        }
    }
    return false;
}

int main()
{
    grid test;
    std::ifstream ifs;
    ifs.open("data.txt", std::ios::in);
    
    uint64_t total=0;
    while (readGrid(ifs, test))
    {
        unsigned pos;
        bool vert;
        assert(findSmudge(test, pos, vert));

        total += (vert) ? pos+1 : (pos+1)*100;
    }

    std::cout << "total: " << total << '\n';
}