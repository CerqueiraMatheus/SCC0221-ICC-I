#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define READLINE_BUFFER 4096

char *readline(FILE *stream) {
    char *string = 0;
    int pos = 0;
    do {
        if (pos % READLINE_BUFFER == 0) {
            string = (char *)realloc(string,
                                     (pos / READLINE_BUFFER + 1) * READLINE_BUFFER);
        }
        string[pos] = (char)fgetc(stream);
    } while (string[pos++] != '\n' && !feof(stream) && string[pos++] != '\r');
    string[pos - 1] = '\0';
    string = (char *)realloc(string, pos);
    return string;
}

int main() {
    char *book, **stored_books, **rented_books;
    int first_ex = TRUE, opt = 0, position = 0, rent = FALSE;
    int stored_size = 0, rented_size = 0, *days_stored, *days_rented;

    scanf("%d", &stored_size);

    stored_books = (char **)malloc(stored_size * sizeof(char *));
    days_stored = (int *)malloc(stored_size * sizeof(int));

    rented_books = (char **)malloc(sizeof(char *));
    book = (char *)malloc(sizeof(char));

    for (int i = 0; i < stored_size; i++) {
        fflush(stdin);
        stored_books[i] = readline(stdin);
        scanf("%d", &days_stored[i]);
    }

    do {
        if (!first_ex) {
            free(book);
        }

        scanf("%d", &opt);
        first_ex = FALSE;

        switch (opt) {
            case 1:
                printf("Digite o livro que voce procura:\n");
                book = readline(stdin);
                printf("saiu");

                int days;
                rent = FALSE;
                position = 0;

                do {
                    position++;
                } while (!strcmp(book, stored_books[position]));

                if (stored_books[position]) {
                    for (int i = 0; i < rented_size; i++) {
                        if (strcmp(book, rented_books[i])) {
                            rent = TRUE;
                            days = days_stored;
                        }
                    }

                    if (rented_size >= 10) {
                        printf("Voce ja tem 10 livros alugados\n");

                    } else if (rent) {
                        printf("Livro ja alugado\n");

                    } else {
                        rented_size++;

                        printf("1");
                        rented_books = (char **)realloc(rented_books, rented_size * sizeof(char *));
                        rented_books[rented_size - 1] = (char *)realloc(rented_books[rented_size - 1], sizeof(book));
                        rented_books[rented_size - 1] = book;

                        printf("2");
                        days_rented = (int *)realloc(days_rented, rented_size * sizeof(int));
                        days_rented[rented_size - 1] = days;

                        printf("%s alugado com sucesso\n", book);
                    }

                } else {
                    printf("“Livro nao encontrado na biblioteca\n");
                }

                break;

            case 2:
                if (rented_size != 0) {
                    printf("Voce tem %d livro(s) alugado(s)\n", rented_size);

                    for (int i = 0; i < rented_size; i++) {
                        printf("Livro nome: %s\n", rented_books[i]);
                        printf("Devolve-lo daqui %d dias\n", days_rented[i]);
                    }
                } else {
                    printf("Voce nao tem livros alugados\n");
                }
                break;

            case 3:
                printf("Digite o livro que deseja devolver:\n");
                book = readline(stdin);

                rent = FALSE;
                position = 0;

                for (int i = 0; i < rented_size; i++) {
                    if (strcmp(book, rented_books)) {
                        rent = TRUE;
                        position = i;
                    }
                }

                if (rent) {
                    for (int i = position; i <= rented_size; i++) {
                        if (i != rented_size) {
                            rented_books[i] = rented_books[i + 1];
                            days_rented[i] = days_rented[i + 1];
                        } else {
                            free(rented_books[i]);
                            free(days_rented[i]);
                        }
                    }

                    rented_size--;
                    rented_books = (char **)realloc(rented_books, rented_size * sizeof(char *));
                    days_rented = (int *)realloc(days_rented, rented_size * sizeof(int));

                    printf("Livro %s foi devolvido com sucesso\n", book);
                }

                break;

            case 4:
                free(book);
                for (int i = 0; i < stored_size; i++) {
                    free(stored_books[i]);
                }
                free(stored_books);
                free(days_stored);

                for (int i = 0; i < stored_size; i++) {
                    free(rented_books[i]);
                }
                free(rented_books);
                free(days_rented);

                break;
        }

    } while (opt != 4);

    return EXIT_SUCCESS;
}