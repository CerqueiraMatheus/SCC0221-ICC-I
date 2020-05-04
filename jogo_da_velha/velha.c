#include <stdio.h>
#include <stdlib.h>

//Para operações booleanas
#define bool int
#define FALSE 0
#define TRUE 1

//Para controle do jogo
#define EM_PROCESSO 2
#define EMPATE 3
#define VITORIA 4

//Função que exibe o jogo
void show_velha(char **velha) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%c   |", velha[i][j]);
        }
        printf("\n----+----+-----\n");
    }
}

//Função que verifica o estado
int get_state(char **velha) {
    //Casos de vitória
    if (
        ((velha[0][0] == velha[1][1]) && (velha[1][1] == velha[2][2])) ||
        ((velha[0][2] == velha[1][1]) && (velha[1][1] == velha[2][0])) ||
        ((velha[0][0] == velha[0][1]) && (velha[0][1] == velha[0][2])) ||
        ((velha[1][0] == velha[1][1]) && (velha[1][1] == velha[1][2])) ||
        ((velha[2][0] == velha[2][1]) && (velha[2][1] == velha[2][2])) ||
        ((velha[0][0] == velha[1][0]) && (velha[1][0] == velha[2][0])) ||
        ((velha[0][1] == velha[1][1]) && (velha[1][1] == velha[2][1])) ||
        ((velha[0][2] == velha[1][2]) && (velha[1][2] == velha[2][2]))) {
        return VITORIA;
    }
    //Caso "jogando"
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (velha[i][j] != 'X' && velha[i][j] != 'O') {
                return EM_PROCESSO;
            }
        }
    }
    //Se nenhum dos dois, empate
    return EMPATE;
}

//Função que executa a jogada de um jogador
bool make_move(char **velha, char move, char player) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (velha[i][j] == move) {
                //Atribui a posição ao jogador
                velha[i][j] = player;
                return TRUE;
            }
        }
    }
    //Caso ocupada, retorna falso
    return FALSE;
}

//Função que troca o jogador atual
void change_player(char *player) {
    *player = (*player == 'O') ? ('X') : ('O');
}

int main() {
    char **velha, aux = '0', player = 'O', move = '0';

    //Instancia e atribui os valores para o jogo
    velha = (char **)malloc(3 * sizeof(char *));
    for (int i = 0; i < 3; i++) {
        velha[i] = (char *)malloc(3 * sizeof(char));
        for (int j = 0; j < 3; j++) {
            aux++;
            velha[i][j] = aux;
        }
    }

    show_velha(velha);
    printf("Jogo da velha!\n");

    //Enquanto o estado for "em processo"
    while (get_state(velha) == EM_PROCESSO) {
        //Muda o jogador e recebe a jogada
        change_player(&player);
        printf("Escolha um número para jogar, jogador %c!\n", player);
        scanf(" %c", &move);

        //Se o movimento for inválido,
        //itera até o válido
        if (!make_move(velha, move, player)) {
            do {
                printf("Jogada inválida! Escolha outra casa.\n");
                scanf(" %c", &move);
            } while (!make_move(velha, move, player));
        }

        show_velha(velha);
    }

    //Exibe o resultado
    if(get_state(velha) == VITORIA){
        printf("Fim do jogo! O jogador %c venceu!\n", player);
    } else {
        printf("Deu velha!\n");
    }

    //Libera os ponteiros
    for (int i = 0; i < 3; i++) {
        free(velha[i]);
    }
    free(velha);

    return EXIT_SUCCESS;
}