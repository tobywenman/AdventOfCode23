#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

struct route
{
    char *dirs;
    size_t numDirs;
};

struct node
{
    size_t left;
    size_t right;
    bool end;
};

struct node nodes[17576]; //size of 3 digit base 26 number

struct route getRoute(FILE *fp)
{
    struct route out;

    size_t bufSize = 300;
    out.dirs = malloc(sizeof(char)*bufSize);

    out.numDirs = getline(&out.dirs, &bufSize, fp)-1;
    out.dirs = realloc(out.dirs, sizeof(char)*out.numDirs);
    return out;
}

size_t getIdx(char label[3])
{
    size_t out = 0;
    out += (size_t)(label[2]-'A');
    out *= 26;
    out += (size_t)(label[1]-'A');
    out *= 26;
    out += (size_t)(label[0]-'A');
    return out;
}

uint64_t gcd(uint64_t a, uint64_t b)
{
    uint64_t temp;
    while (b != 0)
    {
        temp = a % b;

        a = b;
        b = temp;
    }
    return a;
}

uint64_t lcm(uint64_t a, uint64_t b)
{
    return (a*b) / gcd(a,b);
}

int main()
{
    FILE *fp;
    fp = fopen("data.txt", "rb");

    struct route m_route = getRoute(fp);

    for (unsigned i=0; i<m_route.numDirs; i++)
    {
        printf("%c", m_route.dirs[i]);
    }
    printf("\n");

    size_t bufSize = 300;
    char *buf = malloc(sizeof(char)*bufSize);
    getline(&buf, &bufSize, fp);

    size_t *paths = malloc(sizeof(size_t));
    size_t pathSize = 1;
    size_t pathTop = 0;

    while (getline(&buf, &bufSize, fp) > 16)
    {
        char idxStr[3];
        char leftStr[3];
        char rightStr[3];

        for (unsigned i=0; i<3; i++)
        {
            idxStr[i] = buf[i];
            leftStr[i] = buf[i+7];
            rightStr[i] = buf[i+12];
        }
        
        size_t idx = getIdx(idxStr);

        if (idxStr[2] == 'A')
        {
            if (pathSize == pathTop)
            {
                pathSize *= 2;
                paths = realloc(paths, pathSize*sizeof(size_t));
            }
            paths[pathTop] = idx;
            pathTop++;
        }

        struct node newNode;
        newNode.left = getIdx(leftStr);
        newNode.right = getIdx(rightStr);

        newNode.end = idxStr[2] == 'Z';
        
        nodes[idx] = newNode;
    }

    uint64_t *steps = calloc(pathTop,sizeof(uint64_t));

    for (unsigned i=0; i<pathTop; i++)
    {
        size_t nextTurn = 0;
        while (!nodes[paths[i]].end)
        {
            steps[i]++;
            if (m_route.dirs[nextTurn] == 'L')
            {
                paths[i] = nodes[paths[i]].left;
            }
            else
            {
                paths[i] = nodes[paths[i]].right;
            }
            nextTurn++;
            if(nextTurn == m_route.numDirs)
            {
                nextTurn = 0;
            }
        }
    }

    for (unsigned i=0; i<pathTop; i++)
    {
        printf("steps: %lu\n", steps[i]);
    }

    uint64_t lcmOut=lcm(steps[0], steps[1]);

    for (unsigned i=2; i<pathTop; i++)
    {
        lcmOut = lcm(steps[i], lcmOut);
    }

    printf("lcm: %lu\n", lcmOut);
}