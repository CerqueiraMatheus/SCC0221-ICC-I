#include <stdio.h>
#include <stdlib.h>

#include "boolean.h"
#include "extractor.h"
#include "index.h"
#include "metadata.h"
#include "register.h"

//Limpa stdin
void cleanInput() {
    int c;
    //Consome todos os caracteres de stdin
    while (((c = fgetc(stdin)) != '\n') && (c != EOF));
}

int main() {
    char *metadataFileName = 0;
    int opt = 0;
    int index;
    boolean indexCreated = FALSE;

    //Recebe o nome do arquivo de metadados
    //e popula a struct
    metadataFileName = getLineFromFile(stdin);
    Metadata *metadata = NULL;
    metadata = readMetaData(metadataFileName);

    //Inicializa o registro auxiliar
    Register *reg = NULL;

    //Loop até exit
    do {
        //Recebe a opção da linha
        opt = getOptionFromFile(stdin);

        //Trata a opção
        switch (opt) {
            case INSERT:
                //Lê, escreve e libera o registro
                readRegisterFromString(&reg, getLineFromFile(stdin), metadata);
                writeRegisterToFile(reg, metadata);
                freeRegister(&reg, metadata);
                break;

            case INDEX:
                //Escreve o arquivo de index e seta
                //o operador de criação
                writeIndex(metadata);
                indexCreated = TRUE;
                break;

            case SEARCH:
                //Se o operador de criação do index não
                //estiver ativado, cria o arquivo
                if (!indexCreated) writeIndex(metadata);

                //Lê o index e trata stdin
                scanf(" %d", &index);
                cleanInput();

                //Se o index for encontrado, exibe e libera
                if (readRegisterFromFile(metadata, &reg, index)) {
                    printRegister(reg, metadata);
                    freeRegister(&reg, metadata);
                }
                break;

            default:
                break;
        }
    } while (opt != EXIT);

    //Libera os ponteiros utilizados
    freeMetadata(metadata);
    free(metadataFileName);

    return EXIT_SUCCESS;
}