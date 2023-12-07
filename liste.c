#include "liste.h"
#include "cle.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Initialise une liste vide
ListeCle *initialisation()
{
    ListeCle *liste = malloc(sizeof(ListeCle));
    if (liste == NULL)
        exit(EXIT_FAILURE);
    liste->premier = NULL;

    return liste;
}

// Insert une cle au début de la liste
void insertion(ListeCle *liste, uint128_t cle)
{
    /* Création du nouvel élément */
    Element *nouveau = malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    nouveau->cle = cle;

    /* Insertion de l'élément au début de la liste */
    nouveau->suivant = liste->premier;
    liste->premier = nouveau;
}

// Affichage de la liste
void printListe(ListeCle *liste)
{
    if (liste == NULL)
        exit(EXIT_FAILURE);

    Element *actuel = liste->premier;

    printf("Liste de cle : ");
    while (actuel != NULL)
    {
        print_uint128_bis(&(actuel->cle));
        printf(" -> ");
        actuel = actuel->suivant;
    }
    printf("NULL\n");
}
