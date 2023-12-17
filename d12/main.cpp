#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

struct test
{
    std::string str;
    unsigned test;
    unsigned nextVal;
    unsigned current;
    bool dot;
};

unsigned backTrack(std::string str, std::vector<unsigned> vals)
{
    unsigned out=0;

    std::stack<test> tests;

    test newTest;

    newTest.current = 0;
    newTest.test = 0;
    newTest.nextVal = 0;
    newTest.dot = true;
    newTest.str = str;

    for (unsigned i=0; i<str.size(); i++)
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

        for (unsigned i=curTest.test; i<str.size(); i++)
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



int main()
{
    std::ifstream ifs;

    ifs.open("data.txt", std::ios::in);

    std::string line;

    uint64_t total = 0;

    while (std::getline(ifs, line))
    {
        std::string str;
        unsigned i;
        for (i=0; i<line.size(); i++)
        {
            if (line[i] == ' ')
            {
                break;
            }
            str.push_back(line[i]);
        }

        std::string numStr;
        std::vector<unsigned> nums;
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
        total += backTrack(str, nums);
    }
    std::cout << "result: " << total << '\n';    
}