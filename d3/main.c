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

struct schematic initSchematic(FILE *fp)
{
    struct schematic out;

    char *buf;
    size_t bufSize = 100;

    buf = malloc(sizeof(char)*bufSize);

    out.lines = 1;
    out.lineLen = (size_t)(getline(&buf, &bufSize, fp)-1);

    printf("line: %s\n", buf);

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


int main()
{
    FILE *fp;

    fp = fopen("test.txt", "rb");

    struct schematic sch;

    sch = initSchematic(fp);

    printSchematic(sch);
}