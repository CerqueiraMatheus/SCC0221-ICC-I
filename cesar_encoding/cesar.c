#include <stdio.h>
#include <stdlib.h>

int do_cesar(int opt, char c, int key) {
    /**
     * Três casos para operação (decriptar/encriptar)
     *
     * 1) É número e não fica no range do formato // retorna c (+key-26 / -key+26)
     * 2) É número e fica no range // retorna c (+key / -key)
     * 3) Não é número  //retorna c
     * 
     * ***range: MAIÚSCULAS (65~90); MINÚSCULAS (97~122)
     * */

    switch (opt) {
        //Encripta
        case 1:
            if (((c >= 65 && c <= 90) && (c + key > 90)) ||
                ((c >= 97 && c <= 122) && (c + key > 122))) {
                return c + key - 26;
            } else if ((c + key >= 65 && c + key <= 90) ||
                       (c + key >= 97 && c + key <= 122)) {
                return c + key;
            } else {
                return c;
            }
            break;

        //Decripta
        case 2:
            if (((c >= 65 && c <= 90) && (c - key < 65)) ||
                ((c >= 97 && c <= 122) && (c - key < 97))) {
                return c - key + 26;
            } else if ((c - key >= 65 && c - key <= 90) ||
                       (c - key >= 97 && c - key <= 122)) {
                return c - key;
            } else {
                return c;
            }
            break;
    }
    return 0;
}

int main() {
    int code, key, i;
    char msg[1000] = {0}, *conv;

    scanf("%d \n", &code);
    scanf("%d \n", &key);
    scanf("%[^\n]", msg);

    conv = (char *)malloc(sizeof(char));

    //Executa o do_cesar enquanto houver preenchimento
    for (i = 0; i <= 1000 && msg[i] != 0; i++) {
        conv[i] = do_cesar(code, msg[i], key);
        conv = (char *)realloc(conv, (i + 1 * sizeof(char)));
    }

    //Insere o terminador de String na última posição
    conv[i] = '\0';

    printf("%s", conv);

    free(conv);

    return EXIT_SUCCESS;
}