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

        struct node newNode;
        newNode.left = getIdx(leftStr);
        newNode.right = getIdx(rightStr);
        
        nodes[idx] = newNode;
    }

    size_t nextNode = 0;
    size_t nextTurn = 0;

    uint64_t steps = 0;

    while (nextNode != 17575)
    {
        steps++;
        if (m_route.dirs[nextTurn] == 'L')
        {
            printf("left\n");
            nextNode = nodes[nextNode].left;
        }
        else
        {
            printf("right\n");
            nextNode = nodes[nextNode].right;
        }
        nextTurn++;
        
        if(nextTurn == m_route.numDirs)
        {
            nextTurn = 0;
        }
    }

    printf("steps: %lu\n", steps);
}