#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cle.h"
#include "tas.h"
#include <mach/mach_time.h>
#define REPEATS 10
#define INITIAL_CAPACITY 1000

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
}



// Ajout : Ajoute un nouvel élément au tas

void Ajout(Tas *tas, uint128_t cle) {
    if (tas->taille >= tas->capacite) {
        int nouvelleCapacite = tas->capacite + (tas->capacite >> 1);  // Augmenter la capacité de 50%
        uint128_t *nouvellesCles = realloc(tas->cles, nouvelleCapacite * sizeof(uint128_t));
        if (!nouvellesCles) {
            perror("Allocation failure in Ajout");
            exit(EXIT_FAILURE);
        }
        tas->cles = nouvellesCles;
        tas->capacite = nouvelleCapacite;
    }
    tas->cles[tas->taille] = cle;
    int i = tas->taille;
    tas->taille++;
    while (i > 0 && inf(tas->cles[i], tas->cles[(i - 1) / 2])) {
        echanger(&tas->cles[i], &tas->cles[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}


// AjoutsIteratifs : Construit un tas en ajoutant itérativement chaque clé
void AjoutsIteratifs(Tas *tas, uint128_t *cles, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        Ajout(tas, cles[i]);
    }
}

// Question 2.5 Construction : Construit un tas à partir d'une liste de clés de manière efficace
/*
pseudo-code :
Fonction Construction(cles, taille):
    tas = nouveau tableau de taille 'taille'

    // Étape 1 : Copie des clés dans le tas
    Pour i de 0 à taille-1:
        tas[i] = cles[i]

    // Étape 2 : Construction du tas
    // On commence par le dernier parent et on descend chaque clé
    Pour i de (taille / 2 - 1) à 0:
        SiftDown(tas, i, taille)

    Retourner tas


Fonction SiftDown(tas, index, tailleMax):
    racine = index
    Tant que racine a au moins un enfant:
        enfant = 2 * racine + 1 // enfant gauche
        swap = racine

        Si enfant < tailleMax et tas[enfant] < tas[swap]:
            swap = enfant
        Si enfant + 1 < tailleMax et tas[enfant + 1] < tas[swap]:
            swap = enfant + 1

        Si swap != racine:
            échanger tas[racine] avec tas[swap]
            racine = swap
        Sinon:
            Quitter la boucle
*/

void Construction(Tas *tas, uint128_t *cles, int taille)
{
    tas->taille = taille;
    tas->cles = malloc(taille * sizeof(uint128_t));
    if (!tas->cles)
    {
        perror("Échec de l'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    memcpy(tas->cles, cles, taille * sizeof(uint128_t));

    // Construire le tas en partant des nœuds à mi-chemin dans l'arbre et en descendant
    for (int i = (taille - 2) / 2; i >= 0; i--)
    {
        SiftDown(tas, i);
    }
}

/*Question 2.6 La fonction Union prend en arguments deux tas ne partageant aucune clé commune,
et construit un tas qui contient l’ensemble de toutes les clés.
Proposer un pseudo-code expliqué afin de garantir la complexité linéaire en la somme des tailles des deux tas, puis l’implémenter.
*/
/*Fonction Union(tas1, tas2):
    tailleUnion = taille de tas1 + taille de tas2
    tasUnion = nouveau tableau de taille 'tailleUnion'

    // Copier les clés de tas1 et tas2 dans tasUnion
    Copier les clés de tas1 dans les premières positions de tasUnion
    Copier les clés de tas2 dans les positions restantes de tasUnion

    // Construire le nouveau tas à partir de la liste fusionnée
    Pour i de (tailleUnion / 2 - 1) à 0:
        SiftDown(tasUnion, i, tailleUnion)

    Retourner tasUnion
*/

bool Union(Tas *tas1, Tas *tas2) {
    int tailleUnion = tas1->taille + tas2->taille;
    
    // 检查容量是否足够
    if (tailleUnion > tas1->capacite) {
        // 扩展容量
        tas1->capacite = tailleUnion;
        uint128_t *new_cles = realloc(tas1->cles, tas1->capacite * sizeof(uint128_t));
        if (!new_cles) {
            perror("Échec de l'allocation mémoire");
            return false;
        }
        tas1->cles = new_cles;
    }

    // 将tas2的元素直接追加到tas1的数组后面
    memcpy(tas1->cles + tas1->taille, tas2->cles, tas2->taille * sizeof(uint128_t));
    tas1->taille = tailleUnion;

    // 重新构建堆
    for (int i = (tailleUnion - 2) / 2; i >= 0; i--) {
        SiftDown(tas1, i);
    }

    return true;
}

/************************************************************/
// Question 2.7 :
bool load_dataset(const char *filename, uint128_t **dataset, int *dataset_size)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return false;
    }

    // mémoire allouée pour le dataset
    *dataset = (uint128_t *)malloc(*dataset_size * sizeof(uint128_t));
    if (!*dataset)
    {
        perror("Memory allocation failed");
        fclose(file);
        return false;
    }

    // Lire le fichier ligne par ligne
    uint128_t key;
    int count = 0;
    while (fscanf(file, "%8x%8x%8x%8x",
                  &key.parts[3], &key.parts[2],
                  &key.parts[1], &key.parts[0]) == 4)
    {
        if (count < *dataset_size)
        {
            (*dataset)[count++] = key;
        }
    }

    fclose(file);
    return true;
}

double get_time(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

// 测试函数性能并将结果写入CSV文件
void test_function_and_write_to_csv(FILE *csv_file, const char *function_name, uint128_t *dataset, int size, void (*function)(Tas *, uint128_t *, int)) {
    double total_time = 0.0;

    for (int i = 0; i < REPEATS; ++i) {
        Tas tas = {malloc(INITIAL_CAPACITY * sizeof(uint128_t)), 0, INITIAL_CAPACITY}; // 预先分配足够的内存

        double start_time = get_time();
        function(&tas, dataset, size);
        double end_time = get_time();

        total_time += end_time - start_time;
        free(tas.cles);  // 立即释放内存
    }

    double average_time = total_time / REPEATS;
    fprintf(csv_file, "%s,%d,%.6f\n", function_name, size, average_time);
}
/*void test_Ajout_performance(FILE *csv_file, uint128_t *dataset, int dataset_size) {
    Tas tas = {malloc(INITIAL_CAPACITY * sizeof(uint128_t)), 0, INITIAL_CAPACITY};
    double total_time = 0.0;

    for (int repeat = 0; repeat < REPEATS; ++repeat) {
        double start_time = get_time();
        for (int i = 0; i < dataset_size; ++i) {
            Ajout(&tas, dataset[i]);
        }
        double end_time = get_time();
        total_time += (end_time - start_time);
        
        // Reset the heap for the next iteration
        tas.taille = 0;
    }
    free(tas.cles);
    double average_time = total_time / REPEATS;
    fprintf(csv_file, "Ajout,%d,%.6f\n", dataset_size, average_time);
}*/

void test_Ajout_performance(FILE *csv_file, uint128_t *dataset, int dataset_size) {
    Tas tas = {malloc(INITIAL_CAPACITY * sizeof(uint128_t)), 0, INITIAL_CAPACITY};
    double total_time = 0.0;

    for (int repeat = 0; repeat < REPEATS; ++repeat) {
        double start_time = get_time();
        for (int i = 0; i < dataset_size; ++i) {
            Ajout(&tas, dataset[i]);
        }
        double end_time = get_time();
        total_time += (end_time - start_time);
        // 清空堆，以便下一次重复测试
        tas.taille = 0;
    }

    free(tas.cles);
    double average_time = total_time / REPEATS;
    fprintf(csv_file, "Ajout,%d,%.6f\n", dataset_size, average_time);
}



void test_SupprMin_performance(FILE *csv_file, uint128_t *dataset, int dataset_size) {
    double total_time = 0.0;

    for (int repeat = 0; repeat < REPEATS; ++repeat) {
        Tas tas = {malloc(INITIAL_CAPACITY * sizeof(uint128_t)), 0, INITIAL_CAPACITY};

        // 先将所有元素添加到堆中，但不包括在计时中
        for (int i = 0; i < dataset_size; ++i) {
            Ajout(&tas, dataset[i]);
        }

        double start_time = get_time();
        while (tas.taille > 0) {
            SupprMin(&tas);
        }
        double end_time = get_time();

        total_time += (end_time - start_time);
        free(tas.cles);
    }

    double average_time = total_time / REPEATS;
    fprintf(csv_file, "SupprMin,%d,%.6f\n", dataset_size, average_time);
}
