#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

//Para booleanos
#define boolean int
#define TRUE 1
#define FALSE 0

//Para as opções de organização
#define ALPHA 1
#define NUMERIC 2

//Buffer da readRegisters
#define READLINE_BUFFER 2048

//Struct para os registros
typedef struct {
    int number;
    char *name;
} Register;

//Checa se é fim de linha
boolean isEndLine(char c) {
    return (c == '\r' || c == '\n' || c == EOF);
}

//Limpa stdin
void cleanInput() {
    int c;
    //Consome todos os caracteres de stdin
    while (((c = fgetc(stdin)) != '\n') && (c != EOF));
}

//Retorna o char em minúscula
char getLower(char c) {
    return (c > 64 && c < 91) ? (c + 32) : (c);
}

//Verifica duas strings alfabeticamente
boolean checkStrings(char *s1, char *s2) {
    int i = 0;

    //Percorre as duas palavras e compara letra por letra
    for (; s1[i] != '\0' && s2[i] != '\0'; i++) {
        if (getLower(s2[i]) > getLower(s1[i])) {
            return FALSE;
        } else if (getLower(s2[i]) < getLower(s1[i])) {
            return TRUE;
        }
    }

    //Caso termine de percorrer e as duas sejam iguais,
    //retorna a menor String
    return (s1[i] == '\0' && (s2[i] == '\0')) ? FALSE
                                              : ((s1[i] == '\0') ? FALSE : TRUE);
}

//Lê os registros
void readRegisters(FILE *stream, Register ***registersList, int *registersSize) {
    do {
        char *nameString = 0, *numberString = 0, actualChar = 0, storedChar;
        int namePosition = 0, numberPosition = 0;

        do {
            if (namePosition % READLINE_BUFFER == 0) {  //Incrementa o tamanho da string de nome
                nameString =
                    (char *)realloc(nameString, (namePosition / READLINE_BUFFER + 1) * READLINE_BUFFER);
            }
            if (numberPosition % READLINE_BUFFER == 0) {  //Incrementa o tamanho da string de número
                numberString =
                    (char *)realloc(numberString, (numberPosition / READLINE_BUFFER + 1) * READLINE_BUFFER);
            }

            actualChar = (char)fgetc(stream);

            //Se o char atual for -, armazena
            if (actualChar == '-') {
                storedChar = actualChar;
            } else if (isdigit(actualChar)) {

                //Se o char for dígito e houver o char armazenado
                //adiciona o - e reseta o char de armazenamento
                if (storedChar) {
                    numberString[numberPosition++] = storedChar;
                    storedChar = (char) 0;
                }

                //Incrementa a string de número com o char atual
                numberString[numberPosition++] = actualChar;

            } else if (!isEndLine(actualChar)) {
                
                //Se o char for dígito e houver o char armazenado
                //adiciona o - reseta o char de armazenamento
                if (storedChar) {
                    nameString[namePosition++] = storedChar;
                    storedChar = (char) 0;
                }

                //Incrementa a string de nomecom o char atual
                nameString[namePosition++] = actualChar;
            }
        } while (!isEndLine(actualChar) && !feof(stream));

        //Coloca o terminador nas strings
        nameString[namePosition - 1] = '\0';
        numberString[numberPosition] = '\0';

        //Altera a lista e adiciona os elementos da linha
        (*registersList) = (Register **)realloc(*registersList, (++(*registersSize)) * sizeof(Register *));
        (*registersList)[(*registersSize) - 1] = (Register *)malloc(sizeof(Register));
        ((*registersList)[(*registersSize) - 1])->name = (char *)realloc(nameString, namePosition);
        ((*registersList)[(*registersSize) - 1])->number = atoi((char *)realloc(numberString, numberPosition));

    } while (!feof(stream));
}

//Organiza a lista conforme opção
void sortRegisters(Register ***registersList, int registersSize, int opt) {
    Register *aux = 0;
    boolean swap = FALSE;

    //Executa o bubble sort na lista
    for (int i = 1; i < registersSize; i++) {
        for (int j = registersSize - 1; j >= i; j--) {
            //Reseta o operador de troca
            swap = FALSE;

            //De acordo com a opção
            //checa se o elemento deve ser trocado
            //Se a > b -> troca
            switch (opt) {
                case ALPHA:
                    if (checkStrings(((*registersList)[j - 1])->name, ((*registersList)[j])->name))
                        swap = TRUE;
                    break;

                case NUMERIC:
                    if (((*registersList)[j - 1])->number > ((*registersList)[j])->number)
                        swap = TRUE;
                    break;
            }

            //Se o operador de troca for ativado
            //troca os itens
            if (swap) {
                aux = (*registersList)[j - 1];
                (*registersList)[j - 1] = (*registersList)[j];
                (*registersList)[j] = aux;
            }
        }
    }
}

int main() {
    Register **registersList = NULL;
    int registersSize = 0, opt;

    //Recebe a opção
    scanf("%d", &opt);
    cleanInput();

    //Lê e organiza a lista conforme opção
    readRegisters(stdin, &registersList, &registersSize);
    sortRegisters(&registersList, registersSize, opt);

    //Exibe e libera os elementos da lista e a lista
    for (int i = 0; i < registersSize; i++) {
        printf("%d\t%s\n", registersList[i]->number, registersList[i]->name);
        free(registersList[i]->name);
        free(registersList[i]);
    }
    free(registersList);

    return EXIT_SUCCESS;
}