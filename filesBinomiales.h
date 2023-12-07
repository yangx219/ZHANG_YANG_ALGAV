#include <stdio.h>
#include <stdlib.h>


typedef struct NoeudBinomial {
    int cle;
    int degre;
    struct NoeudBinomial *parent;
    struct NoeudBinomial *enfant;
    struct NoeudBinomial *frere;
} NoeudBinomial;

typedef struct FileBinomiale {
    NoeudBinomial *tete; // Pointer to the head of the heap
} FileBinomiale;


// Créer un nouveau noeud binomial
NoeudBinomial* creerNoeud(int cle);

// Initialisation d'une file binomiale
FileBinomiale* initFileBinomiale();

// Créer une file binomiale à partir d'un noeud binomial
// Lier deux noeuds binomiaux
void lierNoeudsBinomiaux(NoeudBinomial* y, NoeudBinomial* z);

// Fusionner les bases binomiales
FileBinomiale* fusionner(FileBinomiale* H1, FileBinomiale* H2);

// Insérer un noeud dans une file binomiale
FileBinomiale* ajouter(FileBinomiale* H, NoeudBinomial* x);

// Supprimer le noeud minimum d'une file binomiale
NoeudBinomial* supprMin(FileBinomiale* H);

// Rechercher un noeud dans une file binomiale
NoeudBinomial* rechercher(FileBinomiale* H, int cle);

// Supprimer un noeud dans une file binomiale
FileBinomiale* supprimer(FileBinomiale* H, int cle);

// construire une file binomiale à partir d'un tableau d'éléments
FileBinomiale* construire(FileBinomiale* H, int* elements, int n);

