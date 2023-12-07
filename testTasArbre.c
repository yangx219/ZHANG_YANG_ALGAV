#include "tasArbre.h"
#include "cle.h"
#include "liste.h"

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
    uint128_t cle9;
    uint128_t cle10;
    uint128_t cle11;
    uint128_t cle12;
    uint128_t cle13;
    uint128_t cle14;

    // instanciation des clés
    init_uint128(&cle1, 0, 0, 0, 1);
    init_uint128(&cle2, 0, 2, 0, 8);
    init_uint128(&cle3, 7, 68, 25, 351);
    init_uint128(&cle4, 35, 43, 34, 763);
    init_uint128(&cle5, 35, 585, 575, 34);
    init_uint128(&cle6, 54, 58, 0, 8);
    init_uint128(&cle7, 75786, 68, 25, 351);
    init_uint128(&cle8, 35, 43, 0, 763);
    init_uint128(&cle9, 77337, 0, 0, 1);
    init_uint128(&cle10, 235, 2, 0, 727);
    init_uint128(&cle11, 212, 68, 25, 351);
    init_uint128(&cle12, 38, 245, 34554, 763);
    init_uint128(&cle13, 123, 8345, 3543, 453);
    init_uint128(&cle14, 76, 534, 345, 265);

    // Déclaration et instanciation d'une liste
    ListeCle *liste1 = initialisation();
    insertion(liste1, cle1);
    insertion(liste1, cle2);
    insertion(liste1, cle3);
    insertion(liste1, cle4);
    insertion(liste1, cle5);
    insertion(liste1, cle6);
    insertion(liste1, cle7);

    // Déclaration et instanciation d'une liste
    ListeCle *liste2 = initialisation();
    insertion(liste2, cle8);
    insertion(liste2, cle9);
    insertion(liste2, cle10);
    insertion(liste2, cle11);
    insertion(liste2, cle12);
    insertion(liste2, cle13);
    insertion(liste2, cle14);

    // Tas vide
    TasArbre *tas1 = NULL;
    TasArbre *tas2 = NULL;

    printf("_________________________________________________________________________\n\n");
    printf("Ajout iteratif d'une liste de cle dans un tas vide tas1: \n\n");
    printListe(liste1);

    printf("\n\nEtas du tas apres chaque ajout (ajoutIteratifs) : \n");
    ajoutsIteratifsAfficheEtape(&tas1, liste1);
    printf("_________________________________________________________________________\n\n");

    // suppirme min
    suppMin(&tas1);
    printf("Supprimer le minimum de tas1 :\n");
    printTasArbre(tas1);
    suppMin(&tas1);

    printf("\n\nSupprimer le minimum de tas1:\n");
    printTasArbre(tas1);
    printTasArbre(tas1);
    printf("\n_________________________________________________________________________\n\n");

    // construit un tas
    printf("Construction du tas2 : \n");
    printListe(liste2);
    printf("\n");
    constructionAfficheEtape(&tas2, liste2);
    printf("_________________________________________________________________________\n\n");

    // union de deux tas
    printf("Union de tas1 et tas2 : \n");
    unionTasArbreAfficheEtape(tas1, tas2);

    return 0;
}
