#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
//
struct Node {
    int cle;  
    int degree;  
    struct Node *parent;
    struct Node *enfant;
    struct Node *frere;
};

typedef struct Node * BinNode;

//
struct Heap {
    int size;
    int capacity; 
    BinNode *list;
};

typedef struct Heap * BinHeap;
typedef struct Node * BinTree;

BinNode lier(BinNode tree1, BinNode tree2);
void ajout(BinHeap H,int C);
BinHeap Init();
void afficher(BinHeap H);
void traverse(BinTree L);
void fb_union(BinHeap H1,BinHeap H2);
int SupprMin(BinHeap H);
BinHeap Construction(BinHeap H,int *elements,int n);

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

BinHeap Init() {
    BinHeap H = (BinHeap)malloc(sizeof(struct Heap));
    if (!H) return NULL;

    H->size = 0;
    H->capacity = 10; // 初始容量
    H->list = (BinNode *)malloc(sizeof(BinNode) * H->capacity);
    if (!H->list) {
        free(H);
        return NULL;
    }

    for (int i = 0; i < H->capacity; i++) {
        H->list[i] = NULL;
    }

    return H;
}

void expandHeap(BinHeap H) {
    int newCapacity = H->capacity * 2;
    H->list = (BinNode *)realloc(H->list, sizeof(BinNode) * newCapacity);
    for (int i = H->capacity; i < newCapacity; i++) {
        H->list[i] = NULL;
    }
    H->capacity = newCapacity;
}

void fb_union(BinHeap H1, BinHeap H2) {
    if (H1->size + H2->size > H1->capacity) {
        expandHeap(H1);
    }
    BinTree carry = NULL;
    for (int i = 0; i < H1->capacity; i++) {
        BinTree t1 = i < H1->size ? H1->list[i] : NULL;
        BinTree t2 = i < H2->size ? H2->list[i] : NULL;

        int caseNum = (t1 != NULL) + 2 * (t2 != NULL) + 4 * (carry != NULL);
        switch (caseNum) {
            case 0:  
            case 1:  
                break;
            case 2: 
                H1->list[i] = t2;
                H2->list[i] = NULL;
                break;
            case 3: 
                carry = lier(t1, t2);
                H1->list[i] = H2->list[i] = NULL;
                break;
            case 4: 
                H1->list[i] = carry;
                carry = NULL;
                break;
             case 5:  
                carry = lier(t1, carry);
                H1->list[i] = NULL;
                break;
             case 6:  
                carry = lier(t2, carry);
                H2->list[i] = NULL;
                break;
               case 7:  
                H1->list[i] = carry;
                carry = lier(t1, t2);
                H2->list[i] = NULL;
                break;}
        }
    
    int new_size = 0;
    for (int i = 0; i < H1->capacity; i++) {
        if (H1->list[i] != NULL) {
            new_size = i + 1; // 更新new_size为最大的非空节点索引+1
        }
    }

    H1->size = new_size;
    H2->size = 0;
}
    
       
void ajout(BinHeap H, int C) {
    if (H->size == H->capacity) {
        expandHeap(H);
    }

    BinNode newNode = (BinNode)malloc(sizeof(struct Node));
    newNode->cle = C;
    newNode->degree = 0;
    newNode->parent = NULL;
    newNode->enfant = NULL;
    newNode->frere = NULL;

    BinHeap tempHeap = Init();
    tempHeap->list[0] = newNode;
    tempHeap->size = 1; // 设置tempHeap的大小为1

    fb_union(H, tempHeap);
    free(tempHeap); // 释放tempHeap的内存，但不释放其节点
}


int SupprMin(BinHeap H) {
    if (H == NULL || H->size == 0) {
        return -1;
    }

    int minIndex = -1;
    int minKey = INT_MAX;
    for (int i = 0; i < H->capacity; i++) {
        if (H->list[i] != NULL && H->list[i]->cle < minKey) {
            minKey = H->list[i]->cle;
            minIndex = i;
        }
    }

    if (minIndex == -1) {
        return -1;
    }

    BinNode minTreeRoot = H->list[minIndex];
    H->list[minIndex] = NULL;

    BinNode prev = NULL;
    BinNode current = minTreeRoot->enfant;
    while (current != NULL) {
        BinNode next = current->frere;
        current->frere = prev;
        prev = current;
        current = next;
    }

    BinHeap newHeap = Init();
    newHeap->size = (1 << minIndex) - 1;
    int childIndex = 0;
    while (prev != NULL) {
        newHeap->list[childIndex] = prev;
        prev = prev->frere;
        newHeap->list[childIndex]->frere = NULL;
        childIndex++;
    }

    fb_union(H, newHeap);
    free(newHeap); // 释放newHeap分配的内存

    free(minTreeRoot);

    if (H->size > (1 << minIndex)) {
        H->size -= (1 << minIndex);
    } else {
        H->size = 0;
    }

    return minKey;
}




BinHeap fb_Construction(int *elements, int n) {
    BinHeap H = Init(); // Initialize a new binomial heap
    for (int i = 0; i < n; i++) {
        ajout(H, elements[i]); // Add each element to the heap
    }
    return H;
}

void traverse(BinTree L) {
    if (L) {
        printf("%d ", L->cle);
        traverse(L->enfant);
        traverse(L->frere);
    }
}

void afficher(BinHeap H) {
    printf("Binomial Heap:\n");
    for (int i = 0; i < H->capacity; i++) {
        if (H->list[i]) {
            printf("Tree of degree %d:\n", i);
            traverse(H->list[i]);
            printf("\n");
        }
    }
}

int main() {
    // 法语注释 
    BinHeap H1 = Init();
    // Add some elements to the heap
    int elementsToAdd1[] = {3, 5, 2, 7, 6, 1, 8};
    int n = sizeof(elementsToAdd1) / sizeof(elementsToAdd1[0]);
    H1 = fb_Construction(elementsToAdd1, n);
    printf("*H1*\n");
    afficher(H1);

    // Remove the minimum element from the heap
    int min = SupprMin(H1);
    printf("SupprMin___*H1*: %d\n", min);
    afficher(H1);

    ajout(H1, 4);
    printf("ajout__4*H1*\n");
    afficher(H1);

    BinHeap H2 = Init();
    // Add some elements to the heap
    int elementsToAdd2[] = {9,10,11,12};
    int n2 = sizeof(elementsToAdd2) / sizeof(elementsToAdd2[0]);
    H2 = fb_Construction(elementsToAdd2, n2);
    printf("*H2*\n");
    afficher(H2);

    fb_union(H1, H2);
    printf("fb_union___(*H1* *H2*):\n");
    afficher(H1);

    return 0; 
}
