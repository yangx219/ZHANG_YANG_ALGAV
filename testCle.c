#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cle.h"

int main() {
    uint128_t test_Cles[6] = {
        {0xdf6943ba, 0x6d51464f, 0x6b021579, 0x33bdd9ad},
        {0xd192acf4, 0xc06fe7c7, 0xdf042f07, 0xd290bdd4},
        {0x2c15aed1, 0xa9eab933, 0x38d0348f, 0x12ef9a3b},
        {0x5f003a25, 0x87337655, 0xaf8a166b, 0xe8439a49},
        {0x1573c8d1, 0x56d03e63, 0x3c20c36f, 0x1b70862},
        
        {0xdf5d8018, 0xd0af5d1a, 0x979d449c, 0x91282bfc}       
    };
    for (int i = 0; i < 6; i++){
        printf("Cle[%d]: ", i);
        print_uint128(&test_Cles[i]);
        printf("\n");
    }
    
    printf("\n ——tetster inf:\n");
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++) {
            printf("Cle[%d] inf Cle[%d]: %d\n", i, j, inf(test_Cles[i], test_Cles[j]));
        }
    }

    printf("\n ——tetster eg:\n");
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++) {
            printf("Cle[%d] eg Cle[%d]: %d\n", i, j, eg(test_Cles[i], test_Cles[j]));
        }
    }
    return 0;
}