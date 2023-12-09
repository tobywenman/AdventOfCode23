#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

struct seeds
{
    uint64_t *seedVals;
    size_t numSeeds;
    size_t arraySize;
};

struct range
{
    uint64_t dst;
    uint64_t src;
    uint64_t range;
};


struct map
{
    struct range *ranges;
    size_t rangeSize;
    size_t rangeTop;
};

struct seeds getSeeds(FILE *fp)
{
    char *buf;
    size_t bufSize = 300;

    buf = malloc(sizeof(char)*bufSize);

    int lineLen = getline(&buf, &bufSize, fp);

    char num[15];
    size_t numPos=0;

    struct seeds out;
    out.numSeeds = 0;
    out.arraySize = 1;

    out.seedVals = malloc(sizeof(uint64_t));

    for (int i=0; i<lineLen; i++)
    {
        if (buf[i] >= '0' && buf[i] <= '9')
        {
            num[numPos] = buf[i];
            numPos++;
        }
        else
        {
            if (numPos != 0)
            {
                num[numPos] = '\0';
                if (out.numSeeds == out.arraySize)
                {
                    out.arraySize *= 2;
                    out.seedVals = realloc(out.seedVals, sizeof(uint64_t)*out.arraySize);
                }

                out.seedVals[out.numSeeds] = strtoull(num, NULL, 10);
                out.numSeeds++;
                numPos = 0;
            }
        }
    }
    getline(&buf, &bufSize, fp);
    free(buf);
    return out;
}

struct seeds getSeedsRange(FILE *fp)
{
    char *buf;
    size_t bufSize = 300;

    buf = malloc(sizeof(char)*bufSize);

    int lineLen = getline(&buf, &bufSize, fp);

    char num[15];
    size_t numPos=0;

    struct seeds out;
    out.numSeeds = 0;
    out.arraySize = 1;

    out.seedVals = malloc(sizeof(uint64_t));

    uint64_t seedPair[2];
    unsigned seedPairPos=0;

    for (int i=0; i<lineLen; i++)
    {
        if (buf[i] >= '0' && buf[i] <= '9')
        {
            num[numPos] = buf[i];
            numPos++;
        }
        else
        {
            if (numPos != 0)
            {
                num[numPos] = '\0';
                seedPair[seedPairPos] = strtoull(num, NULL, 10);
                seedPairPos++;
                if(seedPairPos == 2)
                {
                    for (uint64_t i=0; i<seedPairPos)
                }
            }
        }
    }
    getline(&buf, &bufSize, fp);
    free(buf);
    return out;
}

struct map getMap(FILE *fp)
{
    char *buf;
    size_t bufSize = 100;

    buf = malloc(sizeof(char)*bufSize);

    getline(&buf, &bufSize, fp); //dump title


    struct map out;

    out.rangeSize = 1;
    out.rangeTop = 0;

    out.ranges = malloc(sizeof(struct range));

    while (true)
    {
        int lineLen = getline(&buf, &bufSize, fp);

        if (lineLen <= 1)
        {
            break;
        }

        if (out.rangeSize == out.rangeTop)
        {
            out.rangeSize *= 2;
            out.ranges = realloc(out.ranges, sizeof(struct range)*out.rangeSize);
        }

        char num[15];
        size_t numPos=0;
        unsigned type=0;

        struct range curRange;

        for (int i=0; i<lineLen; i++)
        {
            if (buf[i] >= '0' && buf[i] <= '9')
            {
                num[numPos] = buf[i];
                numPos++;
            }
            else
            {
                num[numPos] = '\0';
                numPos = 0;
                switch (type)
                {
                case 0:
                    curRange.dst = strtoull(num, NULL, 10);
                    break;
                case 1:
                    curRange.src = strtoull(num, NULL, 10);
                    break;
                case 2:
                    curRange.range = strtoull(num, NULL, 10);
                    break;
                default:
                    break;
                }
                type++;
            }
        }   
        out.ranges[out.rangeTop] = curRange;
        out.rangeTop++;
    }
    free(buf);
    return out;
}

void printLocations(struct seeds _seeds, struct map *maps, size_t numMaps)
{
    uint64_t lowestVal = (uint64_t)(-1);
    for (size_t i=0; i<_seeds.numSeeds; i++)
    {
        uint64_t newVal = _seeds.seedVals[i];
        for (size_t j=0; j<numMaps; j++)
        {
            for (size_t k=0; k<maps[j].rangeTop; k++)
            {
                if (newVal >= maps[j].ranges[k].src && newVal <= maps[j].ranges[k].src+maps[j].ranges[k].range)
                {
                    newVal = maps[j].ranges[k].dst + (newVal - maps[j].ranges[k].src);
                    break;
                }
            }
        }
        if (newVal < lowestVal)
        {
            lowestVal = newVal;
        }
        printf("seed: %zu, %llu\n", i, newVal);
    }
    printf("lowestVal: %llu\n", lowestVal);
}

int main()
{
    struct seeds _seeds;

    FILE *fp;
    fp = fopen("data.txt", "rb");

    _seeds = getSeeds(fp);

    for (unsigned i=0; i<_seeds.numSeeds; i++)
    {
        printf("seedVal: %u\n", _seeds.seedVals[i]);
    }

    struct map *maps;
    size_t mapsTop = 0;
    size_t mapsSize = 1;
    

    maps = malloc(sizeof(struct map));

    char *buf;
    size_t bufSize = 100;
    buf = malloc(sizeof(char)*bufSize);
    while (true)
    {
        if (mapsTop == mapsSize)
        {
            mapsSize *= 2;
            maps = realloc(maps, mapsSize*sizeof(struct map));
        }

        maps[mapsTop] = getMap(fp);
        mapsTop++;

        long before = ftell(fp);

        if (getline(&buf, &bufSize, fp) <= 1)
        {
            break;
        }
        fseek(fp, before, SEEK_SET);
        
    }
    for (unsigned j=0; j<mapsTop; j++)
    {
        struct map testMap = maps[j];
        for (unsigned i=0; i<testMap.rangeTop; i++)
        {
            printf("dst: %llu, src: %llu, range: %llu\n", testMap.ranges[i].dst, testMap.ranges[i].src, testMap.ranges[i].range);
        }
        printf("\n");
    }

    printLocations(_seeds, maps, mapsTop);
}