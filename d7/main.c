#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

struct hand
{
    char cards[5];
    unsigned bid;
};


int main()
{
    FILE *fp;
    fp = fopen("data.txt", "rb");

    unsigned numHands = 0;
    char nextChar;

    while(!feof(fp))
    {
        nextChar = fgetc(fp);
        if(nextChar == '\n')
        {
            numHands++;
        }
    }
    fseek(fp, 0, SEEK_SET);

    struct hand *hands;

    hands = malloc(sizeof(struct hand)*numHands);

    char *buf;
    size_t bufSize = 300;

    buf = malloc(sizeof(char)*bufSize);


    for (unsigned i=0; i<numHands; i++)
    {
        int lineLen = getline(&buf, &bufSize, fp);    
        for (unsigned j=0; j<5; j++)
        {
            hands[i].cards[j] = buf[j];
        }

        char numStr[10];
        unsigned numPos = 0;

        for (unsigned j=6; j<lineLen; j++)
        {
            numStr[numPos] = buf[j];
            numPos++;
        }
        numStr[numPos] = '\0';

        hands[i].bid = strtoul(numStr, NULL, 10);
    }

    for (unsigned i=0; i<numHands; i++)
    {
        printf("hand: %c%c%c%c%c, bid: %u\n", hands[i].cards[0], hands[i].cards[1], hands[i].cards[2], hands[i].cards[3], hands[i].cards[4], hands[i].bid);
    }
}