#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cle.h"
#include "tasArbre.h"
#include "liste.h"
#include <time.h>
#define REPEATS 1
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

/*******************************TasArbre**********************************************/

TasArbre *construireTasAPartirDataset(ListeCle *liste, int nbCles) {
    TasArbre *tas = NULL;
    construction(&tas, liste);
    return tas;
}
void test_ajoutsIteratifs(FILE *csv_file, ListeCle *liste, int nbCles) {
    double total_time = 0.0;
    for (int i = 0; i < REPEATS; ++i) {
        double start_time = get_time();
        TasArbre *tas = NULL;
        ajoutsIteratifs(&tas, liste);
        double end_time = get_time();
        total_time += end_time - start_time;
        // tas
        free(tas);
    }
    double average_time = total_time / REPEATS;
    fprintf(csv_file, "TasArbre_AjoutsIteratifs,%d,%.6f\n", nbCles, average_time);
}

void test_construction(FILE *csv_file, ListeCle *liste, int nbCles) {
    double total_time = 0.0;
    for (int i = 0; i < REPEATS; ++i) {
        double start_time = get_time();
        TasArbre *tas = NULL;
        construction(&tas, liste);
        double end_time = get_time();
        total_time += end_time - start_time;
        
        free(tas);
    }
    double average_time = total_time / REPEATS;
    fprintf(csv_file, "TasArbre_Construction,%d,%.6f\n", nbCles, average_time);
}

void test_union(FILE *csv_file, ListeCle *liste, int nbCles) {
    double total_time = 0.0;
    TasArbre* tas1 = construireTasAPartirDataset(liste, nbCles);
    TasArbre* tas2 = construireTasAPartirDataset(liste, nbCles);
    for (int i = 0; i < REPEATS; ++i) {
        double start_time = get_time();
        TasArbre *union_tas = unionTasArbre(tas1, tas2);
        double end_time = get_time();
        total_time += end_time - start_time;

        free(union_tas);
    }
    free(tas1);
    free(tas2);
    double average_time = total_time / REPEATS;
    fprintf(csv_file, "TasArbre_Union,%d,%.6f\n", nbCles, average_time);
}

/*****************************************************************************/
int main() {
    const char *file_format = "cles_alea/jeu_%d_nb_cles_%d.txt";
    int jeux[] = {1, 2, 3, 4, 5};
    int nb_cles[] = {1000, 5000, 10000, 20000, 50000, 80000, 120000, 200000};

    FILE *csv_file_tas_arbre = fopen("performance_tas_arbre_cons.csv", "w");
    if (csv_file_tas_arbre == NULL) {
        perror("Cannot open performance_tas_arbre_cons.csv");
        return 1;
    }
    fprintf(csv_file_tas_arbre, "Function,nbCles,Time\n");

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
            
/****************************************tester la RUN TIME de TasArbre********************************************************************/

            ListeCle *liste = initialisation();
            for (int i = 0; i < nbCles; ++i) {
                insertion(liste, dataset1[i]);
            }
          
            test_construction(csv_file_tas_arbre, liste, nbCles);

            //test_ajoutsIteratifs(csv_file_tas_arbre, liste, nbCles);
            
            //test_union(csv_file_tas_arbre, liste, nbCles);
            

            free(dataset1);
            free(dataset2);
            free(liste);

        
        }
    }

    fclose(csv_file_tas_arbre);
    return 0;
}




