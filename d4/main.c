#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


struct cards
{
    unsigned **winning;
    unsigned **nums;
    size_t numCards;
    size_t winNums;
    size_t haveNums;
};

struct cards initCard(FILE *fp, size_t wins, size_t nums, size_t numCards)
{
    struct cards out;

    out.numCards = numCards;
    out.haveNums = nums;
    out.winNums = wins;

    out.winning = malloc(sizeof(unsigned*)*numCards);
    for (size_t i=0; i<numCards; i++)
    {
        out.winning[i] = malloc(sizeof(unsigned)*wins);
    }

    out.nums = malloc(sizeof(unsigned*)*numCards);
    for (size_t i=0; i<numCards; i++)
    {
        out.nums[i] = malloc(sizeof(unsigned)*nums);
    } 

    char *buf;
    size_t bufSize = 100;

    buf = malloc(sizeof(char)*bufSize);

    char numStr[4];
    numStr[3] = '\0';

    for (size_t i=0; i<numCards; i++)
    {
        getline(&buf, &bufSize, fp);
        size_t strPos = strchr(buf, ':')-buf+1;
        printf("strPos: %zu\n", strPos);
        for (size_t j=0; j<wins; j++)
        {
            for (size_t k=0; k<3; k++)
            {
                numStr[k] = buf[strPos];
                strPos++;
            }
            out.winning[i][j] = atoi(numStr);
        }
        strPos += 2;

        for (size_t j=0; j<nums; j++)
        {
            for (size_t k=0; k<3; k++)
            {
                numStr[k] = buf[strPos];
                strPos++;
            }
            out.nums[i][j] = atoi(numStr);
        }
    }

    free(buf);
    return out;
}

void printCards(struct cards in)
{
    for (size_t i=0; i<in.numCards; i++)
    {
        for (size_t j=0; j<in.winNums; j++)
        {
            printf("%02u ", in.winning[i][j]);
        }
            printf("| ");
        for (size_t j=0; j<in.haveNums; j++)
        {
            printf("%02u ", in.nums[i][j]);
        }
        printf("\n");
    }
}

unsigned countScore(struct cards in)
{
    unsigned out = 0;
    for (size_t i=0; i<in.numCards; i++)
    {
        unsigned cardScore = 0;
        for (size_t j=0; j<in.haveNums; j++)
        {
            for (size_t k=0; k<in.winNums; k++)
            {
                if (in.nums[i][j] == in.winning[i][k])
                {
                    cardScore = (cardScore==0) ? 1 : cardScore*2;
                    break;
                }
            }
        }
        out += cardScore;
    }
    return out;
}

unsigned numCards(struct cards in)
{
    unsigned copies[in.numCards];

    for (size_t i=0; i<in.numCards; i++)
    {
        copies[i] = 1;
    }

    for (size_t i=0; i<in.numCards; i++)
    {
        unsigned wins=0;
        for (size_t j=0; j<in.haveNums; j++)
        {
            for (size_t k=0; k<in.winNums; k++)
            {
                if (in.nums[i][j] == in.winning[i][k])
                {
                    wins++;
                    break;
                }
            }
        }

        for (unsigned j=1; j<wins+1; j++)
        {
            if (i+j<in.numCards)
            {
                copies[i+j] += copies[i];
            }
        }
    }

    unsigned out=0;
    for (size_t i=0; i<in.numCards; i++)
    {
        out += copies[i];
    }
    return out;
}

int main()
{
    FILE *fp;

    fp = fopen("data.txt", "rb");

    struct cards _cards = initCard(fp, 10, 25, 212);

    printCards(_cards);

    printf("score: %u\n", numCards(_cards));
}