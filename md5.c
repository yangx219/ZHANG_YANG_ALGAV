#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cle.h"
#include "md5.h"

// Définition des constantes r
const uint32_t r[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
                        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

// Définition des constantes k
const uint32_t k[64] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

// Fonction de rotation à gauche
uint32_t leftrotate(uint32_t x, int c)
{
    return (x << c) | (x >> (32 - c));
}

void afficherTableauUint8(uint8_t *tableau, size_t taille)
{
    for (size_t i = 0; i < taille; i++)
    {
        printf("%02x ", tableau[i]);
    }
    printf("\n");
}

void convertirEnUint8(const char *chaine, uint8_t *resultat, size_t longueurChaine)
{
    for (size_t i = 0; i < longueurChaine; i++)
    {
        resultat[i] = (uint8_t)chaine[i];
    }
}

// Modifiez cette fonction pour prendre en compte la longueur réelle du message
void paddingMessage(uint8_t *message, size_t originalLength)
{
    size_t temp = originalLength;

    // Ajouter le bit "1" au message
    message[temp++] = 0x80; // 0x80 en hexadécimal représente le bit "1" suivi de zéros

    while ((temp * 8) % 512 != 448)
        message[temp++] = 0x00; // 0x00 en hexadécimal représente le bit "0"

    // Ajouter la taille du message initial codée en 64 bits little-endian
    uint64_t bitLength = originalLength * 8;
    for (size_t i = 0; i < 8; i++)
        message[temp++] = (uint8_t)(bitLength >> (i * 8)); // Little-endian
}

// découpe le message en un ensemble de bloc de 512 bits
void decoupeMessage(uint8_t *paddedMessage, size_t nbBloc, size_t TaillepaddedMessage, uint8_t **blocs)
{
    for (int i = 0; i < nbBloc; i++)
    {
        for (int j = 0; j < TaillepaddedMessage; j += 64)
        {
            blocs[i] = (uint8_t *)malloc(64 * sizeof(uint8_t));
            if (blocs[i] == NULL)
            {
                // Gestion de l'erreur d'allocation
                perror("Erreur d'allocation de mémoire");
                exit(EXIT_FAILURE);
            }
            memcpy(blocs[i], paddedMessage + j, 64);
            printf("Bloc %d : ", i);
            afficherTableauUint8(blocs[i], 64);
        }
    }
}

// Affiche les valeurs de w
void afficherW(uint32_t *w)
{
    for (int i = 0; i < 16; i++)
    {
        printf("w[%2d] = %08x\n", i, w[i]);
    }
}

void bouclePrincipal(uint8_t **blocs, size_t nombreBlocs, uint128_t *res)
{

    // Initialisation des valeurs de hachage
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xefcdab89;
    uint32_t h2 = 0x98badcfe;
    uint32_t h3 = 0x10325476;

    for (int b = 0; b < nombreBlocs; b++)
    {

        // Convertir le bloc en tableau de mots de 32 bits en little-endian
        uint32_t w[16];
        for (int j = 0; j < 16; j++)
        {
            w[j] = (uint32_t)blocs[b][j * 4] |
                   (uint32_t)blocs[b][j * 4 + 1] << 8 |
                   (uint32_t)blocs[b][j * 4 + 2] << 16 |
                   (uint32_t)blocs[b][j * 4 + 3] << 24;
        }
        printf("Mots :\n");
        afficherW(w);

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t f, g, temp;

        for (int i = 0; i < 64; i++)
        {
            if (i < 16)
            {
                f = (b & c) | (~b & d);
                g = i;
            }
            else if (i < 32)
            {
                f = (d & b) | (~d & c);
                g = (5 * i + 1) % 16;
            }
            else if (i < 48)
            {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            }
            else
            {
                f = c ^ (b | ~d);
                g = (7 * i) % 16;
            }
            temp = d;
            d = c;
            c = b;
            b = b + leftrotate(a + f + k[i] + w[g], r[i]);
            a = temp;
        }
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
    }

    res->parts[0] = h0;
    res->parts[1] = h1;
    res->parts[2] = h2;
    res->parts[3] = h3;
}

void md5(const char *message, uint128_t *res)
{
    size_t longueurMessage = strlen(message);
    // uint8_t convertieMessage[longueurMessage];
    uint8_t convertieMessage[longueurMessage + 1];

    // message convertie
    convertirEnUint8(message, convertieMessage, longueurMessage);
    printf("Convertion : ");
    afficherTableauUint8(convertieMessage, longueurMessage);

    // remplissage
    size_t taillePaddedMessage = ((longueurMessage + 8) / 64 + 1) * 64;
    uint8_t paddedMessage[taillePaddedMessage];
    memcpy(paddedMessage, convertieMessage, longueurMessage);
    paddingMessage(paddedMessage, longueurMessage);
    printf("Padding : ");
    afficherTableauUint8(paddedMessage, taillePaddedMessage);

    // découpage
    size_t nbBloc = taillePaddedMessage / 64;
    uint8_t *blocs[nbBloc];
    printf("Decoupage :\n");
    decoupeMessage(paddedMessage, nbBloc, taillePaddedMessage, blocs);
    // afficherTableauUint8(blocs[0], 64);

    // boucle principal
    bouclePrincipal(blocs, nbBloc, res);

    // libère la mémoire
    for (size_t i = 0; i < nbBloc; i++)
        free(blocs[i]);
}
/*
int main()
{
    const char *message = "Hello World!";
    uint128_t *res = (uint128_t *)malloc(sizeof(uint128_t));
    if (res == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        return 1;
    }
    md5(message, res);
    printf("md5 : %08x%08x%08x%08x\n", res->parts[0], res->parts[1], res->parts[2], res->parts[3]);

    free(res);
    return 0;
}
*/
