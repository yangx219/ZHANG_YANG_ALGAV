#ifndef LISTE_H
#define LISTE_H

#include "cle.h"
#include <stdbool.h>

typedef struct Element Element;
struct Element
{
    uint128_t cle;
    Element *suivant;
};

typedef struct ListeCle ListeCle;
struct ListeCle
{
    Element *premier;
};


// Initialise une liste vide
ListeCle *initialisation();

// Insert une cle au début de la liste
void insertion(ListeCle *liste, uint128_t cle);

// Affichage de la liste
void printListe(ListeCle *liste);



#endif