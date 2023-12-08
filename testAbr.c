#include <stdbool.h>
#include <string.h>
#include "cle.h"
#include "liste.h"
#include "abr.h"

int main()
{
    // déclaration des clés
    uint128_t cle1;
    uint128_t cle2;
    uint128_t cle3;
    uint128_t cle4;
    uint128_t cle5;
    uint128_t cle6;
    uint128_t cle7;
    uint128_t cle8;

    // instanciation des clés
    init_uint128(&cle1, 6115, 0, 0, 1);
    init_uint128(&cle2, 76, 2, 0, 8);
    init_uint128(&cle3, 842, 68, 25, 351);
    init_uint128(&cle4, 35, 43, 34, 763);
    init_uint128(&cle5, 91, 585, 575, 34);
    init_uint128(&cle6, 5, 58, 0, 8);
    init_uint128(&cle7, 46, 68, 25, 351);
    init_uint128(&cle8, 35, 43, 0, 763);

    // Déclaration et instanciation d'une liste
    ListeCle *liste = initialisation();

    insertion(liste, cle1);
    insertion(liste, cle2);
    insertion(liste, cle3);

    insertion(liste, cle4);
    insertion(liste, cle5);
    insertion(liste, cle6);
    insertion(liste, cle7);

    // ABR vide
    ABR *arbre = NULL;

    printf("_________________________________________________________________________\n\n");
    printf("Liste de cle a ajouter dans l'ABR : \n\n");
    printListe(liste);

    printf("\n\nArbre : \n");
    construction(&arbre, liste);
    printABR(arbre);

    bool b1 = chercherCle(arbre, cle7);
    printf("\n\nCheche une cle existant ");
    print_uint128_bis(&cle7);
    printf(": %s ", b1 ? "true" : "false");

    bool b2 = chercherCle(arbre, cle8);
    printf("\n\nCheche une cle inexistant ");
    print_uint128_bis(&cle8);
    printf(": %s ", b2 ? "true" : "false");

    return 0;
}