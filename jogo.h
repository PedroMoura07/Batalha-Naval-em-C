#include "frota.h"
#ifndef JOGO_H
#define JOGO_H

typedef struct Jogador {
    Tabuleiro tabuleiro; // Onde os navios do jogador estão 
    Tabuleiro tiros;     // Mapa de tiros que o jogador deu no adversário
    Frota frota;         // Frota de navios do jogador
    char apelido[50];    // Nome de exibição do jogador
} Jogador;               // Struct do jogador

typedef struct Jogo {
    Jogador j1, j2;             // Jogadores 1 e 2
    int jogador_atual;          // 1 ou 2
    int fim_de_jogo;            // 1=sim, 0=não
} Jogo;                         // Struct do jogo


// Inicializa a estrutura Jogo e seus componentes
void inicializar_jogo(Jogo *j, int tamanho_tabuleiro, const char *nick1, const char *nick2);

// Libera toda a memória alocada para o jogo
void destruir_jogo(Jogo *j);

// Posicionamento
void posicionar_frota_auto(Jogador *jogador, int tamanho);


// Função do tiro
int processar_tiro(Jogador *atirador, Jogador *alvo, int linha, int coluna);




#endif