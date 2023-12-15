#include "cle.h"
#include "md5.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    const char *message = "Hello World!";
    uint128_t *res = (uint128_t *)malloc(sizeof(uint128_t));
    if (res == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        return 1;
    }
    md5(message, res);
    printf("md5 : %08x%08x%08x%08x\n", res->parts[0], res->parts[1], res->parts[2], res->parts[3]);

    free(res);
    return 0;
}