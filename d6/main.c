#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

struct race
{
    uint64_t time;
    uint64_t dist;
};

struct races
{
    struct race* data;
    size_t stackTop;
    size_t stackSize;
};

struct races getRaces(FILE *fp)
{
    struct races out;

    out.stackSize = 1;
    out.stackTop = 0;

    out.data = malloc(sizeof(struct race));

    char *buf;
    size_t bufSize = 300;

    buf = malloc(sizeof(char)*bufSize);

    int lineLen = getline(&buf, &bufSize, fp);    

    char numStr[20];
    unsigned numPos=0;

    for (int i=0; i<lineLen; i++)
    {
        if (buf[i] >= '0' && buf[i] <= '9')
        {
            numStr[numPos] = buf[i];
            numPos++;
        }
        else if (numPos != 0)
        {
            numStr[numPos] = '\0';
            numPos = 0;
            if (out.stackSize == out.stackTop)
            {
                out.stackSize *= 2;
                out.data = realloc(out.data, sizeof(struct race)*out.stackSize);
            }
            out.data[out.stackTop].time = strtoull(numStr, NULL, 10); 
            out.stackTop++;
        }
    }

    lineLen = getline(&buf, &bufSize, fp); 

    size_t race=0;

    for (int i=0; i<lineLen; i++)
    {
        if (buf[i] >= '0' && buf[i] <= '9')
        {
            numStr[numPos] = buf[i];
            numPos++;
        }
        else if (numPos != 0)
        {
            numStr[numPos] = '\0';
            numPos = 0;
            out.data[race].dist = strtoull(numStr, NULL, 10); 
            race++;
        }
    }
    return out;
}

int main()
{
    FILE *fp;

    fp = fopen("data.txt", "rb");

    struct races _races = getRaces(fp);

    uint64_t count=1;

    for (unsigned i=0; i<_races.stackTop; i++)
    {
        printf("time: %lu, dist: %lu\n", _races.data[i].time, _races.data[i].dist);

        unsigned wins=0;
        for (uint64_t j=0; j<_races.data[i].time+1; j++)
        {
            uint64_t dist =  (_races.data[i].time - j)*j;
            if(dist > _races.data[i].dist)
            {
                wins++;
            }
        }
        count *= wins;
    }

    printf("count: %lu\n", count);
}