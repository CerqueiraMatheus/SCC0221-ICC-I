#include <stdio.h>
#include <stdlib.h>

int main() {
    /**
     * Além das variáveis indicadas na entrada,
     * utilizei "position" para indicar a posição
     * a ser somada.
     * */
    int bar_amount, data_amount, position, *bar;

    bar = (int *)malloc(1 * sizeof(int));

    scanf("%d", &bar_amount);
    scanf("%d", &data_amount);

    bar = realloc(bar, bar_amount * sizeof(int));

    //Percorre unitariamente a entrada de dados
    for (int i = 0; i < data_amount; i++) {
        scanf("%d", &position);
        //Soma mais um à posição da entrada
        bar[position]++;
    }

    //Exibe a saída formatada
    for (int i = 0; i < bar_amount; i++) {
        printf("%d - ", i);
        //Executa o print de # de acordo com a soma da entrada
        for (int j = 0; j < bar[i]; j++) {
            printf("#");
        }
        printf("\n");
    }

    free(bar);

    return EXIT_SUCCESS;
}