#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Considerei o número binário como um vetor de char e o preenchi com 0
    char binario[30] = {0};
    int decimal = 0;
    int exp = 0;

    scanf("%s", binario);

    // Invertendo o vetor
    for (int i = 29; i >= 0; i--) {
        if (binario[i] != 0) {
            // Decimal recebe o número binário convertido de char * 2
            // elevado ao expoente e incrementa o expoente
            decimal += (binario[i] - '0') * pow(2, exp);
            exp++;
        }
    }

    printf("%d", decimal);

    return EXIT_SUCCESS;
}