#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tasTableau.h"
#include "cle.h"
#include "tasArbre.h"
#include "liste.h"
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
/****************************TasTableau*************************************************/
//rend les construction de tas disponibles pour les tests
void constructionAdapter(Tas *tas, uint128_t *cles, int nbCles) {

     if (tas->cles != NULL) {
        free(tas->cles);
        tas->cles = NULL;
    }
    Tas new_tas = Construction(tas, cles, nbCles);
    *tas = new_tas;
}

/*****************************************************************************/
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

/*****************************************************************************/
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


/*****************************************************************************/
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



/*****************************************************************************/
// tester la fonction Union et écrire les résultats dans un fichier CSV
void test_Union_performance(FILE *csv_file, Tas *tas1, Tas *tas2, int nbCles) {
   // printf("Before Union: tas1 taille = %d, tas2 taille = %d\n", tas1->taille, tas2->taille);
    double start_time = get_time();
    Tas tas_union = Union(tas1, tas2);
    double end_time = get_time();
   // printf("After Union: tas_union taille = %d\n", tas_union.taille);

    fprintf(csv_file, "Union,%d,%.6f\n", nbCles, end_time - start_time);
    free(tas_union.cles);
}





/*****************************************************************************/
int main() {
    const char *file_format = "cles_alea/jeu_%d_nb_cles_%d.txt";
    int jeux[] = {1, 2, 3, 4, 5};
    int nb_cles[] = {1000, 5000, 10000, 20000, 50000, 80000, 120000, 200000};
    
    FILE *csv_file_tas_tableau = fopen("performance_tas_tableau.csv", "w");
    if (csv_file_tas_tableau == NULL) {
        perror("Cannot open performance_tas_tableau.csv");
        return 1;
    }
    fprintf(csv_file_tas_tableau, "Function,nbCles,Time\n");

    for (int i = 0; i < sizeof(nb_cles) / sizeof(nb_cles[0]); ++i) {
        for (int j = 0; j < sizeof(jeux) / sizeof(jeux[0]); ++j) {
            int nbCles = nb_cles[i];
            uint128_t *dataset1 = NULL;
            uint128_t *dataset2 = NULL;
            char filename[256];
            sprintf(filename, file_format, jeux[j], nbCles);

            if (!load_dataset(filename, &dataset1, &nbCles)) {
                fprintf(stderr, "Failed to load dataset from file: %s\n", filename);
                continue;
            }
            /********************tester la fonction AjoutsIteratifs et Construction************************/ 
            test_function_and_write_to_csv(csv_file_tas_tableau, "AjoutsIteratifs", dataset1, nbCles, AjoutsIteratifs);
        
            test_function_and_write_to_csv(csv_file_tas_tableau, "Construction", dataset1, nbCles, constructionAdapter);

            //test_SupprMin_performance(csv_file, dataset, nbCles);


        
            sprintf(filename, file_format, (jeux[j] % 5) + 1, nbCles);
            if (!load_dataset(filename, &dataset2, &nbCles)) {
                fprintf(stderr, "Failed to load second dataset from file: %s\n", filename);
                free(dataset1);
                continue;
            }
            Tas tas1 = init_tas();
            tas1 =Construction(&tas1, dataset1, nbCles);
            Tas tas2 = init_tas();
            tas2 =Construction(&tas2, dataset2, nbCles);


            /********************tester la fonction Union************************/
            test_Union_performance(csv_file_tas_tableau, &tas1, &tas2, nbCles);
            
            

            free(dataset1);
            free(dataset2);

        
        }
    }
    fclose(csv_file_tas_tableau);
    return 0;
}




