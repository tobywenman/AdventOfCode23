#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct schematic
{
    char **data;
    size_t lineLen;
    size_t lines;
};

struct part
{
    unsigned partNum;
    size_t mins[2];
    size_t maxs[2];
};

struct schematic initSchematic(FILE *fp)
{
    struct schematic out;

    char *buf;
    size_t bufSize = 100;

    buf = malloc(sizeof(char)*bufSize);

    out.lines = 1;
    out.lineLen = (size_t)(getline(&buf, &bufSize, fp)-1);

    while (getline(&buf, &bufSize, fp) != -1)
    {
        out.lines++;
    }
    
    out.data = malloc(sizeof(char*)*out.lines);
    fseek(fp, 0, SEEK_SET);
    for (size_t i=0; i<out.lines; i++)
    {
        getline(&buf, &bufSize, fp);
        out.data[i] = malloc(sizeof(char)*out.lineLen);
        memcpy(out.data[i], buf, out.lineLen);
    }
    free(buf);
    return out;
}

void printSchematic(struct schematic in)
{
    printf("size: %zu %zu\n", in.lineLen, in.lines);
    for (size_t i=0; i<in.lines; i++)
    {
        for (size_t j=0; j<in.lineLen; j++)
        {
            putchar(in.data[i][j]);
        }
        putchar('\n');
    }
}

struct part* growStack(struct part* stack, size_t curSize)
{
    size_t newSize = curSize*2;

    return realloc(stack, newSize*sizeof(struct part));
}

struct part* getPartBounds(struct schematic in, size_t *numParts)
{
    size_t stackSize=1;
    size_t top=0;

    struct part* stack;

    stack = malloc(sizeof(struct part));

    char num[10];
    size_t numPos = 0;

    struct part newPart;

    for (size_t i=0; i<in.lines; i++)
    {
        for (size_t j=0; j<in.lineLen; j++)
        {
            if(in.data[i][j] >= 48 && in.data[i][j] <= 57)
            {
                if (numPos == 0)
                {
                    newPart.mins[0] = (i==0) ? 0 : i-1;
                    newPart.mins[1] = (j==0) ? 0 : j-1;
                }
                num[numPos] = in.data[i][j];
                numPos++;
            }
            else
            {
                if (numPos > 0)
                {
                    num[numPos] = '\0';
                    numPos = 0;
                    newPart.partNum = atoi(num);
                    newPart.maxs[0] = (i==in.lines-1) ? in.lines-1 : i+1;
                    newPart.maxs[1] = j;

                    if (top == stackSize)
                    {
                        stack = growStack(stack, stackSize);
                    }
                    stack[top] = newPart;
                    top++;
                }
            }
        }
        if (numPos > 0)
        {
            num[numPos] = '\0';
            numPos = 0;
            newPart.partNum = atoi(num);
            newPart.maxs[0] = (i==in.lines-1) ? in.lines-1 : i+1;
            newPart.maxs[1] = in.lineLen-1;

            if (top == stackSize)
            {
                stack = growStack(stack, stackSize);
            }
            stack[top] = newPart;
            top++;
        }
    }

    *numParts = top;
    return stack;
}

bool checkPart(struct part curPart, struct schematic sch)
{
    for (size_t i=curPart.mins[0]; i<curPart.maxs[0]+1; i++)
    {
        for (size_t j=curPart.mins[1]; j<curPart.maxs[1]+1; j++)
        {
            if((sch.data[i][j] < 48 || sch.data[i][j] > 57) && sch.data[i][j] != '.')
            {
                return true;
            }
        }
    }
    printf("not found: %u\n", curPart.partNum);
    return false;
}

unsigned sum(struct part* parts, size_t numParts, struct schematic sch)
{
    unsigned count = 0;
    for (size_t i=0; i<numParts; i++)
    {
        if(checkPart(parts[i], sch))
        {
            count += parts[i].partNum;
            printf("found part: %u\n", parts[i].partNum);
        }
    }
    return count;
}

unsigned checkGears(struct part* parts, size_t numParts, struct schematic sch)
{
    unsigned gears[sch.lines][sch.lineLen];
    unsigned gearVals[sch.lines][sch.lineLen];

    for (size_t i=0; i<sch.lines; i++)
    {
        for (size_t j=0; j<sch.lines; j++)
        {
            gears[i][j] = 0;
            gearVals[i][j] = 1;
        }
    }

    for (size_t partIdx=0; partIdx<numParts; partIdx++)
    {
        struct part curPart = parts[partIdx];
        for (size_t i=curPart.mins[0]; i<curPart.maxs[0]+1; i++)
        {
            for (size_t j=curPart.mins[1]; j<curPart.maxs[1]+1; j++)
            {
                if(sch.data[i][j]=='*')
                {
                    gears[i][j]++;
                    if (gears[i][j]<=2)
                    {
                        gearVals[i][j] *= curPart.partNum;
                    }
                }
            }
        }

    }
    unsigned count=0;

    for (size_t i=0; i<sch.lines; i++)
    {
        for (size_t j=0; j<sch.lines; j++)
        {
            if(gears[i][j]==2)
            {
                printf("gear found: %zu, %zu\n", i, j);
                count += gearVals[i][j];
            }
        }
    }
    return count;
}

int main()
{
    FILE *fp;

    fp = fopen("data.txt", "rb");

    struct schematic sch;

    sch = initSchematic(fp);

    printSchematic(sch);

    size_t numParts;
    struct part* stack;
    stack = getPartBounds(sch, &numParts);

    printf("sum: %u\n", checkGears(stack, numParts, sch));
}