#ifndef ABR_H
#define ABR_H

#include <stdio.h>
#include "cle.h"
#include "liste.h"

typedef struct ABR{
    uint128_t cle;
    struct ABR *gauche;
    struct ABR *droite;
}ABR;

// Fonction pour créer un nouveau nœud
ABR *creerNoeud(uint128_t cle);

// Ajouter une clé dans un ABR : 
void ajout(ABR **arbre, uint128_t cle);

// Construit un ARB avec une liste de clé 
void construction(ABR **arbre, ListeCle *liste);

// fonction auxiliaire de printABR
void printABRBis(ABR *tas, char *indentation);

// Affiche le ABR
void printABR(ABR *arbre);

// Cherche si une clé est dans l'arbre
bool chercherCle(ABR *arbre, uint128_t cle);

#endif