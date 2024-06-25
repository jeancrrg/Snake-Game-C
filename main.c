#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define COLUNAS 50
#define LINHAS 18
#define TAMANHO_MAXIMO_COBRA 500

char tabuleiro[LINHAS][COLUNAS];
int jogoAcabou = 0;
int deltaX = 1, deltaY = 0;  // Direção inicial (direita)

struct ParteDaCobra {
    int x, y;
};
struct Cobra {
    int comprimento;
    struct ParteDaCobra parte[TAMANHO_MAXIMO_COBRA];
};
struct Cobra cobra;

struct Comida {
    int x, y;
    int consumida;
};
struct Comida comida;

void esconderCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void gerarCobra() {
    cobra.comprimento = 0;
    cobra.parte[0].x = COLUNAS / 2; 
    cobra.parte[0].y = LINHAS / 2;
}

void gerarComida() {
    comida.x = 1 + rand() % (COLUNAS - 2);
    comida.y = 1 + rand() % (LINHAS - 2);
    comida.consumida = 0;
}

void preencherTabuleiro() {
    int x, y;
    
    for (y = 0; y < LINHAS; y++) {
        for (x = 0; x < COLUNAS; x++) {
            if (x == 0 || y == 0 || x == COLUNAS - 1 || y == LINHAS - 1) {
                tabuleiro[y][x] = '*';
            } else {
                tabuleiro[y][x] = ' ';
            }
        }
    }
}

void desenharComida() {
    if (!comida.consumida) {
        tabuleiro[comida.y][comida.x] = '@';
    }
}

void moverCobra() {
    int i;

    for (i = cobra.comprimento - 1; i > 0; i--) {
        cobra.parte[i] = cobra.parte[i - 1];
    }
    
    cobra.parte[0].x += deltaX;
    cobra.parte[0].y += deltaY;
}

void desenharCobra() {
    int i;
    
    for (i = cobra.comprimento - 1; i > 0; i--) {
        tabuleiro[cobra.parte[i].y][cobra.parte[i].x] = '#';
    }
    tabuleiro[cobra.parte[0].y][cobra.parte[0].x] = '$';
}

void validarRegrasJogo() {
    int i;
    
    if (!comida.consumida && comida.x == cobra.parte[0].x && comida.y == cobra.parte[0].y) {
        comida.consumida = 1;
        cobra.comprimento++;
        gerarComida();
    }
    
    if (cobra.parte[0].x == 0 || cobra.parte[0].x == COLUNAS - 1 || cobra.parte[0].y == 0 || cobra.parte[0].y == LINHAS - 1) {
        jogoAcabou = 1;
    }
    
    for (i = 1; i < cobra.comprimento; i++) {
        if (cobra.parte[0].x == cobra.parte[i].x && cobra.parte[0].y == cobra.parte[i].y) {
            jogoAcabou = 1;
        }
    }
}

void moverCursorInicioDaTela() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hOut, coord);
}

void imprimirCabecalhoJogo() {
    printf("+ ============================================== + \n");
    printf("|               SNAKE GAME                       | \n");
    printf("|------------------------------------------------| \n");
    printf("| PONTOS: %d                                     | \n", cobra.comprimento * 10);
    printf("+ ============================================== + \n");
}

void imprimirTabuleiro() {
    int x, y;
        
    for (y = 0; y < LINHAS; y++) {
        for (x = 0; x < COLUNAS; x++) {
            putch(tabuleiro[y][x]);
        }
        putch('\n');
    }    
}

void lerTeclado() {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 224) { // Código especial para as setas
            ch = _getch();
            switch (ch) {
                // seta para cima
                case 72:
                    if (deltaY == 0) { 
                        deltaX = 0; 
                        deltaY = -1;
                    } 
                    break;
                // seta para baixo
                case 80:
                    if (deltaY == 0) { 
                        deltaX = 0; 
                        deltaY = 1;
                    } 
                    break;  
                // seta para a esquerda 
                case 75:
                    if (deltaX == 0) { 
                        deltaX = -1; 
                        deltaY = 0; 
                    }
                    break;
                // seta para a direita
                case 77: 
                    if (deltaX == 0) { 
                        deltaX = 1; 
                        deltaY = 0; 
                    } 
                    break;  
            }
        }
    }
}

void definirVelocidadeCobra() {
    Sleep(100);
}

void imprimirFimDeJogo() {
    printf("+ ============================================== + \n");
    printf("|                   GAME OVER!                   | \n");
    printf("|------------------------------------------------| \n");
    printf("| PONTOS FINAIS: %i                              | \n", cobra.comprimento * 10);
    printf("+ ============================================== + \n");
}

int main() {  
    // Inicializa o gerador de números aleatórios
    srand(time(0));
    
    esconderCursor();
    gerarCobra();
    gerarComida();

    while (!jogoAcabou) {
        preencherTabuleiro();
        desenharComida();
        moverCobra();
        desenharCobra();
        validarRegrasJogo();
        moverCursorInicioDaTela();
        imprimirCabecalhoJogo();
        imprimirTabuleiro();
        lerTeclado();
        definirVelocidadeCobra();
    }
    imprimirFimDeJogo();
    
    return 0;
}
