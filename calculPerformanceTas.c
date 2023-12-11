#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tas.h"
#include "cle.h"
#include "tasArbre.h"
#include <time.h>
#define REPEATS 10
#define INITIAL_CAPACITY 1000

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
    return (double)clock() / CLOCKS_PER_SEC;
}

//rend les construction de tas disponibles pour les tests
void constructionAdapter(Tas *tas, uint128_t *cles, int nbCles) {

     if (tas->cles != NULL) {
        free(tas->cles);
        tas->cles = NULL;
    }
    Tas new_tas = Construction(tas, cles, nbCles);
    *tas = new_tas;
}


// tester une fonction et écrire les résultats dans un fichier CSV
void test_function_and_write_to_csv(FILE *csv_file, const char *function_name, uint128_t *dataset, int nbCles, void (*function)(Tas *, uint128_t *, int)) {
    double total_time = 0.0;

    for (int i = 0; i < REPEATS; ++i) {
        Tas tas = init_tas(); 

        double start_time = get_time();
        function(&tas, dataset, nbCles);
        double end_time = get_time();

        total_time += end_time - start_time;
        free(tas.cles); 
    }

    double average_time = total_time / REPEATS;
    fprintf(csv_file, "%s,%d,%.6f\n", function_name, nbCles, average_time);
}

// tester la fonction Ajout et écrire les résultats dans un fichier CSV
void test_Ajout_performance(FILE *csv_file, uint128_t *dataset, int dataset_size) {
    double total_time = 0.0;

    for (int repeat = 0; repeat < REPEATS; ++repeat) {
        Tas tas = init_tas();  

        double start_time = get_time();
        for (int i = 0; i < dataset_size; ++i) {
            Ajout(&tas, dataset[i]);
        }
        double end_time = get_time();

        total_time += (end_time - start_time);
        free(tas.cles);  

    double average_time = total_time / REPEATS;
    fprintf(csv_file, "Ajout,%d,%.6f\n", dataset_size, average_time);
}
}
// tester la fonction SupprMin et écrire les résultats dans un fichier CSV
void test_SupprMin_performance(FILE *csv_file, uint128_t *dataset, int dataset_size) {
    double total_time = 0.0;

    for (int repeat = 0; repeat < REPEATS; ++repeat) {
        Tas tas = init_tas();  

        // Ajouter toutes les clés au tas
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



// 测试Union函数并记录性能
void test_Union_performance(FILE *csv_file, Tas *tas1, Tas *tas2, int nbCles) {
   // printf("Before Union: tas1 taille = %d, tas2 taille = %d\n", tas1->taille, tas2->taille);
    double start_time = get_time();
    Tas tas_union = Union(tas1, tas2);
    double end_time = get_time();
   // printf("After Union: tas_union taille = %d\n", tas_union.taille);

    fprintf(csv_file, "Union,%d,%.6f\n", nbCles, end_time - start_time);
    free(tas_union.cles);
}


// 测试函数 - 使用数据集创建堆并打印其内容
void test_with_dataset(const char *filename) {
    int dataset_size = 100; // 假设数据集大小
    uint128_t *dataset = NULL;

    if (!load_dataset(filename, &dataset, &dataset_size)) {
        fprintf(stderr, "Failed to load dataset from file: %s\n", filename);
        return;
    }

    // 使用数据集创建堆
    Tas tas = init_tas();
    tas = Construction(&tas, dataset, dataset_size);

    

    // 执行其他操作，例如添加或删除最小元素

    // 释放资源
    free(tas.cles);
    free(dataset);
}





int main() {
    const char *file_format = "cles_alea/jeu_%d_nb_cles_%d.txt";
    int jeux[] = {1, 2, 3, 4, 5};
    int nb_cles[] = {1000, 5000, 10000, 20000, 50000, 80000, 120000, 200000};
    FILE *csv_file = fopen("performance.csv", "w");

    if (csv_file == NULL) {
        perror("Cannot open CSV file");
        return 1;
    }
    fprintf(csv_file, "Function,nbCles,Time\n");

    for (int i = 0; i < sizeof(nb_cles) / sizeof(nb_cles[0]); ++i) {
        for (int j = 0; j < sizeof(jeux) / sizeof(jeux[0]); ++j) {
            int nbCles = nb_cles[i];
            uint128_t *dataset = NULL;
            char filename[256];
            sprintf(filename, file_format, jeux[j], nbCles);

            if (!load_dataset(filename, &dataset, &nbCles)) {
                fprintf(stderr, "Failed to load dataset from file: %s\n", filename);
                continue;
            }
            //tester la fonction 
            test_function_and_write_to_csv(csv_file, "AjoutsIteratifs", dataset, nbCles, AjoutsIteratifs);
        
            test_function_and_write_to_csv(csv_file, "Construction", dataset, nbCles, constructionAdapter);

            //test_SupprMin_performance(csv_file, dataset, nbCles);

            uint128_t *dataset_copy = malloc(nbCles * sizeof(uint128_t));
            if (dataset_copy == NULL) {
                free(dataset);
                fprintf(stderr, "Memory allocation failed for dataset_copy\n");
                continue;
            }
            
            Tas tas1 = init_tas();
            tas1 =Construction(&tas1, dataset, nbCles);
            Tas tas2 = init_tas();
            tas2 =Construction(&tas2, dataset, nbCles);
            // tester la fonction Union
            test_Union_performance(csv_file, &tas1, &tas2, nbCles);
            
            free(dataset_copy);
            free(dataset);
            
        }
    }

    fclose(csv_file);
    return 0;
}

