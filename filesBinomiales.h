#ifndef filesBinomiales_H
#define filesBinomiales_H


// Binomial Tree Node     
struct Node {
    int cle;  
    int degree;  
    struct Node *parent;
    struct Node *enfant;
    struct Node *frere;
};
typedef struct Node * BinNode;
// Binomial Heap
struct Heap {
    int size;
    int capacity; 
    BinNode *list;
};

typedef struct Heap * BinHeap;
typedef struct Node * BinTree;

//lier deaux tas binomiaux de même degré
BinNode lier(BinNode tree1, BinNode tree2);
//initialiser un file binomiale
BinHeap Init();
//étendre la capacité du file binomiale
void expandHeap(BinHeap H);
//union de deux tas binomiaux
void fb_union(BinHeap H1, BinHeap H2) ;
//ajouter un élément au file binomial 
void ajout(BinHeap H, int C);
//supprimer l'élément minimum du tas binomial
int SupprMin(BinHeap H);
// Construction d'un file binomial à partir d'un tableau d'éléments
BinHeap fb_Construction(int *elements, int n);

//affichage d'un file binomiale
void afficher(BinHeap H);
//traverser un tas binomial
void traverse(BinTree L);

#endif // filesBinomiales_H