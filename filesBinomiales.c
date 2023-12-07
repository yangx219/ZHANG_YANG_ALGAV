#include <stdio.h>
#include <stdlib.h>
#include "filesBinomiales.h"
#include "tas.h"

// Création d'un noeud binomial
NoeudBinomial* creerNoeud(int cle) {
    NoeudBinomial* nouveau = (NoeudBinomial*)malloc(sizeof(NoeudBinomial));
    if (!nouveau) {
        return NULL;
    }
    nouveau->cle = cle;
    nouveau->degre = 0;
    nouveau->parent = NULL;
    nouveau->enfant = NULL;
    nouveau->frere = NULL;
    return nouveau;
}   

// Initialisation d'une file binomiale
FileBinomiale* initFileBinomiale() {
    FileBinomiale* Tas = (FileBinomiale*)malloc(sizeof(FileBinomiale));
    if (!Tas) { 
        return NULL;
    }
    Tas->tete = NULL;
    return Tas;
}

// Lier deux noeuds binomiaux
void lierNoeudsBinomiaux(NoeudBinomial* y, NoeudBinomial* z) {
    y->parent = z;
    y->frere = z->enfant;
    z->enfant = y;
    z->degre++;
}

// Fusionner les bases binomiales
FileBinomiale* fusionner(FileBinomiale* H1, FileBinomiale* H2) {
    // vérifier si les deux tas sont vides
    if (!H1 && !H2) return NULL;
    if (!H1->tete) {
        free(H1); 
        return H2;
    }
    if (!H2->tete) {
        free(H2);  
        return H1;
    }

    FileBinomiale* newHeap = initFileBinomiale();
    if (!newHeap) return NULL; 

    NoeudBinomial **pos = &(newHeap->tete);
    NoeudBinomial *h1Curr = H1->tete;
    NoeudBinomial *h2Curr = H2->tete;

    while (h1Curr && h2Curr) {
        if (h1Curr->degre <= h2Curr->degre) {
            *pos = h1Curr;
            h1Curr = h1Curr->frere;
        } else {
            *pos = h2Curr;
            h2Curr = h2Curr->frere;
        }
        pos = &((*pos)->frere);
    }

    while (h1Curr) {
        *pos = h1Curr;
        h1Curr = h1Curr->frere;
        pos = &((*pos)->frere);
    }

    while (h2Curr) {
        *pos = h2Curr;
        h2Curr = h2Curr->frere;
        pos = &((*pos)->frere);
    }
    return newHeap;
}

// Supprimer le minimum
NoeudBinomial* supprMin(FileBinomiale* H) {
    if (!H || !H->tete) return NULL;

    NoeudBinomial *minPrev = NULL;
    NoeudBinomial *min = H->tete;
    NoeudBinomial *current = H->tete;

    while (current->frere) {
        if (current->frere->cle < min->cle) {
            min = current->frere;
            minPrev = current;
        }
        current = current->frere;
    }

    if (minPrev) {
        minPrev->frere = min->frere;
    } else {
        H->tete = min->frere;
    }

    FileBinomiale* H1 = initFileBinomiale();
    if (!H1) return NULL; 

    NoeudBinomial *child = min->enfant;
    NoeudBinomial *temp;
    while (child) {
        temp = child->frere;
        child->frere = H1->tete;
        child->parent = NULL;
        H1->tete = child;
        child = temp;
    }

    FileBinomiale* newHeap = fusionner(H, H1);
    if (!newHeap) return NULL; 

    H->tete = newHeap->tete;
    

    printf("Removing min node with key: %d\n", min->cle);

    min->enfant = NULL;
    return min;
}

// Ajouter un élément dans la file
FileBinomiale* ajouter(FileBinomiale* H, NoeudBinomial* x) {
    FileBinomiale* H1 = initFileBinomiale();
    if (!H1) return NULL; 

    x->parent = NULL;
    x->enfant = NULL;
    x->frere = NULL;
    x->degre = 0;
    H1->tete = x;

    FileBinomiale* newHeap = fusionner(H, H1);
    if (!newHeap) return NULL; 

    return newHeap;
}

// Construire la file par ajouts itératifs
FileBinomiale* construire(FileBinomiale* H, int* elements, int n) {
    for (int i = 0; i < n; i++) {
        NoeudBinomial* x = creerNoeud(elements[i]);
        if (!x) return NULL; 
        H = ajouter(H, x);
    }
    return H;
}
