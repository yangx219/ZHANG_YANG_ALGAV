#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "filesBinomiales.h"
#include "cle.h"

int main() {
    uint128_t test_Cles[6] = {
        {0xdf6943ba, 0x6d51464f, 0x6b021579, 0x33bdd9ad},
        {0xd192acf4, 0xc06fe7c7, 0xdf042f07, 0xd290bdd4},
        {0x2c15aed1, 0xa9eab933, 0x38d0348f, 0x12ef9a3b},
        {0x5f003a25, 0x87337655, 0xaf8a166b, 0xe8439a49},
        {0x1573c8d1, 0x56d03e63, 0x3c20c36f, 0x1b70862},
        
        {0xdf5d8018, 0xd0af5d1a, 0x979d449c, 0x91282bfc}       
    };
    // Initialize a new file binomial
    BinHeap H1 = Init();
    // construire un tas binomial à partir d'un tableau d'éléments
    
    int nbCles1 = sizeof(test_Cles) / sizeof(test_Cles[0]);
    H1 = fb_Construction(test_Cles, nbCles1);
    printf("\n*H1*\n");
    afficher(H1);

    // Supprimer l'élément minimum du tas binomial
    uint128_t min = fb_SupprMin(H1);
    printf("\nSupprMin___*H1*: \n");
    print_uint128(&min);
    afficher(H1);


    // Ajouter un élément au tas binomial
    ajout(H1, min);
    printf("\najout__*H1*\n");
    afficher(H1);

    // Initializer un autre file binomial
    BinHeap H2 = Init();
   uint128_t test_Cles2_AjoutIte[2] = {
        {0x1233be51,0x326a1b2b,0x9cc45caa,0x867d053b},
        {0xd0021c8d,0xa304b898,0xc87101ae,0xbbce50d1}    
    };  
    int nbCles2 = sizeof(test_Cles2_AjoutIte) / sizeof(test_Cles2_AjoutIte[0]);
    H2 = fb_Construction(test_Cles2_AjoutIte, nbCles2);
    printf("\n*H2*\n");
    afficher(H2);

    // Union de deux files binomiaux
    fb_union(H1, H2);
    printf("\nfb_union___(*H1* *H2*):\n");
    afficher(H1);

    return 0; 
}
