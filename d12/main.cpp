#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <algorithm>

struct test
{
    std::string str;
    uint64_t test;
    uint64_t nextVal;
    uint64_t current;
    bool dot;
};

uint64_t backTrack(std::string str, std::vector<uint64_t> vals)
{
    uint64_t out=0;

    std::stack<test> tests;

    test newTest;

    newTest.current = 0;
    newTest.test = 0;
    newTest.nextVal = 0;
    newTest.dot = true;
    newTest.str = str;

    for (uint64_t i=0; i<str.size(); i++)
    {
        if (str[i]=='#')
        {
            newTest.current++;
        }
        else if (str[i]=='.')
        {
            if (newTest.current != 0)
            {
                newTest.nextVal++;
                newTest.current = 0;
            }
        }
        else
        {
            newTest.test = i;
            break;
        }
    }

    tests.push(newTest);

    while (true)
    {
        next:
        if (tests.size() == 0)
        {
            break;
        }
        test curTest = tests.top();
        tests.pop();

        if (curTest.dot)
        {
            curTest.dot = false;
            tests.push(curTest);
            curTest.str[curTest.test] = '.';
        }
        else
        {
            curTest.str[curTest.test] = '#';
        }

        for (uint64_t i=curTest.test; i<str.size(); i++)
        {
            if (curTest.str[i] == '#')
            {
                curTest.current++;
                if (vals.size() == curTest.nextVal)
                {
                    goto next;
                }
                else if (curTest.current > vals[curTest.nextVal])
                {
                    goto next;
                }
                else
                {
                    curTest.test++;                
                }
            }
            else if (curTest.str[i] == '.')
            {
                curTest.test++;
                if (curTest.current != 0)
                {
                    if (vals[curTest.nextVal] != curTest.current)
                    {
                        goto next;
                    }
                    else
                    {
                        curTest.nextVal++;
                    }
                }
                curTest.current = 0;
            }
            else
            {
                curTest.dot = true;
                tests.push(curTest);
                goto next;
            }
        }
        if (curTest.current)
        {
            if(curTest.current == vals[curTest.nextVal])
            {
                curTest.nextVal++;
            }
        }
        if (curTest.nextVal == vals.size())
        {
            out++;
        }
    }   
    return out;
}

std::string unfold(std::string str)
{
    std::string out;
    
    for (uint64_t i=0; i<4; i++)
    {
        out.append(str);
        out.push_back('?');
    }
    out.append(str);

    return out;
}

std::vector<uint64_t> unfold(std::vector<uint64_t> vals)
{
    std::vector<uint64_t> out;

    for (uint64_t i=0; i<5; i++)
    {
        for (auto i : vals)
        {
            out.push_back(i);
        }
    }
    return out;
}

uint64_t iterative(std::string str, std::vector<uint64_t> vals)
{
    std::unordered_map<uint64_t,uint64_t> positions;
    positions[0] = 1;

    for (uint64_t i=0; i<vals.size(); i++)
    {
        std::unordered_map<uint64_t,uint64_t> newPos;

        for (auto it=positions.begin(); it!=positions.end(); it++)
        {
            auto lowBound = (i+1 >= vals.size()) ? vals.end() : vals.begin()+i+1;

            uint64_t sum=0;

            for (auto it=lowBound; it!=vals.end();it++)
                sum += *it;


            uint64_t top = str.size() + (vals.end()-lowBound) - sum;
            for (uint64_t j=it->first; j<top; j++)
            {
                if (j+vals[i]-1<str.size() && str.substr(j, vals[i]).find('.')==std::string::npos)
                {
                    if ((i == vals.size()-1 && str.substr(j+vals[i]).find('#') == std::string::npos) || (i < vals.size()-1 && j+vals[i]<str.size() && str[j+vals[i]] != '#'))
                    {
                        newPos[j+vals[i]+1] += it->second;
                    }
                }
                if (str[j] == '#')
                {
                    break;
                }
            }
        }
        positions = newPos;
    }
    uint64_t total=0;
    for (auto i : positions)
        total += i.second;

    return total;
}

int main()
{
    std::ifstream ifs;

    ifs.open("data.txt", std::ios::in);

    std::string line;

    uint64_t total = 0;

    uint64_t test=1;

    while (std::getline(ifs, line))
    {
        test++;
        std::string str;
        uint64_t i;
        for (i=0; i<line.size(); i++)
        {
            if (line[i] == ' ')
            {
                break;
            }
            str.push_back(line[i]);
        }

        std::string numStr;
        std::vector<uint64_t> nums;
        for (; i<line.size(); i++)
        {
            if(line[i] != ',')
            {
                numStr.push_back(line[i]);
            }
            else
            {
                nums.push_back(std::stoul(numStr));
                numStr.clear();
            }
        }
        nums.push_back(std::stoul(numStr));

        str = unfold(str);
        nums = unfold(nums);

        total += iterative(str, nums);
    }
    std::cout << "result: " << total << '\n';    
}