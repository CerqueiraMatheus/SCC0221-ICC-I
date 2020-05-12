#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Para trabalhar com booleanos
#define boolean int
#define TRUE 1
#define FALSE 0

#define READLINE_BUFFER 4096

//Função para limpar stdin
void cleanInput() {
    int c;
    //Consome todos os caracteres de stdin
    while (((c = fgetc(stdin)) != '\n') && (c != EOF))
        ;
}

//Função para ler linha
char *readline(FILE *stream) {
    char *string = 0;
    int position = 0;
    char aux = 0;

    do {
        if (position % READLINE_BUFFER == 0) {
            string =
                (char *)realloc(string, (position / READLINE_BUFFER + 1) * READLINE_BUFFER);
        }
        string[position] = (char)fgetc(stream);
        aux = string[position++];
    } while (aux != '\n' && aux != '\r' && !feof(stream));

    string[position - 1] = '\0';
    string = (char *)realloc(string, position);

    return string;
}

//Struct para livros
struct Book {
    char *name;
    int daysAvailable;
};

//Função para alugar livros
void rentBook(struct Book ***storedBooks, struct Book ***rentedBooks, int *storedSize, int *rentedSize) {
    struct Book *aux;
    boolean rent = FALSE, exists = FALSE;
    int position = 0;

    //Inicializa o livro auxiliar
    aux = (struct Book *)malloc(sizeof(struct Book));

    //Recebe o livro
    printf("Digite o livro que voce procura:\n");
    aux->name = readline(stdin);

    //Se houver livro armazenado
    if ((*storedSize) != 0) {
        //Enquanto a posição for menor que a quantidade de livros armazenada
        //e o livro procurado não "existir"
        while ((position < (*storedSize)) && (!exists)) {
            if (strcmp(aux->name, (*storedBooks)[position]->name) == 0) {
                exists = TRUE;
            } else {
                position++;
            }
        }

        //Se o livro "existir"
        if (exists) {
            //Percorre a lista de emprestados e verifica
            //se está alugado
            for (int i = 0; i < (*rentedSize); i++) {
                if (!strcmp(aux->name, (*rentedBooks)[i]->name)) {
                    rent = TRUE;
                }
            }

            //Se tiver mais de 10 livros alugados
            if (*rentedSize >= 10) {
                printf("Voce ja tem 10 livros alugados\n");

            //Se não tiver, mas o livro já estiver
            //alugado
            } else if (rent) {
                printf("Livro ja alugado\n");

            //Se estiver disponível
            } else {
                (*rentedSize)++;
                (*rentedBooks) = (struct Book **)realloc(*rentedBooks, (*rentedSize) * sizeof(struct Book *));
                (*rentedBooks)[(*rentedSize) - 1] = (struct Book *)malloc(sizeof(struct Book));
                (*rentedBooks)[(*rentedSize) - 1] = (*storedBooks)[position];

                //Exibe a mensagem de sucesso
                printf("%s alugado com sucesso\n", ((*rentedBooks)[(*rentedSize) - 1])->name);
            }

        //Se não for encontrado
        } else {
            printf("Livro nao encontrado na biblioteca\n");
        }
    }

    //Libera o livro auxiliar
    free(aux->name);
    free(aux);
}

//Função para exibir livros alugados
void checkBooks(struct Book **rentedBooks, int rentedSize) {
    //Se o número de livros alugados for diferente de 0
    if (rentedSize != 0) {
        printf("Voce tem %d livro(s) alugado(s)\n", rentedSize);

        //E itera os livros
        for (int i = 0; i < rentedSize; i++) {
            printf("Livro nome: %s\n", rentedBooks[i]->name);
            printf("Devolve-lo daqui %d dias\n", rentedBooks[i]->daysAvailable);
        }

        //Do contrário, exibe a mensagem
    } else {
        printf("Voce nao tem livros alugados\n");
    }
}

void returnBook(struct Book ***rentedBooks, int *rentedSize) {
    struct Book **newRentedList = 0;
    struct Book *aux;
    boolean rent = FALSE;
    int position = 0;

    //Inicializa o livro auxiliar
    aux = (struct Book *)malloc(sizeof(struct Book));

    //Recebe o livro
    printf("Digite o livro que deseja devolver:\n");
    aux->name = readline(stdin);

    //Procura o livro na lista de alugados
    //Caso ache, armazena a posição e seta a flag
    while (position < (*rentedSize) && !rent) {
        if (strcmp(aux->name, ((*rentedBooks)[position]->name)) == 0) {
            rent = TRUE;
        } else {
            position++;
        }
    }

    //Se estiver alugado,
    //"devolve", excluindo-o de uma nova lista
    if (rent) {
        int iterator = 0;
        newRentedList = (struct Book **)realloc(newRentedList, (--(*rentedSize)) * sizeof(struct Book *));
        for (int i = 0; i <= (*rentedSize); i++) {
            if (i != position) {
                newRentedList[iterator] = (*rentedBooks)[i];
                iterator++;
            }
        }

        //Atribui a nova lista e exibe mensagem
        *rentedBooks = newRentedList;
        printf("Livro %s foi devolvido com sucesso\n", aux->name);

    //Do contrário, aponta o erro
    } else {
        printf("Voce nao possui esse livro\n");
    }

    //Libera o livro auxiliar
    free(aux->name);
    free(aux);
}

//Função para ler as entradas
void readData(struct Book ***storedBooks, int storedSize) {
    for (int i = 0; i < storedSize; i++) {
        (*storedBooks)[i] = (struct Book *)malloc(sizeof(struct Book));
        (*storedBooks)[i]->name = readline(stdin);
        scanf("%d", &(*storedBooks)[i]->daysAvailable);
        cleanInput();
    }
}

int main() {
    //Ponteiros de struct para os livros
    struct Book **storedBooks, **rentedBooks = 0;

    //Para os tamanhos
    int storedSize, rentedSize = 0;

    //Armazenador da opção
    int opt = 0;

    //Armazena o tamanho e dimensiona o ponteiro de livros
    scanf("%d", &storedSize);
    storedBooks = (struct Book **)malloc(storedSize * sizeof(struct Book *));
    cleanInput();

    //Lê as entradas
    readData(&storedBooks, storedSize);

    do {
        //Lê a opção
        scanf("%d", &opt);
        cleanInput();

        //Executa a operação escolhida
        switch (opt) {
            case 1:
                rentBook(&storedBooks, &rentedBooks, &storedSize, &rentedSize);
                break;

            case 2:
                checkBooks(rentedBooks, rentedSize);
                break;

            case 3:
                returnBook(&rentedBooks, &rentedSize);
                break;

            case 4:
                printf("Programa finalizado\n");
                break;
        }

    } while (opt != 4);

    //Libera o ponteiro dos livros armazenados
    for (int i = 0; i < storedSize; i++) {
        free(storedBooks[i]->name);
        free(storedBooks[i]);
    }
    free(storedBooks);

    //Libera o ponteiro dos livros alugados
    free(rentedBooks);

    return EXIT_SUCCESS;
}