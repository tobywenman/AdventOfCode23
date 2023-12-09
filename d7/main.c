#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

struct hand
{
    char cards[5];
    unsigned bid;
};

enum handType
{
    highCard=0,
    onePair=1,
    twoPair=2,
    threeKind=3,
    fullHouse=4,
    fourKind=5,
    fiveKind=6
};

unsigned getCardVal(char card)
{
    switch (card)
    {
    case 'J': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'T': return 10;
    case 'Q': return 12;
    case 'K': return 13;
    case 'A': return 14;
    default:  return 0;
    }
}

enum handType getHandType(const struct hand *in)
{
    unsigned occurances[5];

    for (unsigned i=0; i<5; i++)
    {
        occurances[i]=1;

        for (unsigned j=0; j<5; j++)
        {
            if (i != j)
            {
                if(in->cards[i] == in->cards[j])
                {
                    occurances[i]++;
                }
            }
        }
    }
    unsigned max = 0;
    unsigned pos = 0;

    for (unsigned i=0; i<5; i++)
    {
        if (occurances[i] > max && in->cards[i] != 'J')
        {
            max = occurances[i];
            pos = i;
        }
    }

    char test[5];

    if (max != 0)
    {
        for (unsigned i=0; i<5; i++)
        {
            if (in->cards[i] == 'J')
            {
                test[i] = in->cards[pos];
            }
            else
            {
                test[i] = in->cards[i];
            }
        }
    }
    else
    {
        for (unsigned i=0; i<5; i++)
        {
            test[i] = 'A';
        }
    }

    for (unsigned i=0; i<5; i++)
    {
        occurances[i]=1;

        for (unsigned j=0; j<5; j++)
        {
            if (i != j)
            {
                if(test[i] == test[j])
                {
                    occurances[i]++;
                }
            }
        }
    }

    unsigned three = 0;
    unsigned pairs = 0;

    for (unsigned i=0; i<5; i++)
    {
        switch (occurances[i])
        {
        case 5:
            return fiveKind;
        case 4:
            return fourKind;
        case 3:
            three++;
            break;
        case 2:
            pairs++;
            break;
        default:
            break;
        }
    }
    if (three)
    {
        if (pairs)
        {
            return fullHouse;
        }
        return threeKind;
    }
    else if (pairs == 4)
    {
        return twoPair;
    }
    else if (pairs == 2)
    {
        return onePair;
    }
    else
    {
        return highCard;
    }
}

int compareCards(const void * aVoid, const void * bVoid)
{
    struct hand *a = (struct hand *)aVoid;
    struct hand *b = (struct hand *)bVoid;

    enum handType aType = getHandType(a);
    enum handType bType = getHandType(b);

    if (aType > bType)
    {
        return 1;
    }
    else if (aType < bType)
    {
        return -1;
    }
    else
    {
        for (unsigned i=0; i<5; i++)
        {
            unsigned cardAVal = getCardVal(a->cards[i]);
            unsigned cardBVal = getCardVal(b->cards[i]);
            if (cardAVal > cardBVal)
            {
                return 1;
            }
            else if (cardAVal < cardBVal)
            {
                return -1;
            }
        }
    }
    return 0;
}

void printType(enum handType type)
{
    switch (type)
    {
    case highCard:
        printf("highcard\n");
        break;
    case onePair:
        printf("onepair\n");
        break;
    case twoPair:
        printf("twopair\n");
        break;
    case threeKind:
        printf("threeKind\n");
        break;
    case fullHouse:
        printf("fullHouse\n");
        break;
    case fourKind:
        printf("fourKind\n");
        break;
    case fiveKind:
        printf("fiveKind\n");
        break;
    default:
        break;
    }
}

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

    qsort(hands, numHands, sizeof(struct hand), compareCards);

    for (unsigned i=0; i<numHands; i++)
    {
        // printf("hand: %c%c%c%c%c, bid: %u\n", hands[i].cards[0], hands[i].cards[1], hands[i].cards[2], hands[i].cards[3], hands[i].cards[4], hands[i].bid);
        // printType(getHandType(&hands[i]));
        printf("%u\n", hands[i].bid);
    }
    
    uint64_t winnings = 0;
    for (unsigned i=0; i<numHands; i++)
    {
        winnings += hands[i].bid * (i+1);
    }
    printf("Part 1 : %lu\n", winnings);
}