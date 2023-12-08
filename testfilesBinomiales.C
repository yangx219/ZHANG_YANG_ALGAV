#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "filesBinomiales.h"
int main() {
    // Initialize a new file binomial
    BinHeap H1 = Init();
    // construire un tas binomial à partir d'un tableau d'éléments
    int elementsToAdd1[] = {3, 5, 2, 7, 6, 1, 8};
    int n = sizeof(elementsToAdd1) / sizeof(elementsToAdd1[0]);
    H1 = fb_Construction(elementsToAdd1, n);
    printf("*H1*\n");
    afficher(H1);

    // Supprimer l'élément minimum du tas binomial
    int min = SupprMin(H1);
    printf("SupprMin___*H1*: %d\n", min);
    afficher(H1);
    // Ajouter un élément au tas binomial
    ajout(H1, 4);
    printf("ajout__4*H1*\n");
    afficher(H1);

    // Initializer un autre file binomial
    BinHeap H2 = Init();
    int elementsToAdd2[] = {9,10,11,12};
    int n2 = sizeof(elementsToAdd2) / sizeof(elementsToAdd2[0]);
    H2 = fb_Construction(elementsToAdd2, n2);
    printf("*H2*\n");
    afficher(H2);

    // Union de deux files binomiaux
    fb_union(H1, H2);
    printf("fb_union___(*H1* *H2*):\n");
    afficher(H1);

    return 0; 
}
