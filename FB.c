#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define MAX 10  // arbres max dans un tas binomial


struct Node {
    int cle;  
    int degree;  
    struct Node *paren;
    struct Node *enfant;
    struct Node *frere;
};

typedef struct Node * BinNode;

//
struct Heap {
    int size;  // 
    BinNode list[MAX];  // 
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
    tree2->paren = tree1;
    tree2->frere = tree1->enfant;
    tree1->enfant = tree2;
    tree1->degree++;
    return tree1;
}

BinHeap Init() {
    BinHeap H = (BinHeap)malloc(sizeof(struct Heap));
    if (!H) return NULL;

    H->size = 0;
    for (int i = 0; i < MAX; i++) {
        H->list[i] = NULL;
    }

    return H;
}
/*4 void fb_union(BinHeap H1, BinHeap H2) {
45     BinNode carry = NULL;
46     for (int i = 0; i < MAX; i++) {
47         BinNode t1 = H1->list[i];
48         BinNode t2 = (i < MAX) ? H2->list[i] : NULL; // Ensure H2->list[i] is not NULL before dereferencing
49         int caseNum = (t1 != NULL) + 2 * (t2 != NULL) + 4 * (carry != NULL);
50         switch (caseNum) {
51             case 0:  
52             case 1:  
53                 break;
54             case 2: 
55                 H1->list[i] = t2;
56                 H2->list[i] = NULL;
57                 break;
58             case 3: 
59                 carry = lier(t1, t2);
60                 H1->list[i] = H2->list[i] = NULL;
61                 break;
62             case 4: 
63                 H1->list[i] = carry;
64                 carry = NULL;
65                 break;
66             case 5:  
67                 carry = lier(t1, carry);
68                 H1->list[i] = NULL;
69                 break;
70             case 6:  
71                 carry = lier(t2, carry);
72                 H2->list[i] = NULL;
73                 break;
74             case 7:  
75                 H1->list[i] = carry;
76                 carry = lier(t1, t2);
77                 H2->list[i] = NULL;
78                 break;}}
79     H1->size += H2->size;
80     H2->size = 0;
81 }*/
void fb_union(BinHeap H1, BinHeap H2) {
    BinTree carry = NULL;
    for (int i = 0; i < MAX; i++) {
        BinTree t1 = H1->list[i];
        BinTree t2 = (i < MAX) ? H2->list[i] : NULL; // Ensure H2->list[i] is not NULL before dereferencing
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
                break;}}
     H1->size += H2->size;
     H2->size = 0;
}
    
       
void ajout(BinHeap H, int C) {
    BinNode newNode = (BinNode)malloc(sizeof(struct Node));
    newNode->cle = C;
    newNode->degree = 0;
    newNode->paren = NULL;
    newNode->enfant = NULL;
    newNode->frere = NULL;

    BinHeap tempHeap = Init();
    tempHeap->list[0] = newNode;
    


    fb_union(H, tempHeap);
}



int SupprMin(BinHeap H) {
    if (H == NULL || H->size == 0) {
        return -1; // Binomial heap is empty or not initialized
    }

    // Find the tree with the smallest root
    int minIndex = -1;
    int minKey = INT_MAX;
    for (int i = 0; i < MAX; i++) {
        if (H->list[i] && H->list[i]->cle < minKey) {
            minKey = H->list[i]->cle;
            minIndex = i;
        }
    }

    // If there is no tree in the heap, return error
    if (minIndex == -1) {
        return -1;
    }

    // Remove the tree with the smallest root from the heap
    BinNode minTreeRoot = H->list[minIndex];
    H->list[minIndex] = NULL; // Remove the root node

    // Reverse the child list of the removed root to prepare for the union
    BinNode prev = NULL;
    BinNode current = minTreeRoot->enfant;
    while (current != NULL) {
        BinNode next = current->frere;
        current->frere = prev;
        prev = current;
        current = next;
    }
    
    // Create a new heap from the reversed child list
    BinHeap newHeap = Init();
    newHeap->size = (1 << minIndex) - 1;
    int childIndex = 0;
    while (prev != NULL) {
        newHeap->list[childIndex] = prev;
        prev = prev->frere;
        newHeap->list[childIndex]->frere = NULL;
        childIndex++;
    }

    // Merge the new heap with the original heap
    fb_union(H, newHeap);

    // Cleanup
    free(minTreeRoot);

    // Update the size of the original heap
    H->size = H->size - newHeap->size;

    // Return the minimum key
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
    for (int i = 0; i < MAX; i++) {
        if (H->list[i]) {
            printf("Tree of degree %d:\n", i);
            traverse(H->list[i]);
            printf("\n");
        }
    }
}

int main() {
    // Create a new binomial heap
    BinHeap myHeap = Init();

    // Add some elements to the heap
    int elementsToAdd[] = {3, 5, 2, 7, 6, 1, 8};
    int n = sizeof(elementsToAdd) / sizeof(elementsToAdd[0]);
    myHeap = fb_Construction(elementsToAdd, n);
    afficher(myHeap);
    ///*
    // Remove the minimum element from the heap
    int min = SupprMin(myHeap);
    printf("The minimum element was: %d\n", min);
    afficher(myHeap);
//*/

    ajout(myHeap, 4);
    printf("Heap after adding an element:\n");
    afficher(myHeap);
/*
    fb_union(myHeap, myHeap);
    printf("Heap after union:\n");
    afficher(myHeap);
*/
    // Remember to free the memory allocated for the heap
    // This is where you would write your cleanup code


    return 0; // Return success
}
