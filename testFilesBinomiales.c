#include <stdio.h>
#include <stdlib.h>
#include "filesBinomiales.h"

void afficherNoeud(NoeudBinomial* noeud) {
    while (noeud != NULL) {
        printf("Key: %d, Degree: %d\n", noeud->cle, noeud->degre);
        afficherNoeud(noeud->enfant); // Recursively display children
        noeud = noeud->frere;         // Move to the next sibling
    }
}

void afficherFileBinomiale(FileBinomiale* heap) {
    printf("Binomial Heap:\n");
    afficherNoeud(heap->tete);
}

int main() {
    // Example usage of the functions 
    FileBinomiale* heap = initFileBinomiale();
    int elements[] = {10, 3, 6, 5, 7, 1};
    int n = sizeof(elements) / sizeof(elements[0]);

    heap = construire(heap, elements, n);
    printf("Heap after construction:\n");
    afficherFileBinomiale(heap);

    heap = ajouter(heap, 2);
    printf("Heap after adding an element:\n");
    afficherFileBinomiale(heap);

    NoeudBinomial* minNode = supprMin(heap);
    printf("Removed min: %d\n", minNode->cle);
    printf("Heap after removing the minimum element:\n");
    afficherFileBinomiale(heap);
    //free(minNode);

    //测试fusionner
    FileBinomiale* heap2 = initFileBinomiale();
    int elements2[] = {11, 4, 8, 9, 12, 2};
    int n2 = sizeof(elements2) / sizeof(elements2[0]);
    heap2 = construire(heap2, elements2, n2);
    heap = binomial_union(heap, heap2); 
    printf("Heap after binomial_union:\n");
    afficherFileBinomiale(heap);

    
    return 0;
}