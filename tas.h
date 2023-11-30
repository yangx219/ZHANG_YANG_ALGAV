#ifndef TAS_H
#define TAS_H

#include <stdio.h>
#include "cle.h"

/*
En cours, nous avons introduit la structure de données de tas min. Nous allons la représenter en
mémoire avec deux structures distinctes : via un arbre binaire et via un tableau.
Question 2.4 Implémenter les 3 fonctions fondamentales d’un tas min : SupprMin, Ajout, AjoutsIteratifs.
Ces fonctions permettent respectivement de supprimer l’élément de clé minimale de la structure, d’ajouter un élément à un tas.
La fonction AjoutsIteratifs prend en argument une liste de clés et construit le tas correspondant par ajouts simples dans un tas (on fait appel à Ajout).
*/
// Question 2.4
//  Structure de tas minimum avec un arbre binaire
typedef struct Noeud
{
    uint128_t cle; // Clé de l'élément
    struct Noeud *gauche, *droite;
} Noeud;
// Structure de tas minimum avec un tableau
typedef struct
{
    uint128_t *cles; // Tableau des clés
    int taille;      // Nombre d'éléments dans le tas
} Tas;

// Fonction pour échanger deux éléments uint128_t
void echanger(uint128_t *a, uint128_t *b);

// Fonction pour restaurer les propriétés d'un tas min après la suppression du min
void SiftDown(Tas *tas, int index);

// SupprMin : Supprime l'élément minimal du tas
void SupprMin(Tas *tas);

// Ajout : Ajoute un nouvel élément au tas
void Ajout(Tas *tas, uint128_t cle);

// AjoutsIteratifs : Construit un tas en ajoutant itérativement chaque clé
void AjoutsIteratifs(Tas *tas, uint128_t *cles, int taille);

// Question 2.5 Construction : Construit un tas à partir d'une liste de clés de manière efficace
void Construction(Tas *tas, uint128_t *cles, int taille);

// Question 2.6 La fonction Union prend en arguments deux tas ne partageant aucune clé commune,
// et construit un tas qui contient l’ensemble de toutes les clés.
bool Union(Tas *tas1, Tas *tas2);

// Question 2.7 :
bool load_dataset(const char *filename, uint128_t **dataset, int *dataset_size);

double get_time();

// 测试函数性能并将结果写入CSV文件
void test_function_and_write_to_csv(FILE *csv_file, const char *function_name, uint128_t *dataset, int size, void (*function)(Tas *, uint128_t *, int));

#endif