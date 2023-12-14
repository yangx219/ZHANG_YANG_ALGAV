#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "cle.h"
#include "tasArbre.h"
#include "liste.h"

/****************************************************/
/*                                                  */
/*       Fonction pour créer un nouveau noeud        */
/*                                                  */
/****************************************************/

TasArbre *creerNoeud(uint128_t cle)
{
    TasArbre *nouveauNoeud = malloc(sizeof(TasArbre));
    nouveauNoeud->cle = cle;
    nouveauNoeud->gauche = NULL;
    nouveauNoeud->droite = NULL;
    return nouveauNoeud;
}

/****************************************************/
/*                                                  */
/*              Affichage de TasArbre               */
/*                                                  */
/****************************************************/

// fonction auxiliaire de printTasArbre
void printTasArbreBis(TasArbre *tas, char *indentation)
{
    if (tas == NULL)
    {
        printf("vide");
    }
    else
    {
        printf("( ");
        print_uint128_bis(&tas->cle);
        printf("\n%s  Gauche: ", indentation);
        printTasArbreBis(tas->gauche, strcat(strdup(indentation), "       "));
        printf("\n%s  Droite: ", indentation);
        printTasArbreBis(tas->droite, strcat(strdup(indentation), "        "));
        printf(" )");
    }
}

// Affiche le tas
void printTasArbre(TasArbre *arbre)
{
    printTasArbreBis(arbre, "");
}

/****************************************************/
/*                                                  */
/*                Ajout d'une clé                   */
/*                                                  */
/****************************************************/

// Fonction pour chercher la profondeur maximale du noeud le plus bas a gauche
int profondeurMaxGauche(TasArbre *tas)
{
    if (tas == NULL)
        return -1; // Profondeur -1 pour un tas vide (0 - 1)
    else
    {
        int profondeur = profondeurMaxGauche(tas->gauche);
        return 1 + profondeur;
    }
}

// Fonction pour chercher la profondeur maximale du noeud le plus bas a droite
int profondeurMaxDroite(TasArbre *tas)
{
    if (tas == NULL)
        return -1; // Profondeur -1 pour un tas vide (0 - 1)
    else
    {
        int profondeur = profondeurMaxDroite(tas->droite);
        return 1 + profondeur;
    }
}

// Fonction pour échanger deux éléments uint128_t
void echangerCle(TasArbre *a, TasArbre *b)
{
    uint128_t temp = a->cle;
    a->cle = b->cle;
    b->cle = temp;
}

// Cherche la clé et échange avec son père si nécessaire
// rendre vrai si une échange est faite sinon faux
bool chercherEtEchanger(TasArbre **tas, uint128_t cle)
{
    // si le tas est null
    if (*tas != NULL)
    {
        // si le fils gauche est null
        if ((*tas)->gauche != NULL)
        {
            // si la clé du noeud deu fils gauche du noeud courant est egal à la clé
            if (eg((*tas)->gauche->cle, cle))
            {
                // si la clé du fils gauche est inférieur à la clé du noeud courand alors échanger
                if (inf(cle, (*tas)->cle))
                {
                    echangerCle(*tas, (*tas)->gauche);
                    return true;
                }
                return false;
            }
        }
        // si le fils droit est null
        if ((*tas)->droite != NULL)
        {
            if (eg((*tas)->droite->cle, cle))
            {
                // si la clé du fils droit est inférieur à la clé du noeud courand alors échanger
                if (inf(cle, (*tas)->cle))
                {
                    echangerCle(*tas, (*tas)->droite);
                    return true;
                }
                return false;
            }
        }
        if ((*tas)->gauche != NULL && (*tas)->droite != NULL)
            return chercherEtEchanger(&(*tas)->gauche, cle) || chercherEtEchanger(&(*tas)->droite, cle);
    }
    return false;
}

// Cherche la cle dans le tas et le remonte si il est inferieur à la cle de son père
void remonter(TasArbre **tas, uint128_t cle)
{
    bool b = true;
    while (b)
        b = chercherEtEchanger(tas, cle);
}

// ajoute la cle au noeud vide positionné le plus a droite et le plus bas possible
// si l'arbre n'est pas parfait
// rendre vrai si la clé est ajouté sinon faux
bool ajoutBis(TasArbre **tas, TasArbre *cle, int pMax, int p)
{
    // si le noeud est null
    if (*tas == NULL)
    {
        // si le noeud courant est au pronfondeux max
        if (pMax == p)
        {
            // ajoute la cle et retourne true
            *tas = cle;
            return true;
        }
        return false;
    }
    else
    {
        // si la clé n'est pas ajouté au fils gauche alors ajouté au fils droit
        bool b = ajoutBis(&((*tas)->gauche), cle, pMax, p + 1);
        if (!b)
        {
            b = ajoutBis(&((*tas)->droite), cle, pMax, p + 1);
        }
        return b;
    }
}

// Ajouter une clé dans un tas :
// Mettre la clé à la position du noeud null le plus bas et plus à droite
// et remonte si la clé de son père est supérieur à lui
void ajout(TasArbre **tas, uint128_t cle)
{
    // creer le tas contenant cle
    TasArbre *nouveauNoeud = creerNoeud(cle);

    // si le tas est vide
    if (*tas == NULL)
        *tas = nouveauNoeud;
    else
    {
        // cherche la profondeur max gauche
        int pMaxG = profondeurMaxGauche(*tas);
        // cherche la profondeur max droite
        int pMaxD = profondeurMaxDroite(*tas);

        // vérifie si l'abre est parfait
        if (pMaxG == pMaxD)
        {
            TasArbre *position = *tas;
            while (position->gauche != NULL)
                position = position->gauche;
            position->gauche = nouveauNoeud;
            remonter(tas, cle);
        }
        // si l'arbre n'est pas parfait
        else
        {
            bool b = ajoutBis(tas, nouveauNoeud, pMaxG, 0);
            remonter(tas, cle);
        }
    }
}

/****************************************************/
/*                                                  */
/*          Ajout itératif d'une liste de clé       */
/*                                                  */
/****************************************************/

// AjoutsIteratifs : Construit un tas avec une liste de clé
void ajoutsIteratifs(TasArbre **tas, ListeCle *liste)
{
    Element *position = liste->premier;

    if (position != NULL)
    {
        do
        {
            ajout(tas, position->cle);
            position = position->suivant;
        } while (position != NULL);
    }
}

// ajoutsIteratifsAfficheEtape : Construit un tas avec une liste de clé en affichant chaque état
void ajoutsIteratifsAfficheEtape(TasArbre **tas, ListeCle *liste)
{
    Element *position = liste->premier;

    while (position != NULL)
    {
        ajout(tas, position->cle);
        position = position->suivant;
        printf("\ntas arbre apres ajout:\n");
        printTasArbre(*tas);
        printf("\n\n");
    }
}

/****************************************************/
/*                                                  */
/*           Supprimer le minimun d'un tas          */
/*                                                  */
/****************************************************/

// Recherche le dernier nœud non NULL de profondeur pMax dans un tas binaire
TasArbre **chercherDernierNoeud(TasArbre **tas, int pMax, int p)
{
    if (*tas == NULL)
    {
        return NULL; // Aucun nœud trouvé à cette profondeur
    }
    else if (p == pMax && (*tas)->gauche == NULL && (*tas)->droite == NULL)
    {
        return tas; // Dernier nœud non NULL trouvé à cette profondeur
    }
    else
    {
        // Recherche à gauche et à droite pour le dernier nœud non NULL
        TasArbre **a = chercherDernierNoeud(&(*tas)->gauche, pMax, p + 1);
        TasArbre **b = chercherDernierNoeud(&(*tas)->droite, pMax, p + 1);

        // Retourne le dernier nœud non NULL trouvé de profondeur pMax, soit à gauche, soit à droite
        return (b != NULL) ? b : a;
    }
}

// fait decendre la cle du noeud courant si il est supérieur au clé des fils, et échanger avec la clé min entre les fils
void descendre(TasArbre **tas)
{
    if (*tas != NULL && (*tas)->gauche != NULL && (*tas)->droite != NULL)
    {
        // Vérifier si la clé du fils gauche est inférieure à la clé du père
        bool b1 = inf((*tas)->gauche->cle, (*tas)->cle);
        // Vérifier si la clé du fils droit est inférieure à la clé du père
        bool b2 = inf((*tas)->droite->cle, (*tas)->cle);
        // Vérifier si la clé du fils gauche est inférieure à la clé du fils droit
        bool b3 = inf((*tas)->gauche->cle, (*tas)->droite->cle);

        if (b1 && b3)
        {
            // Échanger la clé du père avec la clé du fils gauche
            echangerCle(*tas, (*tas)->gauche);
            // Descendre récursivement dans le sous-arbre gauche
            descendre(&(*tas)->gauche);
        }
        else if (b2 && !b3)
        {
            // Échanger la clé du père avec la clé du fils droit
            echangerCle(*tas, (*tas)->droite);
            // Descendre récursivement dans le sous-arbre droit
            descendre(&(*tas)->droite);
        }
    }
    else if (*tas != NULL && (*tas)->gauche != NULL && inf((*tas)->gauche->cle, (*tas)->cle))
    {
        // Si seul le fils gauche est présent et sa clé est inférieure à celle du père
        // Échanger la clé du père avec la clé du fils gauche
        echangerCle(*tas, (*tas)->gauche);
        // Descendre récursivement dans le sous-arbre gauche
        descendre(&(*tas)->gauche);
    }
}

// supprime le min qui est la racine, le remplace par le noeud le plus bas et plus a droite
// le faire decendre si supérieur au fils
void suppMin(TasArbre **tas)
{
    // si le tas est vide
    if (*tas == NULL)
        printf("Le tas est vide");
    else
    {
        // cherche la profondeur max gauche
        int pMaxG = profondeurMaxGauche(*tas);
        // cherche la profondeur max droite
        int pMaxD = profondeurMaxDroite(*tas);

        // vérifie si l'abre est parfait
        if (pMaxG == pMaxD)
        {
            TasArbre **position = tas;
            // cherche le dernier noeud de profondeur max non null
            while ((*position)->droite != NULL)
                position = &((*position)->droite);

            // supprime le min et met la cle du dernier noeud à la racine
            (*tas)->cle = (*position)->cle;
            (*position) = NULL;

            descendre(tas);
        }
        // si l'arbre n'est pas parfait
        else
        {
            // cherche le dernier noeud de profondeur max non null
            TasArbre **dernierNoeud = chercherDernierNoeud(tas, pMaxG, 0);
            // supprime le min et met la cle du dernier noeud à la racine
            (*tas)->cle = (*dernierNoeud)->cle;
            (*dernierNoeud) = NULL;

            descendre(tas);
        }
    }
}

/****************************************************/
/*                                                  */
/*              Construction efficace               */
/*                                                  */
/****************************************************/

// Ajouter une clé dans un tas :
// Mettre la clé à la position du noeud null le plus bas et plus à droite
void ajoutSimple(TasArbre **tas, uint128_t cle)
{
    // creer le tas contenant cle
    TasArbre *nouveauNoeud = creerNoeud(cle);

    // si le tas est vide
    if (*tas == NULL)
        *tas = nouveauNoeud;
    else
    {
        // cherche la profondeur max gauche
        int pMaxG = profondeurMaxGauche(*tas);
        // cherche la profondeur max droite
        int pMaxD = profondeurMaxDroite(*tas);

        // vérifie si l'abre est parfait
        if (pMaxG == pMaxD)
        {
            TasArbre *position = *tas;
            while (position->gauche != NULL)
                position = position->gauche;
            position->gauche = nouveauNoeud;
        }
        // si l'arbre n'est pas parfait
        else
            ajoutBis(tas, nouveauNoeud, pMaxG, 0);
    }
}

// applique la méthode descendre tout les noeud du niveau p
void descendreNoeudMemeNiveau(TasArbre **tas, int niveau, int p)
{
    if (*tas != NULL && niveau == p)
    {
        descendre(tas);
    }
    else if (*tas != NULL && niveau != p)
    {
        descendreNoeudMemeNiveau(&(*tas)->gauche, niveau, p + 1);
        descendreNoeudMemeNiveau(&(*tas)->droite, niveau, p + 1);
    }
}

// Construit un tas avec une liste de clé
void construction(TasArbre **tas, ListeCle *liste)
{
    // printf("Construction avant: ");
    Element *position = liste->premier;

    while (position != NULL)
    {
        ajoutSimple(tas, position->cle);
        position = position->suivant;
    }
    // parcourt noeud niveau par niveau en partant de l'avant dernier niveau (profondeurMax -1)
    // et applique de méthode descendre
    int niveau = profondeurMaxGauche(*tas) - 1;

    for (int p = niveau; p >= 0; p--)

        // Appliquer la méthode descendre sur chaque noeud du niveau p
        descendreNoeudMemeNiveau(tas, p, 0);

    // printf("Construction après: ");
}

// Construit un tas avec une liste de clé en affichant chaque état
void constructionAfficheEtape(TasArbre **tas, ListeCle *liste)
{
    Element *position = liste->premier;
    // construit un arbre binaire parfait avec tous les éléments sans se soucier de la contrainte d'ordre

    while (position != NULL)
    {
        ajoutSimple(tas, position->cle);
        position = position->suivant;
        printf("\ntas arbre apres ajout simple:\n");
        printTasArbre(*tas);
        printf("\n\n");
    }

    // parcourt noeud niveau par niveau en partant de l'avant dernier niveau (profondeurMax -1)
    // et applique de méthode descendre
    int niveau = profondeurMaxGauche(*tas) - 1;

    for (int p = niveau; p >= 0; p--)
    {
        // Appliquer la méthode descendre sur chaque noeud du profondeur p
        descendreNoeudMemeNiveau(tas, p, 0);

        // Afficher l'état du tas après chaque application de la méthode descendre
        printf("\ntas arbre apres application de descendre au profondeur %d :\n", p);
        printTasArbre(*tas);
        printf("\n\n");
    }
}

/****************************************************/
/*                                                  */
/*              Construction efficace               */
/*                                                  */
/****************************************************/

// Fonction pour fusionner deux tas et retourner le nouveau tas
TasArbre *unionTasArbre(TasArbre *tas1, TasArbre *tas2)
{
    // Créer un tas résultant vide
    TasArbre *tasResultat = NULL;
    TasArbre *copieTas1 = tas1;
    TasArbre *copieTas2 = tas2;

    // Fusionner les deux tas jusqu'à ce qu'un des tas soit vide
    while (copieTas1 != NULL && copieTas2 != NULL)
    {
        // Choisissez la racine avec la clé la plus petite
        if (inf(copieTas1->cle, copieTas2->cle))
        {
            ajoutSimple(&tasResultat, copieTas1->cle);
            suppMin(&copieTas1);
        }
        else
        {
            ajoutSimple(&tasResultat, copieTas2->cle);
            suppMin(&copieTas2);
        }
    }

    // Ajouter les éléments restants du tas non vide
    while (copieTas1 != NULL)
    {
        ajoutSimple(&tasResultat, copieTas1->cle);
        suppMin(&copieTas1);
    }

    while (copieTas2 != NULL)
    {
        ajoutSimple(&tasResultat, copieTas2->cle);
        suppMin(&copieTas2);
    }

    return tasResultat;
}

// Fonction pour fusionner deux tas et retourner le nouveau tas en affichant chaque étape
void *unionTasArbreAfficheEtape(TasArbre *tas1, TasArbre *tas2)
{
    // Créer un tas résultant vide
    TasArbre *tasResultat = NULL;
    TasArbre *copieTas1 = tas1;
    TasArbre *copieTas2 = tas2;

    // Fusionner les deux tas jusqu'à ce qu'un des tas soit vide
    while (copieTas1 != NULL && copieTas2 != NULL)
    {
        // Choisissez la racine avec la clé la plus petite
        if (inf(copieTas1->cle, copieTas2->cle))
        {
            ajoutSimple(&tasResultat, copieTas1->cle);
            suppMin(&copieTas1);
        }
        else
        {
            ajoutSimple(&tasResultat, copieTas2->cle);
            suppMin(&copieTas2);
        }

        // Afficher l'état du tas résultant à chaque étape
        printf("\nEtape de l'union :\n");
        printTasArbre(tasResultat);
        printf("\n\n");
    }

    // Ajouter les éléments restants du tas non vide
    while (copieTas1 != NULL)
    {
        ajoutSimple(&tasResultat, copieTas1->cle);
        suppMin(&copieTas1);

        // Afficher l'état du tas résultant à chaque étape
        printf("\nEtape de l'union :\n");
        printTasArbre(tasResultat);
        printf("\n\n");
    }

    while (copieTas2 != NULL)
    {
        ajoutSimple(&tasResultat, copieTas2->cle);
        suppMin(&copieTas2);

        // Afficher l'état du tas résultant à chaque étape
        printf("\nEtape de l'union :\n");
        printTasArbre(tasResultat);
        printf("\n\n");
    }
}
