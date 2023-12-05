#include <stdio.h>
#include <stdlib.h>
#include "cle.h"
#include "tas.h"

#define NUM_KEYS 10  // 测试数据的数量
#define INITIAL_CAPACITY 1000  // 初始堆容量

int main() {
    // 初始化堆
    Tas tas;
    tas.cles = malloc(INITIAL_CAPACITY * sizeof(uint128_t));
    tas.taille = 0;
    tas.capacite = INITIAL_CAPACITY;

    // 初始化测试数据
    uint128_t test_keys[NUM_KEYS] = {
        {0xdf6943ba, 0x6d51464f, 0x6b021579, 0x33bdd9ad},
        {0xd192acf4, 0xc06fe7c7, 0xdf042f07, 0xd290bdd4},
        {0x2c15aed1, 0xa9eab933, 0x38d0348f, 0x12ef9a3b},
        {0x5f003a25, 0x87337655, 0xaf8a166b, 0xe8439a49},
        {0x1573c8d1, 0x56d03e63, 0x3c20c36f, 0x1b70862},
        {0x298a6eed, 0xbec66315, 0x79f09930, 0xfcf8e175},
        {0x7382113a, 0xce03ffb8, 0xab154f6b, 0xb902a34},
        {0xdcef5251, 0xdb8481cb, 0xc6612b35, 0x54884c2b},
        {0xe38d4e6d, 0x00cbd911, 0xebf4f1d7, 0x7f1584de},
        {0xdf5d8018, 0xd0af5d1a, 0x979d449c, 0x91282bfc}
        // ... 其他测试数据，根据您提供的列表填充
    };

    // Ajout 测试
    printf("Testing Ajout...\n");
    for (int i = 0; i < NUM_KEYS; ++i) {
        Ajout(&tas, test_keys[i]);
        printf("Added key %d\n", i + 1);
    }

    // 打印添加后的堆状态
    printf("\nHeap after Ajout:\n");
    for (int i = 0; i < tas.taille; ++i) {
        print_uint128(&tas.cles[i]);
    }

    // SupprMin 测试
    printf("\nTesting SupprMin...\n");
    SupprMin(&tas);
    printf("Removed the minimum key.\n");

    // 打印删除后的堆状态
    printf("\nHeap after SupprMin:\n");
    for (int i = 0; i < tas.taille; ++i) {
        print_uint128(&tas.cles[i]);
    }

    // AjoutsIteratifs 测试
    printf("\nTesting AjoutsIteratifs...\n");
    AjoutsIteratifs(&tas, test_keys, NUM_KEYS);
    printf("Added %d keys iteratively.\n", NUM_KEYS);

    // 打印迭代添加后的堆状态
    printf("\nHeap after AjoutsIteratifs:\n");
    for (int i = 0; i < tas.taille; ++i) {
        print_uint128(&tas.cles[i]);
    }

    // Construction 测试
    printf("\nTesting Construction...\n");
    Construction(&tas, test_keys, NUM_KEYS);
    printf("Heap constructed with %d keys.\n", NUM_KEYS);

    // 打印构造后的堆状态
    printf("\nHeap after Construction:\n");
    for (int i = 0; i < tas.taille; ++i) {
        print_uint128(&tas.cles[i]);
    }


    // Union 测试
    printf("\nTesting Union...\n");
    Tas tas2;
    tas2.cles = malloc(INITIAL_CAPACITY * sizeof(uint128_t));
    tas2.taille = 0;
    tas2.capacite = INITIAL_CAPACITY;
    // 假设 tas2 已填充部分测试数据
    Union(&tas, &tas2);
    printf("Union performed.\n");

    // 打印合并后的堆状态
    printf("\nHeap after Union:\n");
    for (int i = 0; i < tas.taille; ++i) {
        print_uint128(&tas.cles[i]);
    }

    // 清理资源

    free(tas2.cles);

    return 0;
}
