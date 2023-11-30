#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define REPEATS 10
/************************************************************
 *                                                           *
 *                                                           *
 *                       1 Présentation                      *
 *                                                           *
 *                                                           *
 *************************************************************/

/****************************************
 *                                       *
 *       1.1 : Échauffement              *
 *                                       *
 *****************************************/

// Question 1.1 Définition d'un type pour représenter un entier de 128 bits
typedef struct
{
    // Tableau de 4 entiers de 32 bits
    uint32_t parts[4];
} uint128_t;

// Initialisation d'un entier de 128 bits
void init_uint128(uint128_t *key, uint32_t part0, uint32_t part1, uint32_t part2, uint32_t part3)
{
    key->parts[0] = part0;
    key->parts[1] = part1;
    key->parts[2] = part2;
    key->parts[3] = part3;
}

// Affichage d'un entier de 128 bits
void print_uint128(const uint128_t *key)
{
    printf("128-bit key representation:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("Part %d: %u\n", i, key->parts[i]);
    }
}

// Question 1.2 déterminer si cle1 est strictement inférieure à cle2
bool inf(const uint128_t cle1, const uint128_t cle2)
{
    for (int i = 0; i < 4; i++)
    {
        if (cle1.parts[i] < cle2.parts[i])
        {
            // printf("cle1 is less than cle2\n");
            return true;
        }
        else if (cle1.parts[i] > cle2.parts[i])
        {
            // printf("cle1 is greater than cle2\n");
            return false;
        }
    }
    return false;
}

// Question 1.3 déterminer si cle1 est égale à cle2
bool eg(const uint128_t cle1, const uint128_t cle2)
{
    for (int i = 0; i < 4; i++)
    {
        if (cle1.parts[i] != cle2.parts[i])
        {
            // printf("cle1 is not equal to cle2\n");
            return false;
        }
    }
    // printf("cle1 is equal to cle2\n");
    return true;
}

// process_file() :  lire le fichier et afficher les clés
bool process_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return false;
    }

    // Read file line by line
    char line[40];
    uint128_t key;

    while (fgets(line, sizeof(line), file))
    {

        if (sscanf(line, "%8x%8x%8x%8x",
                   &key.parts[0],
                   &key.parts[1],
                   &key.parts[2],
                   &key.parts[3]) == 4)
        {
            //
            print_uint128(&key);
            //
        }
        else
        {
            fprintf(stderr, "Parsing error: %s", line);
        }
    }

    fclose(file);
    return true;
}

/************************************************************
 *                                                           *
 *                                                           *
 *           2 Structure 1 : Tas priorité min                *
 *                                                           *
 *                                                           *
 *************************************************************/
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
void echanger(uint128_t *a, uint128_t *b)
{
    uint128_t temp = *a;
    *a = *b;
    *b = temp;
}

// Fonction pour restaurer les propriétés d'un tas min après la suppression du min
void SiftDown(Tas *tas, int index)
{
    int gauche = 2 * index + 1;
    int droite = 2 * index + 2;
    int plusPetit = index;

    if (gauche < tas->taille && inf(tas->cles[gauche], tas->cles[index]))
    {
        plusPetit = gauche;
    }
    if (droite < tas->taille && inf(tas->cles[droite], tas->cles[plusPetit]))
    {
        plusPetit = droite;
    }
    if (plusPetit != index)
    {
        echanger(&tas->cles[index], &tas->cles[plusPetit]);
        SiftDown(tas, plusPetit);
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
void Ajout(Tas *tas, uint128_t cle)
{
    // Augmenter la taille du tableau
    tas->taille++;
    tas->cles = realloc(tas->cles, tas->taille * sizeof(uint128_t));
    if (!tas->cles)
    {
        perror("Échec de l'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Ajouter la nouvelle clé à la fin et remonter
    int i = tas->taille - 1;
    tas->cles[i] = cle;
    while (i > 0 && inf(tas->cles[i], tas->cles[(i - 1) / 2]))
    {
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
bool Union(Tas *tas1, Tas *tas2)
{
    int tailleUnion = tas1->taille + tas2->taille;
    uint128_t *clesUnion = malloc(tailleUnion * sizeof(uint128_t));
    if (!clesUnion)
    {
        perror("Échec de l'allocation mémoire");
        return false;
    }

    memcpy(clesUnion, tas1->cles, tas1->taille * sizeof(uint128_t));
    memcpy(clesUnion + tas1->taille, tas2->cles, tas2->taille * sizeof(uint128_t));

    free(tas1->cles); // 释放旧的内存
    tas1->cles = clesUnion;
    tas1->taille = tailleUnion;

    Construction(tas1, clesUnion, tailleUnion);

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
                  &key.parts[0], &key.parts[1],
                  &key.parts[2], &key.parts[3]) == 4)
    {
        if (count < *dataset_size)
        {
            (*dataset)[count++] = key;
        }
    }

    fclose(file);
    return true;
}

double get_time()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}
// 测试函数性能并将结果写入CSV文件
void test_function_and_write_to_csv(FILE *csv_file, const char *function_name, uint128_t *dataset, int size, void (*function)(Tas *, uint128_t *, int))
{
    double start_time, end_time, total_time = 0.0;

    for (int i = 0; i < REPEATS; ++i)
    {
        Tas tas = {NULL, 0};
        start_time = get_time();
        function(&tas, dataset, size);
        end_time = get_time();
        total_time += end_time - start_time;
        free(tas.cles);  // 释放内存
        tas.cles = NULL; // 防止悬空指针
    }

    double average_time = total_time / REPEATS;
    fprintf(csv_file, "%s,%d,%.6f\n", function_name, size, average_time);
}

int main()
{
    const char *file_format = "cles_alea/jeu_%d_nb_cles_%d.txt";
    int jeux[] = {1, 2, 3, 4, 5};
    int nb_cles[] = {1000, 5000, 10000, 20000, 50000, 80000, 120000, 200000};
    FILE *csv_file = fopen("performance.csv", "w");

    if (csv_file == NULL)
    {
        perror("Cannot open CSV file");
        return 1;
    }
    fprintf(csv_file, "Function,Size,Time\n");

    // 对每个数据集大小和每个jeu索引运行测试
    for (int i = 0; i < sizeof(nb_cles) / sizeof(nb_cles[0]); ++i)
    {
        for (int j = 0; j < sizeof(jeux) / sizeof(jeux[0]); ++j)
        {
            int size = nb_cles[i];
            uint128_t *dataset = NULL; // 初始化数据集指针为NULL
            char filename[256];
            sprintf(filename, file_format, jeux[j], size);

            // 使用load_dataset函数加载数据集
            if (!load_dataset(filename, &dataset, &size))
            {
                fprintf(stderr, "Failed to load dataset from file: %s\n", filename);
                continue; // 如果无法加载数据集则跳过这个数据集
            }

            // 初始化Tas结构
            Tas tas = {NULL, 0}; // 初始化tas结构的指针为NULL，大小为0

            // 进行测试并将结果写入CSV文件
            test_function_and_write_to_csv(csv_file, "AjoutsIteratifs", dataset, size, AjoutsIteratifs);
            test_function_and_write_to_csv(csv_file, "Construction", dataset, size, Construction);

            uint128_t *dataset_copy = malloc(size * sizeof(uint128_t));
            if (dataset_copy == NULL)
            {
                free(dataset); // 在退出之前释放dataset
                fprintf(stderr, "Memory allocation failed for dataset_copy\n");
                continue;
            }
            memcpy(dataset_copy, dataset, size * sizeof(uint128_t));
            Tas tas1 = {dataset, size};
            Tas tas2 = {dataset_copy, size};

            double start_time = get_time();
            if (!Union(&tas1, &tas2))
            {
                free(dataset);
                free(dataset_copy);
                fprintf(stderr, "Union failed\n");
                continue;
            }
            double end_time = get_time();
            fprintf(csv_file, "Union,%d,%.6f\n", size, end_time - start_time);

            free(tas1.cles);    // 释放由 Union 分配的内存
            free(dataset_copy); // 释放 dataset_copy
        }
    }

    fclose(csv_file);
    return 0;
}

