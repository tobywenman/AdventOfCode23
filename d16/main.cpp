#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

using grid = std::vector<std::vector<char>>;


class beams
{
    public:
        enum dirEnum
        {
            up,
            down,
            left,
            right
        };
    private:
        struct beamHead
        {
            unsigned row,col;
            dirEnum curDir;
        };
    public:
        beams(const grid &in, dirEnum initDir, unsigned offset);
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
        newHead.curDir = beams::up;
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
        newHead.curDir = beams::down;
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
        newHead.curDir = beams::left;
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
        newHead.curDir = beams::right;
        heads.push(newHead);
    }
}

beams::beams(const grid &in, dirEnum initDir, unsigned offset) : map(in)
{
    dirs blankDirs = {false, false, false, false};
    dirGrid.resize(in.size());
    for (auto it=dirGrid.begin(); it!=dirGrid.end();it++)
    {
        it->resize(in[0].size(),blankDirs);
    }
    beamHead init;

    switch (initDir)
    {
    case beams::down:
        init.row = 0;
        init.col = offset;
        break;
    case beams::up:
        init.row = in.size()-1;
        init.col = offset;
        break;
    case beams::right:
        init.col = 0;
        init.row = offset;
        break;
    case beams::left:
        init.col = in[0].size()-1;
        init.row = offset;
        break;
    default:
        break;
    }

    init.curDir = initDir;
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
        case beams::up:
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
        case beams::down:
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
        case beams::left:
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
        case beams::right:
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

    unsigned max = 0;

    for (unsigned i=0; i<mirrors.size(); i++)
    {
        beams beamGrid(mirrors, beams::right, i);
        beamGrid.bounce();
        unsigned count = beamGrid.count();
        max = (max < count) ? count : max; 
    }
    for (unsigned i=0; i<mirrors.size(); i++)
    {
        beams beamGrid(mirrors, beams::left, i);
        beamGrid.bounce();
        unsigned count = beamGrid.count();
        max = (max < count) ? count : max; 
    }
    for (unsigned i=0; i<mirrors[0].size(); i++)
    {
        beams beamGrid(mirrors, beams::up, i);
        beamGrid.bounce();
        unsigned count = beamGrid.count();
        max = (max < count) ? count : max; 
    }
    for (unsigned i=0; i<mirrors[0].size(); i++)
    {
        beams beamGrid(mirrors, beams::down, i);
        beamGrid.bounce();
        unsigned count = beamGrid.count();
        max = (max < count) ? count : max; 
    }

    std::cout << "count: " << max << '\n';
}