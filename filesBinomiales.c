#include <stdio.h>
#include <stdlib.h>
#include "filesBinomiales.h"
#include "tas.h"

// Création d'un noeud binomial
NoeudBinomial* creerNoeud(int cle) {
    NoeudBinomial* nouveau = (NoeudBinomial*)malloc(sizeof(NoeudBinomial));
    if (!nouveau) {
        return NULL;
    }
    nouveau->cle = cle;
    nouveau->degre = 0;
    nouveau->parent = NULL;
    nouveau->enfant = NULL;
    nouveau->frere = NULL;
    return nouveau;
}   

// Initialisation d'une file binomiale
FileBinomiale* initFileBinomiale() {
    FileBinomiale* Tas = (FileBinomiale*)malloc(sizeof(FileBinomiale));
    if (!Tas) { 
        return NULL;
    }
    Tas->tete = NULL;
    return Tas;
}

// 合并两个 NoeudBinomial 结点
NoeudBinomial* binomial_merge(NoeudBinomial* h1, NoeudBinomial* h2) 
{
    NoeudBinomial* head = NULL;
    NoeudBinomial** pos = &head;

    while (h1 && h2)
    {
        if (h1->degre < h2->degre)
        {
            *pos = h1;
            h1 = h1->frere;
        } 
        else 
        {
            *pos = h2;
            h2 = h2->frere;
        }
        pos = &(*pos)->frere;
    }
    if (h1)
        *pos = h1;
    else
        *pos = h2;

    return head;
}

// 将一个 NoeudBinomial 结点 child 合并到 heap 中
void binomial_link(NoeudBinomial* child, NoeudBinomial* heap) 
{
    child->parent = heap;
    child->frere = heap->enfant;
    heap->enfant = child;
    heap->degre++;
}

// 合并两个二项堆，并返回合并后的堆

FileBinomiale* binomial_union(FileBinomiale* h1, FileBinomiale* h2) 
{
    FileBinomiale *heap;
    NoeudBinomial *prev_x, *x, *next_x;

    // 将h1, h2中的根表合并成一个按度数递增的链表heap
    heap = initFileBinomiale();
    if (!heap)
        return NULL;

    heap->tete = binomial_merge(h1->tete, h2->tete);

    prev_x = NULL;
    x      = heap->tete;
    next_x = x->frere;

    while (next_x != NULL)
    {
        if (   (x->degre != next_x->degre) 
            || ((next_x->frere != NULL) && (next_x->degre == next_x->frere->degre))) 
        {
            // Case 1: x->degre != next_x->degre
            // Case 2: x->degre == next_x->degre == next_x->frere->degre
            prev_x = x;
            x = next_x;
        } 
        else if (x->cle <= next_x->cle) 
        {
            // Case 3: x->degre == next_x->degre != next_x->frere->degre
            //      && x->cle    <= next_x->cle
            x->frere = next_x->frere;
            binomial_link(next_x, x);
        } 
        else 
        {
            // Case 4: x->degre == next_x->degre != next_x->frere->degre
            //      && x->cle    >  next_x->cle
            if (prev_x == NULL) 
            {
                heap->tete = next_x;
            } 
            else 
            {
                prev_x->frere = next_x;
            }
            binomial_link(x, next_x);
            x = next_x;
        }
        next_x = x->frere;
    }

    return heap;
}



// Supprimer le minimum
NoeudBinomial* supprMin(FileBinomiale* H) {
    if (!H || !H->tete) return NULL;

    NoeudBinomial *minPrev = NULL;
    NoeudBinomial *min = H->tete;
    NoeudBinomial *current = H->tete;

    while (current->frere) {
        if (current->frere->cle < min->cle) {
            min = current->frere;
            minPrev = current;
        }
        current = current->frere;
    }

    if (minPrev) {
        minPrev->frere = min->frere;
    } else {
        H->tete = min->frere;
    }

    FileBinomiale* H1 = initFileBinomiale();
    if (!H1) return NULL; 

    NoeudBinomial *child = min->enfant;
    NoeudBinomial *temp;
    while (child) {
        temp = child->frere;
        child->frere = H1->tete;
        child->parent = NULL;
        H1->tete = child;
        child = temp;
    }

    FileBinomiale* newHeap = binomial_union(H, H1);
    if (!newHeap) return NULL; 

    H->tete = newHeap->tete;
    

    printf("Removing min node with key: %d\n", min->cle);

    min->enfant = NULL;
    return min;
}

// Ajouter un élément dans la file
FileBinomiale* ajouter(FileBinomiale* H, int cle) {
    NoeudBinomial* x = creerNoeud(cle);
    if (!x) {
        printf("Erreur : Échec de l'insertion\n");
        return H;
    }

    // 创建一个新的二项堆，其中只包含新插入的节点 x
    FileBinomiale* H1 = initFileBinomiale();
    if (!H1) return NULL;
    
    H1->tete = x;  // 将新节点设置为新堆的头节点

    // 合并原始堆 H 和新堆 H1
    FileBinomiale* newHeap = binomial_union(H, H1);
    if (!newHeap) return NULL;

    return newHeap;
}


// Construire la file par ajouts itératifs
FileBinomiale* construire(FileBinomiale* H, int* elements, int n) {
    for (int i = 0; i < n; i++) {
        H = ajouter(H, elements[i]);
    }
    return H;
}