#ifndef TAS_H
#define TAS_H

#include <stdio.h>
#include "cle.h"
#include <stdbool.h>

// Question 2.4

//  Structure de tas minimum avec un arbre binaire
typedef struct Noeud
{
    uint128_t cle;
    struct Noeud *gauche, *droite;
} Noeud;

// Structure de tas minimum avec un tableau
typedef struct
{
    uint128_t *cles; // Tableau des clés
    int taille;      // Nombre d'éléments dans le tas
    int capacite;    // Capacité du tableau
} Tas;

// afficher le tas
void print_tas(Tas *tas);
// initialiser le tas
Tas init_tas();
// Fonction pour échanger deux éléments uint128_t
void echanger(uint128_t *a, uint128_t *b);

// Fonction pour restaurer les propriétés d'un tas min après la suppression du min
void SiftDown(Tas *tas, int index);

// SupprMin : Supprime l'élément minimal du tas
void SupprMin(Tas *tas);

// Ajout : Ajoute un nouvel élément au tas
void Ajout(Tas *tas, uint128_t cle);

// AjoutsIteratifs : Construit un tas en ajoutant itérativement chaque clé
void AjoutsIteratifs(Tas *tas, uint128_t *cles, int nbCles);

// Question 2.5 Construction : Construit un tas à partir d'une liste de clés de manière efficace
Tas Construction(Tas *tas, uint128_t *cles, int nbCles);

// Question 2.6 La fonction Union prend en arguments deux tas ne partageant aucune clé commune,
// et construit un tas qui contient l’ensemble de toutes les clés.
Tas Union(Tas *tas1, Tas *tas2);

// tester si le tas est un tas min
bool isMinHeap(Tas *tas, int size);

#endif