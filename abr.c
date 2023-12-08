#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cle.h"
#include "liste.h"
#include "abr.h"

// Fonction pour créer un nouveau nœud
ABR *creerNoeudABR(uint128_t cle)
{
    ABR *nouveauNoeud = malloc(sizeof(ABR));
    if (nouveauNoeud == NULL)
    {
        fprintf(stderr, "Échec de l'allocation de mémoire pour le nouveau nœud.\n");
        exit(EXIT_FAILURE);
    }

    nouveauNoeud->cle = cle;
    nouveauNoeud->gauche = NULL;
    nouveauNoeud->droite = NULL;
    return nouveauNoeud;
}

// Fonction auxiliaire de printABR
void printABRBis(ABR *arbre, char *indentation)
{
    if (arbre == NULL)
    {
        printf("vide");
    }
    else
    {
        printf("Noeud( ");
        print_uint128_bis(&arbre->cle);
        printf("\n%s  Gauche: ", indentation);
        printABRBis(arbre->gauche, strcat(strdup(indentation), "       "));
        printf("\n%s  Droite: ", indentation);
        printABRBis(arbre->droite, strcat(strdup(indentation), "        "));
        printf(" )");
    }
}

// Affiche l'arbre
void printABR(ABR *arbre)
{
    printABRBis(arbre, "");
}

// Ajouter une clé dans un ABR :
void ajout(ABR **arbre, uint128_t cle)
{
    if (*arbre == NULL)
        *arbre = creerNoeudABR(cle);

    else
    {
        if (eg(cle, (*arbre)->cle))
            fprintf(stderr, "Ajout d'une cle deja existant dans l'ABR\n");
        else if (inf(cle, (*arbre)->cle))

            ajout(&((*arbre)->gauche), cle);

        else
            ajout(&((*arbre)->droite), cle);
    }
}

// Construit un ABR avec une liste de clés
void construction(ABR **arbre, ListeCle *liste)
{
    Element *position = liste->premier;

    while (position != NULL)
    {
        ajout(arbre, position->cle);
        position = position->suivant;
    }
}

// Cherche si une clé est dans l'arbre
bool chercherCle(ABR *arbre, uint128_t cle)
{
    if (arbre == NULL)
        return false;
    else
    {
        if (eg(cle, arbre->cle))
            return true;
        else if (inf(cle, arbre->cle))
            return chercherCle(arbre->gauche, cle);
        else
            return chercherCle(arbre->droite, cle);
    }
}
