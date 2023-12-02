#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>

class turn
{
    public:
        enum colours
        {
            red=0,
            green=1,
            blue=2
        };
        void add(unsigned count, colours col);
        bool check();
    private:
        std::array<unsigned, 3> values;
};

class game
{
    public:
        game(std::string line);
        bool check();
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

void turn::add(unsigned count, colours col)
{
    values[(unsigned)col] += count;
}

bool game::check()
{
    for (auto t : turns)
    {
        if (t.check() == false)
        {
            return false;
        }
    }
    return true;
}

game::game(std::string line)
{
    std::stringstream ss(line);

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
                newTurn.add(count, turn::red);
            }
            else if (colour == "blue")
            {
                newTurn.add(count, turn::blue);
            }
            else if (colour == "green")
            {
                newTurn.add(count, turn::green);
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
    
    std::cout << gameData.sum() << '\n';
}