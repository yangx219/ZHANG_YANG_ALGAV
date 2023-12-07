#ifndef TASARBRE_H
#define TASARBRE_H

#include <stdio.h>
#include "cle.h"
#include "liste.h"

//  Structure de tas minimum avec un arbre binaire
typedef struct TasArbre
{
    uint128_t cle; // Clé de l'élément
    struct TasArbre *gauche;
    struct TasArbre *droite;
} TasArbre;

// Fonction pour créer un nouveau nœud
TasArbre *creerNoeud(uint128_t cle);

// fonction auxiliaire de printTasArbre
void printTasArbreBis(TasArbre *tas, char *indentation);

// Affiche le tas
void printTasArbre(TasArbre *arbre);

// Fonction pour chercher la profondeur maximale des sous arbre gauche sans prendre en compte les noeud vide
int profondeurMaxGauche(TasArbre *arbre);

// Fonction pour chercher la profondeur maximale des sous arbre droit sans prendre en compte les noeud vide
int profondeurMaxDroite(TasArbre *arbre);

// Fonction pour échanger deux éléments uint128_t
void echangerCle(TasArbre *a, TasArbre *b);

// Cherche la clé et échange avec son père si nécessaire
// rendre vrai si une échange est faite sinon faux
bool chercherEtEchanger(TasArbre **tas, uint128_t cle);

// Cherche la cle dans le tas et le remonte si il est inferieur à la cle de son père
void remonter(TasArbre **tas, uint128_t cle);

// ajoute la cle au noeud vide positionné le plus a droite et le plus bas possible
// si l'arbre n'est pas parfait
bool ajoutBis(TasArbre **tas, TasArbre *cle, int pMax, int p);

// Ajouter une clé dans un tas : 
// Mettre la clé à la position du noeud null le plus bas et plus à droite
// et remonte si la clé de son père est supérieur à lui
void ajout(TasArbre **tas, uint128_t cle);

// AjoutsIteratifs : Construit un tas avec une liste de clé 
void ajoutsIteratifs(TasArbre **tas, ListeCle *liste);

// ajoutsIteratifsAfficheEtape : Construit un tas avec une liste de clé en affichant chaque état
void ajoutsIteratifsAfficheEtape(TasArbre **tas, ListeCle *liste);

// Recherche le dernier nœud non NULL de profondeur pMax dans un tas binaire
TasArbre **chercherDernierNoeud(TasArbre **tas, int pMax, int p);

// supprime le min qui est la racine, le remplace par le noeud le plus bas et plus a droite
// le faire decendre si supérieur au fils
void suppMin(TasArbre **tas);

// Construit un tas avec une liste de clé
void construction(TasArbre **tas, ListeCle *liste);

// Construit un tas avec une liste de clé en affichant chaque état
void constructionAfficheEtape(TasArbre **tas, ListeCle *liste);

// Fonction pour fusionner deux tas et retourner le nouveau tas
TasArbre *unionTasArbre(TasArbre *tas1, TasArbre *tas2);

// Fonction pour fusionner deux tas et retourner le nouveau tas en affichant chaque étape
void *unionTasArbreAfficheEtape(TasArbre *tas1, TasArbre *tas2);



#endif