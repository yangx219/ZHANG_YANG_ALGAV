#include <stdio.h>
#include <stdlib.h>
#include "filesBinomiales.h"
#include <limits.h>
#include <math.h>

//lier deaux tas binomiaux de même degré
BinNode lier(BinNode tree1, BinNode tree2) {
    if (tree1->cle > tree2->cle) {
        return lier(tree2, tree1);
    }
    tree2->parent = tree1;
    tree2->frere = tree1->enfant;
    tree1->enfant = tree2;
    tree1->degree++;
    return tree1;
}

//initialiser un file binomiale
BinHeap Init() {
    //H est un pointeur vers un tas binomial
    BinHeap H = (BinHeap)malloc(sizeof(struct Heap));
    if (!H) return NULL;
    H->size = 0;
    H->capacity = 10; //capacité initiale
    //list est un pointeur vers un tableau de pointeurs vers des tas binomiaux
    H->list = (BinNode *)malloc(sizeof(BinNode) * H->capacity);
    //si l'allocation échoue, libérer la mémoire et retourner NULL
    if (!H->list) {
        free(H);
        return NULL;
    }
    //initialiser tous les pointeurs à NULL
    for (int i = 0; i < H->capacity; i++) {
        H->list[i] = NULL;
    }

    return H;
}

//étendre la capacité du file binomiale
void expandHeap(BinHeap H) {
    int newCapacity = H->capacity * 2;
    H->list = (BinNode *)realloc(H->list, sizeof(BinNode) * newCapacity);
    for (int i = H->capacity; i < newCapacity; i++) {
        H->list[i] = NULL;
    }
    H->capacity = newCapacity;
}

//union de deux tas binomiaux
void fb_union(BinHeap H1, BinHeap H2) {
    //si size de sum est supérieur à la capacité, étendre la capacité
    if (H1->size + H2->size > H1->capacity) {
        expandHeap(H1);
    }
    //// 'carry' est un pointeur vers un nœud binomial utilisé pour retenir le nœud pendant la fusion
    BinTree carry = NULL;
    //parcourir les deux files binomiaux
    for (int i = 0; i < H1->capacity; i++) {
        //t1 et t2 sont les arbres correspondants dans H1 et H2
        BinTree t1 = i < H1->size ? H1->list[i] : NULL;
        BinTree t2 = i < H2->size ? H2->list[i] : NULL;
        // Calculer le cas de fusion en fonction de la présence des arbres et du report
        int caseNum = (t1 != NULL) + 2 * (t2 != NULL) + 4 * (carry != NULL);
        switch (caseNum) {
            //Aucun arbre présent
            case 0: 
            //Uniquement l'arbre de H1 est présent
            case 1:  
                break;
            //Uniquement l'arbre de H2 est présent
            case 2: 
                H1->list[i] = t2;
                H2->list[i] = NULL;
                break;
            //Les arbres de H1 et H2 sont présents, fusionner et mettre le résultat dans 'carry'
            case 3: 
                carry = lier(t1, t2);
                H1->list[i] = H2->list[i] = NULL;
                break;
            //Uniquement l'arbre de carry est présent
            case 4: 
                H1->list[i] = carry;
                carry = NULL;
                break;
            //Les arbres de H1 et carry sont présents, fusionner et mettre le résultat dans 'carry'
            case 5:  
                carry = lier(t1, carry);
                H1->list[i] = NULL;
                break;
            //Les arbres de H2 et carry sont présents, fusionner et mettre le résultat dans 'carry'
            case 6:  
                carry = lier(t2, carry);
                H2->list[i] = NULL;
            break;
            //Les arbres de H1, H2 et carry sont présents, fusionner et mettre le résultat dans 'carry'
            case 7:  
                H1->list[i] = carry;
                carry = lier(t1, t2);
                H2->list[i] = NULL;
                break;
            }
        }
    // Mettre à jour la taille de H1 pour refléter les changements après la fusion
    int new_size = 0;
    for (int i = 0; i < H1->capacity; i++) {
        if (H1->list[i] != NULL) {
            new_size = i + 1; 
        }
    }
    H1->size = new_size;
    H2->size = 0;
}
    
//ajouter un élément au file binomial 
void ajout(BinHeap H, int C) {
    //si la taille du file binomial est égale à sa capacité, étendre la capacité
    if (H->size == H->capacity) {
        expandHeap(H);
    }
    //créer un nouvel file binomial avec un seul noeud
    BinNode newNode = (BinNode)malloc(sizeof(struct Node));
    newNode->cle = C;
    newNode->degree = 0;
    newNode->parent = NULL;
    newNode->enfant = NULL;
    newNode->frere = NULL;

    BinHeap tempHeap = Init();
    tempHeap->list[0] = newNode;
    tempHeap->size = 1; // tempHeap a un seul tas 

    fb_union(H, tempHeap);
    free(tempHeap); // libérer la mémoire tempHeap
}

//supprimer l'élément minimum du tas binomial
int SupprMin(BinHeap H) {
    //si le file binomial est vide, retourner -1
    if (H == NULL || H->size == 0) {
        return -1;
    }
    //trouver le plus petit élément dans le file binomial
    int minIndex = -1;
    int minCle = INT_MAX;
    //parcourir tous les tas binomiaux dans le file binomial
    for (int i = 0; i < H->capacity; i++) {
        if (H->list[i] != NULL && H->list[i]->cle < minCle) {
            minCle = H->list[i]->cle;
            minIndex = i;
        }
    }
    //si le plus petit élément n'est pas trouvé, retourner -1
    if (minIndex == -1) {
        return -1;
    }
    //supprimer le plus petit élément du file binomial
    BinNode minTreeRoot = H->list[minIndex];
    H->list[minIndex] = NULL;
    //inverser la liste d'enfants de minTreeRoot
    BinNode prev = NULL;
    BinNode current = minTreeRoot->enfant;
    while (current != NULL) {
        BinNode next = current->frere;
        current->frere = prev;
        prev = current;
        current = next;
    }
    //créer un nouveau file binomial avec les arbres inversés
    BinHeap newHeap = Init();
    newHeap->size = (1 << minIndex) - 1;
    int childIndex = 0;
    while (prev != NULL) {
        newHeap->list[childIndex] = prev;
        prev = prev->frere;
        newHeap->list[childIndex]->frere = NULL;
        childIndex++;
    }
    //fusionner le nouveau file binomial avec le file binomial d'origine
    fb_union(H, newHeap);
    free(newHeap); 
    free(minTreeRoot);

    if (H->size > (1 << minIndex)) {
        H->size -= (1 << minIndex);
    } else {
        H->size = 0;
    }

    return minCle;
}



// Construction d'un file binomial à partir d'un tableau d'éléments
BinHeap fb_Construction(int *elements, int n) {
    BinHeap H = Init(); // Initialize a new binomial heap
    for (int i = 0; i < n; i++) {
        ajout(H, elements[i]); // Add each element to the heap
    }
    return H;
}

void traverse(BinTree L) {
    //parcourir l'arbre en utilisant un parcours préfixe
    if (L) {
        printf("%d ", L->cle);
        traverse(L->enfant);
        traverse(L->frere);
    }
}

void afficher(BinHeap H) {
    printf("Binomial Heap:\n");
    //parcourir tous les tas binomiaux dans le file binomial
    for (int i = 0; i < H->capacity; i++) {
        //si le tas binomial est présent, afficher le tas
        if (H->list[i]) {
            printf("Tree of degree %d:\n", i);
            traverse(H->list[i]);
            printf("\n");
        }
    }
}
