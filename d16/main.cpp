#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

using grid = std::vector<std::vector<char>>;


class beams
{
    struct beamHead
    {
        enum dirEnum
        {
            up,
            down,
            left,
            right
        };
        unsigned row,col;
        dirEnum curDir;
    };
    public:
        beams(const grid &in);
        void bounce();
        unsigned count();
    private:
        struct dirs
        {
            bool up;
            bool down;
            bool left;
            bool right;
        };
        std::vector<std::vector<dirs>> dirGrid;
        std::stack<beamHead> heads;
        const grid &map;
        void addUp(unsigned row, unsigned col);
        void addDown(unsigned row, unsigned col);
        void addLeft(unsigned row, unsigned col);
        void addRight(unsigned row, unsigned col);
};

void beams::addUp(unsigned row, unsigned col)
{
    beamHead newHead;
    if (row>0)
    {
        newHead.row = row-1;
        newHead.col = col;
        newHead.curDir = beamHead::up;
        heads.push(newHead);
    }
}
void beams::addDown(unsigned row, unsigned col)
{
    beamHead newHead;
    if (row<map.size()-1)
    {
        newHead.row = row+1;
        newHead.col = col;
        newHead.curDir = beamHead::down;
        heads.push(newHead);
    }
}
void beams::addLeft(unsigned row, unsigned col)
{
    beamHead newHead;
    if (col>0)
    {
        newHead.col = col-1;
        newHead.row = row;
        newHead.curDir = beamHead::left;
        heads.push(newHead);
    }
}
void beams::addRight(unsigned row, unsigned col)
{
    beamHead newHead;
    if (col<map[0].size()-1)
    {
        newHead.col = col+1;
        newHead.row = row;
        newHead.curDir = beamHead::right;
        heads.push(newHead);
    }
}

beams::beams(const grid &in) : map(in)
{
    dirGrid.resize(in.size());
    for (auto it=dirGrid.begin(); it!=dirGrid.end();it++)
    {
        it->resize(in[0].size());
    }
    beamHead init;
    init.row = 0;
    init.col = 0;
    init.curDir = beamHead::right;
    heads.push(init);
}

void beams::bounce()
{
    while (heads.size())
    {
        beamHead curHead = heads.top();
        heads.pop();
        switch (curHead.curDir)
        {
        case beamHead::up:
            if (!dirGrid[curHead.row][curHead.col].up)
            {
                dirGrid[curHead.row][curHead.col].up = true;
            }
            else
            {
                break;
            }
            switch (map[curHead.row][curHead.col])
            {
            case '.':
            case '|':
                addUp(curHead.row, curHead.col);
                break;
            case '/':
                addRight(curHead.row, curHead.col);
                break;
            case '\\':
                addLeft(curHead.row, curHead.col);
                break;
            case '-':
                addLeft(curHead.row, curHead.col);
                addRight(curHead.row, curHead.col);
                break;
            default:
                break;
            }
            break;
        case beamHead::down:
            if (!dirGrid[curHead.row][curHead.col].down)
            {
                dirGrid[curHead.row][curHead.col].down = true;
            }
            else
            {
                break;
            }
            switch (map[curHead.row][curHead.col])
            {
            case '.':
            case '|':
                addDown(curHead.row, curHead.col);
                break;
            case '/':
                addLeft(curHead.row, curHead.col);
                break;
            case '\\':
                addRight(curHead.row, curHead.col);
                break;
            case '-':
                addLeft(curHead.row, curHead.col);
                addRight(curHead.row, curHead.col);
                break;
            default:
                break;
            }
            break;
        case beamHead::left:
            if (!dirGrid[curHead.row][curHead.col].left)
            {
                dirGrid[curHead.row][curHead.col].left = true;
            }
            else
            {
                break;
            }
            switch (map[curHead.row][curHead.col])
            {
            case '.':
            case '-':
                addLeft(curHead.row, curHead.col);
                break;
            case '/':
                addDown(curHead.row, curHead.col);
                break;
            case '\\':
                addUp(curHead.row, curHead.col);
                break;
            case '|':
                addUp(curHead.row, curHead.col);
                addDown(curHead.row, curHead.col);
                break;
            default:
                break;
            }
            break;
        case beamHead::right:
            if (!dirGrid[curHead.row][curHead.col].right)
            {
                dirGrid[curHead.row][curHead.col].right = true;
            }
            else
            {
                break;
            }
            switch (map[curHead.row][curHead.col])
            {
            case '.':
            case '-':
                addRight(curHead.row, curHead.col);
                break;
            case '/':
                addUp(curHead.row, curHead.col);
                break;
            case '\\':
                addDown(curHead.row, curHead.col);
                break;
            case '|':
                addUp(curHead.row, curHead.col);
                addDown(curHead.row, curHead.col);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

unsigned beams::count()
{
    unsigned out=0;
    for (auto it=dirGrid.begin(); it!=dirGrid.end(); it++)
    {
        for (auto jt=it->begin(); jt!=it->end(); jt++)
        {
            if (jt->down || jt->up || jt->left || jt->right)
            {
                out++;
            }
        }
    }
    return out;
}

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

int main()
{
    std::ifstream ifs;
    ifs.open("data.txt", std::ios::in);

    grid mirrors;

    readGrid(ifs, mirrors);

    beams beamGrid(mirrors);

    beamGrid.bounce();

    std::cout << "count: " << beamGrid.count() << '\n';
}