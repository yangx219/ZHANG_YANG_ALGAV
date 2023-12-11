#include <stdbool.h>
#include <stdio.h>
#include "cle.h"

// Initialisation d'un entier de 128 bits
void init_uint128(uint128_t *cle, uint32_t part0, uint32_t part1, uint32_t part2, uint32_t part3)
{
    cle->parts[0] = part0;
    cle->parts[1] = part1;
    cle->parts[2] = part2;
    cle->parts[3] = part3;
}

// Affichage d'un entier de 128 bits
void print_uint128(const uint128_t *cle)
{   
    
    for (int i =0; i < 4; i++)
    {
        printf("%08x ", cle->parts[i]);
    }
}

void print_uint128_bis(const uint128_t *cle)
{
    for (int i = 0; i < 4; i++)
    {
        printf("%u ", cle->parts[i]);
    }
}

// Question 1.2 déterminer si cle1 est strictement inférieure à cle2
bool inf(const uint128_t cle1, const uint128_t cle2)
{
    for (int i = 0; i < 4; i++)
    {
        if (cle1.parts[i] < cle2.parts[i])
        {
            // printf("cle1 is less than cle2\n");
            return true;
        }
        else if (cle1.parts[i] > cle2.parts[i])
        {
            // printf("cle1 is greater than cle2\n");
            return false;
        }
    }
    return false;
}

// Question 1.3 déterminer si cle1 est égale à cle2
bool eg(const uint128_t cle1, const uint128_t cle2)
{
    for (int i = 0; i < 4; i++)
    {
        if (cle1.parts[i] != cle2.parts[i])
        {
            // printf("cle1 is not equal to cle2\n");
            return false;
        }
    }
    // printf("cle1 is equal to cle2\n");
    return true;
}
