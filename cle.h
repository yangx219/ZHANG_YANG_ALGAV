#ifndef CLE_H
#define CLE_H

#include <stdint.h>
#include <stdbool.h>

// Question 1.1 Définition d'un type pour représenter un entier de 128 bits
typedef struct
{
    // Tableau de 4 entiers de 32 bits
    uint32_t parts[4];
} uint128_t;

// Initialisation d'un entier de 128 bits
void init_uint128(uint128_t *key, uint32_t part0, uint32_t part1, uint32_t part2, uint32_t part3);

// Affichage d'un entier de 128 bits
void print_uint128(const uint128_t *key);

// Question 1.2 déterminer si cle1 est strictement inférieure à cle2
bool inf(const uint128_t cle1, const uint128_t cle2);

// Question 1.3 déterminer si cle1 est égale à cle2
bool eg(const uint128_t cle1, const uint128_t cle2);

// process_file() :  lire le fichier et afficher les clés
bool process_file(const char *filename);

#endif