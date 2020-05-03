#include <stdio.h>
#include <stdlib.h>

int main() {
    int line, column, order;
    int **matrix;

    //Recebe linha e coluna
    scanf("%d", &line);
    scanf("%d", &column);

    //Verifica se os dados estão dentro dos padrões (ordem 1, 2 e 3; quadrada)
    if ((line == column) && (line > 0 && line < 4)) {
        //Aloca o as linhas da matriz
        matrix = (int **)malloc(line * sizeof(int *));

        //Atribui a ordem (por organização)
        order = line;

        //Aloca as colunas da matriz
        for (int i; i < order; i++) {
            matrix[i] = (int *)malloc(order * sizeof(int));
        }

        //Recebe a matriz
        for (int i = 0; i < order; i++) {
            for (int j = 0; j < order; j++) {
                scanf("%d", &matrix[i][j]);
            }
        }

        //Switch para os casos
        switch (order) {
            //Retorna o módulo do valor único
            case 1:
                printf("O determinante da matriz de entrada equivale a %d e tem ordem %d\n", matrix[0][0], order);
                break;

            //Retorna ad - cb
            case 2:
                printf("O determinante da matriz de entrada equivale a %d e tem ordem %d\n",
                       (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]),
                       order);
                break;

            //Retorna a(ei - fh) - b(ei - fg) + c(dh - eg)
            case 3:
                printf("O determinante da matriz de entrada equivale a %d e tem ordem %d\n",
                       (
                           matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
                           matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
                           matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0])),
                       order);

                break;
        }

        //Libera a matriz
        for (int i = 0; i < order; i++) {
            free(matrix[i]);
        }
        free(matrix);

    } else if ((line <= 0 || line >= 4) || ((column <= 0 || column >= 4))) {
        //Saída de erro 1: a matriz está fora do comprimento
        printf("Entradas invalidas!\n");
    } else {
        //Saída de erro 2: a matriz não é quadrada
        printf("A matriz de entrada nao possui determinante!\n");
    }

    return EXIT_SUCCESS;
}