#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Tamanho do buffer
#define READLINE_BUFFER 2048

//Para usar booleanos
#define boolean int
#define FALSE 0
#define TRUE 1

//Função para ler linha
char *readline(FILE *stream) {
    char *string = 0;
    int position = 0;

    do {
        if (position % READLINE_BUFFER == 0) {
            string =
                (char *)realloc(string, (position / READLINE_BUFFER + 1) * READLINE_BUFFER);
        }
        string[position] = (char)fgetc(stream);
    } while (string[position++] != '\n' && !feof(stream));

    string[position - 1] = '\0';
    string = (char *)realloc(string, position);

    return string;
}

//Função para alimentar um array de char
void feedCharArray(char **charArray, char c, int size) {
    *charArray = (char *)realloc(*charArray, (size + 1) * sizeof(char));
    (*charArray)[size - 1] = c;
}

//Função para substituir uma palavra a partir da posição inicial
void replaceWord(char **phrase, char *oldWord, char *newWord, int initialPosition) {
    boolean endOfNewWord = FALSE, endOfOldWord = FALSE, replacement = FALSE;
    int index = 0;

    int sizeDifference = strlen(newWord) - strlen(oldWord);
    int resize = strlen(*phrase) + sizeDifference + 1;

    //Percorre enquanto não for substituída
    for (int i = initialPosition; replacement == FALSE; i++) {
        //Se não for o final da palavra antiga
        //E a nova, apenas continua
        if (!endOfNewWord && !endOfOldWord) {
            (*phrase)[i] = newWord[index];
            index++;
            if (!isalpha((*phrase)[i + 1])) endOfOldWord = TRUE;
            if (newWord[index] == '\0') endOfNewWord = TRUE;
        }

        //Se não for o final da palavra nova
        //mas for o da antiga, readapta o ponteiro
        //e continua a escrever a palavra
        if (!endOfNewWord && endOfOldWord) {
            *phrase = (char *)realloc(*phrase, resize * sizeof(char));
            for (int j = resize; i < j; j--) (*phrase)[j] = (*phrase)[j - sizeDifference];
            for (int j = i + 1; j < (int)strlen(newWord) + initialPosition; j++) {
                (*phrase)[j] = newWord[index];
                index++;
            }
            replacement = TRUE;
        }

        //Se for o final da nova
        //mas não o da antiga,
        //readapta e decrementa o ponteiro
        if (endOfNewWord && !endOfOldWord) {
            for (int j = i + 1; j < resize; j++) (*phrase)[j] = (*phrase)[j - sizeDifference];
            *phrase = (char *)realloc(*phrase, resize * sizeof(char));
            replacement = TRUE;
        }

        //Se for o final das duas, a substituição foi terminada
        if (endOfNewWord && endOfOldWord) replacement = TRUE;
    }
}

//Função para analisar a frase palavra por palavra
void analyzePhrase(char **phrase, char *oldWord, char *newWord) {
    char *curWord = 0;
    boolean initiatedWord = FALSE, finishedString = FALSE;
    int initialPosition = 0, wordSize = 0, iterator = 0;

    do {
        //Se for a primeira ou uma nova palavra
        if (!initiatedWord) {
            wordSize = 0;
            initialPosition = iterator;
            initiatedWord = TRUE;
        }

        //Se uma palavra tiver sido iniciada e tiver caracter válido na posição
        if (isalpha((*phrase)[iterator]) && initiatedWord) {
            wordSize++;
            feedCharArray(&curWord, (*phrase)[iterator], wordSize);

            //Caso não tenha caracter válido na posição
        } else if (!isalpha((*phrase)[iterator]) && initiatedWord) {
            curWord[wordSize] = '\0';
            initiatedWord = FALSE;
            if ((*phrase)[iterator] == '\0') finishedString = TRUE;
        }

        //Se não hover palavra iniciada (finalizada)
        //e a palavra for igual à solicitada
        if (iterator != 0 && !initiatedWord && !strcmp(curWord, oldWord))
            replaceWord(phrase, oldWord, newWord, initialPosition);

        //Incremento do contador
        iterator++;

        //Enquanto a String não for finalizada
    } while (finishedString == FALSE);

    //Libera o ponteiro
    free(curWord);
}

int main() {
    char *oldWord, *newWord, *phrase;

    //Atribui os ponteiros
    oldWord = readline(stdin);
    newWord = readline(stdin);
    phrase = readline(stdin);

    //Analisa a frase e, caso necessário,
    //substitui e exibe
    analyzePhrase(&phrase, oldWord, newWord);
    printf("%s\n", phrase);

    //Libera os ponteiros
    free(oldWord);
    free(newWord);
    free(phrase);

    return EXIT_SUCCESS;
}