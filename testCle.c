#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cle.h"

int main() {
    const char* cles_alea = "cles_alea/jeu_%d_nb_cles_%d.txt";
    int jeu[] = {1, 2, 3, 4, 5};
    int nb_cles[] = {1000, 5000, 10000, 20000, 50000, 80000, 120000, 200000};

    for (int i = 0; i < sizeof(jeu) / sizeof(jeu[0]); ++i) {
        for (int j = 0; j < sizeof(nb_cles) / sizeof(nb_cles[0]); ++j) {
            char filename[512];
            sprintf(filename, cles_alea, jeu[i], nb_cles[j]);
            if (!process_file(filename)) {
                fprintf(stderr, "Failed to process file: %s\n", filename);
            }
        }
    }
    return 0;
}