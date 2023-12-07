#include <stdbool.h>
#include <stdio.h>
#include "cle.h"

// Initialisation d'un entier de 128 bits
void init_uint128(uint128_t *key, uint32_t part0, uint32_t part1, uint32_t part2, uint32_t part3)
{
    key->parts[0] = part0;
    key->parts[1] = part1;
    key->parts[2] = part2;
    key->parts[3] = part3;
}

// Affichage d'un entier de 128 bits
void print_uint128(const uint128_t *key)
{
    printf("128-bit key representation:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("Part %d: %u\n", i, key->parts[i]);
    }
}

// Question 1.2 déterminer si cle1 est strictement inférieure à cle2
bool inf(const uint128_t cle1, const uint128_t cle2)
{
    for (int i = 3; i >= 0; i--)
    {
        if (cle1.parts[i] < cle2.parts[i])
        {
            return true;
        }
        else if (cle1.parts[i] > cle2.parts[i])
        {
            return false;
        }
    }
    return false; // 如果所有部分都相等，返回 false，因为 cle1 不小于 cle2
}

bool eg(const uint128_t cle1, const uint128_t cle2)
{
    for (int i = 3; i >= 0; i--)
    {
        if (cle1.parts[i] != cle2.parts[i])
        {
            return false;
        }
    }
    return true; // 如果所有部分都相等，返回 true，因为 cle1 等于 cle2
}


// process_file() :  lire le fichier et afficher les clés
bool process_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return false;
    }

    // Read file line by line
    char line[40];
    uint128_t key;

    while (fgets(line, sizeof(line), file))
    {

        if (sscanf(line, "%8x%8x%8x%8x",
                   &key.parts[3],
                   &key.parts[2],
                   &key.parts[1],
                   &key.parts[0]) == 4)
        {
            
            print_uint128(&key);
        }
        else
        {
            fprintf(stderr, "Parsing error: %s", line);
        }
    }

    fclose(file);
    return true;
}