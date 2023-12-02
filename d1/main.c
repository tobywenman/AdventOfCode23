#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

const char *templates[18] = {"1","2","3","4","5","6","7","8","9","one","two","three","four","five","six","seven","eight","nine"};
const size_t sizes[18] = {1,1,1,1,1,1,1,1,1,3,3,5,4,4,3,5,5,4};

unsigned checkLineForward(char* line, size_t size)
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
                if (pos[j] == sizes[j])
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
            else if (line[i] == templates[j][0])
            {
                pos[j] = 1;
                if (pos[j] == sizes[j])
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

unsigned checkLineBackward(char* line, size_t size)
{
    unsigned pos[18];
    for(unsigned i=0; i<18; i++)
        pos[i]=sizes[i]-1;

    for (unsigned i=size; i-->0;)
    {
        for (unsigned j=0; j<18; j++)
        {
            if (line[i] == templates[j][pos[j]])
            {
                if (pos[j] == 0)
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
                pos[j]--;
            }
            else if(line[i] == templates[j][sizes[j]-1])
            {
                pos[j] = sizes[j]-2;
            }
            else
            {
                pos[j] = sizes[j]-1;
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
    unsigned count = 0;
    unsigned test=1;
    while (true)
    {
        unsigned lineVal = 0;
        strLen = getline(&line, &bufSize, fp);
        if (strLen <= 0)
        {
            break;
        }

        lineVal += checkLineBackward(line, strLen);
        lineVal += 10*checkLineForward(line,strLen);
        line[strLen-1] = 0;
        test++;
        count += lineVal;
    } 
    free(line);
    printf("%u\n", count);
}