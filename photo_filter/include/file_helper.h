#ifndef _FILE_HELPER_H
#define _FILE_HELPER_H

#include <stdio.h>

//Booleanos
#define boolean int
#define TRUE 1
#define FALSE 0

//Buffer da função ReadLine
#define READLINE_BUFFER 2048

//Função para ler linhas
char *readLine(FILE *);

#endif