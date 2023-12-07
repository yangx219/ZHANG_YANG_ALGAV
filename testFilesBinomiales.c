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
    // Example usage of the functions you've implemented
    FileBinomiale* heap = initFileBinomiale();
    int elements[] = {10, 3, 6, 5, 7, 1};
    int n = sizeof(elements) / sizeof(elements[0]);

    construire(heap, elements, n);
    printf("Heap after construction:\n");
    afficherFileBinomiale(heap);

    ajouter(heap, creerNoeud(2));
    printf("Heap after adding an element:\n");
    afficherFileBinomiale(heap);

    NoeudBinomial* minNode = supprMin(heap);
    printf("Removed min: %d\n", minNode->cle);
    printf("Heap after removing the minimum element:\n");
    afficherFileBinomiale(heap);
    free(minNode);



    // Clean up
    // Make sure to write a proper cleanup function to free all the allocated memory!
    
    return 0;
}