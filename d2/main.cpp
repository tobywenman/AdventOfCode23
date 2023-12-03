#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>

class turn
{
    public:
        turn();
        enum colours
        {
            red=0,
            green=1,
            blue=2
        };
        void set(unsigned count, colours col);
        bool check();
        std::array<unsigned, 3> values;
};

class game
{
    public:
        game(std::string line);
        bool check();
        unsigned minMult();
        unsigned id;
    private:
        std::vector<turn> turns;
};

class games
{
    public:
        games(std::ifstream *file);
        void print();
        unsigned sum();
        unsigned mults();
    private:
        std::vector<game> data;
};

bool turn::check()
{
    if (values[0] <= 12)
    {
        if (values[1] <= 13)
        {
            if (values[2] <= 14)
            {
                return true;
            }
        }
    }
    return false;
}

turn::turn()
{
    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
}

void turn::set(unsigned count, colours col)
{
    values[(unsigned)col] = count;
}

bool game::check()
{
    for (auto t : turns)
    {
        if (t.check() == false)
        {
            std::cout << id << '\n';
            return false;
        }
    }
    return true;
}

game::game(std::string line)
{
    std::stringstream ss(line);
    turns.clear();

    std::string tmp;
    ss >> tmp;
    ss >> id;
    ss >> tmp;
    while (ss.good())
    {
        turn newTurn;
        while (true)
        {
            unsigned count;
            char back;
            std::string colour;

            ss >> count;
            ss >> colour;
            back = colour.back();
            
            if (back == ',' || back == ';')
                colour.pop_back();

            if (colour == "red")
            {
                newTurn.set(count, turn::red);
            }
            else if (colour == "blue")
            {
                newTurn.set(count, turn::blue);
            }
            else if (colour == "green")
            {
                newTurn.set(count, turn::green);
            }

            if (back == ';' || back != ',')
            {
                turns.push_back(newTurn);
                break;
            }
        }
    }
}

unsigned games::sum()
{
    unsigned total=0;
    for (auto g : data)
    {
        if (g.check())
            total += g.id;
    }
    return total;
}

unsigned game::minMult()
{
    std::array<unsigned,3> maxVals = {0,0,0};
    for (auto t : turns)
    { 
        for (unsigned i=0; i<3; i++)
        {
            if (maxVals[i] < t.values[i])
            {
                maxVals[i] = t.values[i];
            }
        }
    }
    unsigned out=1;
    for (auto i : maxVals)
    {
        out *= i;
    }
    return out;
}

unsigned games::mults()
{
    unsigned total=0;
    for (auto g : data)
    {
        std::cout << g.minMult() << '\n';
        total += g.minMult();
    }
    return total;
}

games::games(std::ifstream *file)
{
    std::string gameStr;
    while (std::getline(*file, gameStr))
    {
        data.push_back(game(gameStr));
    }
}

int main()
{
    std::ifstream file;

    file.open("data.txt", std::ios::in);

    games gameData(&file);
    
    std::cout << gameData.mults() << '\n';
}