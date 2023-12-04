#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tas.h"

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
            
            test_Ajout_performance(csv_file, dataset, size);

            test_SupprMin_performance(csv_file, dataset, size);

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