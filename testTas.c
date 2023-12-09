#include <stdio.h>
#include <stdlib.h>
#include "cle.h"
#include "tas.h"
#define INITIAL_CAPACITY 1000  //capacité initiale du tas

int main() {
    
    Tas tas1 = init_tas();
    uint128_t test_Cles[6] = {
        {0xdf6943ba, 0x6d51464f, 0x6b021579, 0x33bdd9ad},
        {0xd192acf4, 0xc06fe7c7, 0xdf042f07, 0xd290bdd4},
        {0x2c15aed1, 0xa9eab933, 0x38d0348f, 0x12ef9a3b},
        {0x5f003a25, 0x87337655, 0xaf8a166b, 0xe8439a49},
        {0x1573c8d1, 0x56d03e63, 0x3c20c36f, 0x1b70862},
        
        {0xdf5d8018, 0xd0af5d1a, 0x979d449c, 0x91282bfc}       
    };

    printf("\n\n————Testing Construction...\n");
    int nbCles1 = sizeof(test_Cles) / sizeof(test_Cles[0]);
    tas1 = Construction(&tas1, test_Cles, nbCles1);
    printf("\n————Heap constructed with %d Clestest_Cles.\n", nbCles1);
    printf("\n————Heap after Construction:\n");
    print_tas(&tas1);

    printf("\n\n————testing Ajout...\n");
    printf("\n————Ajouter les Cles:\n");
    uint128_t Cle_Ajout = {0x1233be51,0x326a1b2b,0x9cc45caa,0x867d053b};
    Ajout(&tas1, Cle_Ajout);
    printf("\n————tas apres Ajout:\n");
    print_tas(&tas1);

    printf("\n\n————Testing SupprMin...\n");
    SupprMin(&tas1);
    printf("\n————Heap after SupprMin:\n");
    print_tas(&tas1);

    
    printf("\n\n————Testing AjoutsIteratifs...\n");
    uint128_t test_Cles2_AjoutIte[2] = {
        {0x1233be51,0x326a1b2b,0x9cc45caa,0x867d053b},
        {0xd0021c8d,0xa304b898,0xc87101ae,0xbbce50d1}    
    };  
    int nbCles2 = sizeof(test_Cles2_AjoutIte) / sizeof(test_Cles2_AjoutIte[0]);
    printf("\n————Added %d Cles iteratively.\n", nbCles2);
    AjoutsIteratifs(&tas1, test_Cles2_AjoutIte, nbCles2);
    printf("\n————Heap after AjoutsIteratifs:\n");
    print_tas(&tas1);
    
    printf("\n\n————Testing Union...\n");
    Tas tas2 = init_tas();
    uint128_t test_Cles2[3] = {
        {0x76f3be51, 0x426a9b2a, 0x1cc45caa, 0x967e153b},
        {0xe0029c8d, 0xb304b888, 0xd87191ae, 0xabce70d2},
        {0x823fbe52, 0x326a8b2b, 0x2cc55cab, 0x567d953c}  
    };
    int nbCles3 = sizeof(test_Cles2) / sizeof(test_Cles2[0]);
    tas2 = Construction(&tas2, test_Cles2, nbCles3);
    Tas tas3 = Union(&tas1, &tas2);
    printf("\n————Union tas1 et tas2\n");
    print_tas(&tas3);

    free(tas1.cles);

    return 0;
}
