#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

const char *templates[18] = {"1","2","3","4","5","6","7","8","9","one","two","three","four","five","six","seven","eight","nine"};

unsigned checkLine(char* line, size_t size)
{
    unsigned pos[18];
    for(unsigned i=0; i<18; i++)
        pos[i]=0;

    for (unsigned i=0; i<size; i++)
    {
        for (unsigned j=0; j<18; j++)
        {
            if (line[i] == templates[j][pos[j]])
            {
                pos[j]++;
                if (templates[j][pos[j]] == 0)
                {
                    if(j<=8)
                    {
                        return j+1;
                    }
                    else
                    {
                        return j-8;
                    }
                }
            }
            else
            {
                pos[j] = 0;
            }
        }
    }
}

int main()
{
    FILE *fp;

    fp = fopen("data.txt", "rb");

    char *line;
    size_t bufSize = 100;

    line = malloc(bufSize);

    int strLen;
    unsigned count = 1;
    while (true)
    {
        strLen = getline(&line, &bufSize, fp);
        if (strLen <= 0)
        {
            break;
        }

        printf("%u val: %u\n", count, checkLine(line, strLen));
        count++;
    } 
}