#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

uint64_t time = 58996469;
uint64_t dist = 478223210191071;

int main()
{
    unsigned wins=0;
    for (uint64_t i=0; i<time+1; i++)
    {
        uint64_t newDist =  (time - i)*i;
        if(newDist > dist)
        {
            wins++;
        }
    }

    printf("wins: %u\n", wins);
}