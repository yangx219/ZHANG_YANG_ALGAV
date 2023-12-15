#include "filesBinomiales.h"
#include "cle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>


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
    char line[40];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "0x%8x%8x%8x%8x", 
            &key.parts[0], &key.parts[1], &key.parts[2], &key.parts[3]);
    
        //printf("Read key: %08x%08x%08x%08x\n", key.parts[0], key.parts[1], key.parts[2], key.parts[3]);   
        if (count < *dataset_size)
        {
            (*dataset)[count++] = key;
        }
    }

    fclose(file);
    return true;
}
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void test_construction_performance(FILE *csv_file, uint128_t *elements, int n) {
    double start_time = get_time();
    BinHeap heap = fb_Construction(elements, n);
    double end_time = get_time();
    fprintf(csv_file, "Construction,%d,%.6f\n", n, end_time - start_time);
    free(heap);
}

void test_union_performance(FILE *csv_file, uint128_t *elements1, uint128_t *elements2, int nbcle1, int nbcle2) {
    BinHeap heap1 = fb_Construction(elements1, nbcle1);
    // printf("heap1 size: %d\n", heap1->size);
    // afficher(heap1);
    
    BinHeap heap2 = fb_Construction(elements2, nbcle2);
    // afficher(heap2);
    // printf("heap2 size: %d\n", heap2->size);

    double start_time = get_time();
    fb_union(heap1, heap2);
    double end_time = get_time();
    // printf("fb_union size: %d\n", heap1->size);
    fprintf(csv_file, "Union,%d,%.6f\n", nbcle1, end_time - start_time);
    free(heap1);
}

/*
int main() {
    const char *file_format = "cles_alea/jeu_%d_nb_cles_%d.txt";
    int jeux[] = {1, 2, 3, 4, 5};
    int nb_cles[] = {1000, 5000, 10000, 20000, 50000, 80000, 120000, 200000};

    FILE *csv_file = fopen("performance_files_binomiales.csv", "w");
    if (csv_file == NULL) {
        perror("Cannot open performance_files_binomiales.csv");
        return 1;
    }
    fprintf(csv_file, "Function,nbCles,Time\n");

    for (int i = 0; i < sizeof(nb_cles) / sizeof(nb_cles[0]); ++i) {
        for (int j = 0; j < sizeof(jeux) / sizeof(jeux[0]); ++j) {
            int nbCles = nb_cles[i];
            
            uint128_t *dataset1 = NULL;
            uint128_t *dataset2 = NULL;
            char filename1[256];
            char filename2[256];
            sprintf(filename1, file_format, jeux[j], nbCles);
            sprintf(filename2, file_format, (jeux[j] % 5) + 1, nbCles); //différents jeux de clés

            if (!load_dataset(filename1, &dataset1, &nbCles)) {
                fprintf(stderr, "Failed to load dataset from file: %s\n", filename1);
                continue;
            }
            if (!load_dataset(filename2, &dataset2, &nbCles)) {
                fprintf(stderr, "Failed to load dataset from file: %s\n", filename2);
                free(dataset1); // Make sure to free the first dataset if the second fails
                continue;
            }

            // 测试 fb_Construction
            test_construction_performance(csv_file, dataset1, nbCles);

            // 测试 fb_union
            test_union_performance(csv_file, dataset1, dataset2, nbCles);

            free(dataset1);
            free(dataset2);
        }
    }

    fclose(csv_file);
    return 0;
}
*/


int main() {
    const char *file_format = "cles_alea/jeu_%d_nb_cles_%d.txt";
    int jeux[] = {1, 2, 3, 4, 5};
    int nb_cles[] = {1000, 5000, 10000, 20000, 50000, 80000, 120000, 200000};

    FILE *csv_file = fopen("performance_files_binomiales.csv", "w");
    if (csv_file == NULL) {
        perror("Cannot open performance_files_binomiales.csv");
        return 1;
    }
    fprintf(csv_file, "Function,nbCles,Time\n");

    for (int i = 0; i < sizeof(nb_cles) / sizeof(nb_cles[0]); ++i) {
        for (int j = 0; j < sizeof(jeux) / sizeof(jeux[0]); ++j) {
            int nbCles1 = nb_cles[i];
            int nbCles2 = (nb_cles[i%5+1]); // 第二个数据集大小为第一个的一半

            uint128_t *dataset1 = NULL;
            uint128_t *dataset2 = NULL;
            char filename1[256];
            char filename2[256];
            sprintf(filename1, file_format, jeux[j], nbCles1);
            sprintf(filename2, file_format, (jeux[j] % 5) + 1, nbCles2); // 使用不同的文件

            if (!load_dataset(filename1, &dataset1, &nbCles1)) {
                fprintf(stderr, "Failed to load dataset from file: %s\n", filename1);
                continue;
            }
            if (!load_dataset(filename2, &dataset2, &nbCles2)) {
                fprintf(stderr, "Failed to load dataset from file: %s\n", filename2);
                free(dataset1); // 释放第一个数据集的内存
                continue;
            }

            // 测试 fb_Construction
            test_construction_performance(csv_file, dataset1, nbCles1);

            // 测试 fb_union
            test_union_performance(csv_file, dataset1, dataset2, nbCles1, nbCles2);

            free(dataset1);
            free(dataset2);
        }
    }

    fclose(csv_file);
    return 0;
}