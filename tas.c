#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cle.h"
#include "tas.h"
#define REPEATS 10
#define INITIAL_CAPACITY 1000


//initialiser le tas
Tas init_tas()
{
    Tas *tas = malloc(sizeof(Tas));
    tas->cles = malloc(INITIAL_CAPACITY * sizeof(uint128_t));
    tas->taille = 0;
    tas->capacite = INITIAL_CAPACITY;
    return *tas;
}
//afficher le tas
void print_tas(Tas *tas)
{
    
    for (int i = 0; i < tas->taille; ++i)
    {
        printf("\n------    clé_128 %d   ------\n",i);
        print_uint128(&tas->cles[i]);
    }
}
// Fonction pour échanger deux éléments uint128_t
void echanger(uint128_t *a, uint128_t *b)
{
    uint128_t temp = *a;
    *a = *b;
    *b = temp;
}

// Fonction pour restaurer les propriétés d'un tas min après la suppression du min
void SiftDown(Tas *tas, int index) {
    while (index < tas->taille) {
        int gauche = 2 * index + 1;
        int droite = 2 * index + 2;
        int plusPetit = index;

        if (gauche < tas->taille && inf(tas->cles[gauche], tas->cles[index])) {
            plusPetit = gauche;
        }
        if (droite < tas->taille && inf(tas->cles[droite], tas->cles[plusPetit])) {
            plusPetit = droite;
        }
        if (plusPetit != index) {
            echanger(&tas->cles[index], &tas->cles[plusPetit]);
            index = plusPetit; // Continue sifting down the child
        } else {
            break; // The heap property is satisfied
        }
    }
}

// SupprMin : Supprime l'élément minimal du tas
void SupprMin(Tas *tas)
{
    
    if (tas->taille == 0)
        return;

    // Place le dernier élément à la racine
    tas->cles[0] = tas->cles[tas->taille - 1];
    tas->taille--;

    // Restaure les propriétés du tas min
    SiftDown(tas, 0);
    // printf("SupprMin après: ");
    // print_tas(tas);
}
bool isMinHeap(Tas *tas, int size) {
    for (int i = 0; i < (size - 2) / 2; i++) {
        // If the left child is greater, the property is violated.
        if (inf(tas->cles[2*i + 1] , tas->cles[i]))
            return false;
        
        // If the right child exists and is greater, the property is violated.
        if (2*i + 2 < size && inf(tas->cles[2*i + 2] , tas->cles[i]))
            return false;
    }
    return true;
}

// Ajout : Ajoute un nouvel élément au tas
void Ajout(Tas *tas, uint128_t cle) {
    // printf("Ajout avant: ");
    // print_tas(tas);
    if (tas->taille >= tas->capacite) {
        int nouvelleCapacite = tas->capacite + (tas->capacite >> 1);  // Augmenter la capacité de 50%
        // Augmenter la capacité du tas
        uint128_t *nouvellesCles = realloc(tas->cles, nouvelleCapacite * sizeof(uint128_t));
        if (!nouvellesCles) {
            perror("Allocation failure in Ajout");
            exit(EXIT_FAILURE);
        }
        // Mettre à jour le tas
        tas->cles = nouvellesCles;
        tas->capacite = nouvelleCapacite;
    }
    // Ajouter la nouvelle clé à la fin du tas
    tas->cles[tas->taille] = cle;
    int i = tas->taille;
    tas->taille++;
    // Restaurer les propriétés du tas min
    while (i > 0 && inf(tas->cles[i], tas->cles[(i - 1) / 2])) {
        echanger(&tas->cles[i], &tas->cles[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
    // printf("Ajout après: ");
    // print_tas(tas);
}

// AjoutsIteratifs : Construit un tas en ajoutant itérativement chaque clé
void AjoutsIteratifs(Tas *tas, uint128_t *cles,int nbCles)
{
    // printf("Ajout avant: ");
    // print_tas(tas);
    // Ajouter chaque clé au tas
    for (int i = 0; i < nbCles; i++)
    {
        Ajout(tas, cles[i]);
        // printf("Ajout après: ");
        // print_tas(tas);
        // bool isMHeap = isMinHeap(tas, tas->taille);
        // printf("isMinHeap = %d\n", isMHeap);

    }
    printf("Ajout après: Tas size = %d\n", tas->taille);

}

// Question 2.5 Construction : Construit un tas à partir d'une liste de clés de manière efficace
Tas Construction(Tas *tas, uint128_t *cles, int nbCles)
{
    printf("Construction avant: ");
    for (int i = 0; i < nbCles; i++) {
        print_uint128(&cles[i]);
    }
    
    tas->taille = nbCles;
    tas->cles = malloc(nbCles * sizeof(uint128_t));
    if (!tas->cles)
    {
        perror("Échec de l'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    memcpy(tas->cles, cles, nbCles * sizeof(uint128_t));

    // Construire le tas en partant des nœuds à mi-chemin dans l'arbre et en descendant
    for (int i = (nbCles - 2) / 2; i >= 0; i--)
    {
        SiftDown(tas, i);
    }
    printf("Construction après: ");
    print_tas(tas);

    return *tas;
}

/*Question 2.6 Union : Fusionne deux tas en un seul*/

Tas Union(Tas *tas1, Tas *tas2) {
    // printf("Union avant:\nTas1: ");
    // print_tas(tas1);
    // printf("\nTas2: ");
    // print_tas(tas2);

    int tailleUnion = tas1->taille + tas2->taille;
    
    // vérifier si la capacité du tas1 est suffisante pour contenir toutes les clés
    if (tailleUnion > tas1->capacite) {
        // Augmenter la capacité du tas1
        tas1->capacite = tailleUnion;
        uint128_t *new_cles = realloc(tas1->cles, tas1->capacite * sizeof(uint128_t));
        if (!new_cles) {
            perror("Échec de l'allocation mémoire");
            return *tas1;
        }
        tas1->cles = new_cles;
    }

    // Copier les clés de tas2 dans les positions restantes de tas1
    memcpy(tas1->cles + tas1->taille, tas2->cles, tas2->taille * sizeof(uint128_t));
    tas1->taille = tailleUnion;

    // Construire le nouveau tas à partir de la liste fusionnée
    for (int i = (tailleUnion - 2) / 2; i >= 0; i--) {
        SiftDown(tas1, i);
    }

    // printf("Union après: ");
    // print_tas(tas1);

    return *tas1;
}









