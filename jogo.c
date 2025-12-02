#include "tipos.h"
#include "jogo.h"
#include "tabuleiro.h"
#include "frota.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
                    
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


void inicializar_jogo(Jogo *j, int tamanho_tabuleiro, const char *nick1, const char *nick2) {
    

   
    // Jogador 1
    strcpy(j->j1.apelido, nick1);
    j->j1.frota = criar_frota_padrao();
    j->j1.tabuleiro = criar_tabuleiro(tamanho_tabuleiro, tamanho_tabuleiro);
    j->j1.tiros = criar_tabuleiro(tamanho_tabuleiro, tamanho_tabuleiro);
    
    // Jogador 2
    strcpy(j->j2.apelido, nick2);
    j->j2.frota = criar_frota_padrao();
    j->j2.tabuleiro = criar_tabuleiro(tamanho_tabuleiro, tamanho_tabuleiro);
    j->j2.tiros = criar_tabuleiro(tamanho_tabuleiro, tamanho_tabuleiro);
    
    j->jogador_atual = 1;
    j->fim_de_jogo = 0;
}

void destruir_jogo(Jogo *j) {
    
    destruir_frota(&j->j1.frota);
    destruir_tabuleiro(&j->j1.tabuleiro);
    destruir_tabuleiro(&j->j1.tiros);
    
    destruir_frota(&j->j2.frota);
    destruir_tabuleiro(&j->j2.tabuleiro);
    destruir_tabuleiro(&j->j2.tiros);
}




int pode_posicionar(Tabuleiro *t, int linha, int coluna, int comprimento, Orientacao orientacao) {
    for (int i = 0; i < comprimento; i++) {
        int l = linha + (orientacao == ORIENTACAO_V ? i : 0);
        int c = coluna + (orientacao == ORIENTACAO_H ? i : 0);

        if (l < 0 || l >= t->linhas || c < 0 || c >= t->colunas) return 0; 

        N *celula = obter_celula(t, l, c);
        if (celula && celula->estado == N_navio) return 0; 
    }
    return 1; 
}

// Posiciona todos os navios de forma aleatória 
void posicionar_frota_auto(Jogador *jogador, int tamanho) {
    for (int i = 0; i < jogador->frota.contagem; i++) {
        Navio *navio = &jogador->frota.navios[i];
        int posicionado = 0; 
        
        while (!posicionado) {
           
            Orientacao orientacao = (rand() % 2 == 0) ? ORIENTACAO_H : ORIENTACAO_V;
            int linha_rand = rand() % tamanho;
            int coluna_rand = rand() % tamanho;

            if (pode_posicionar(&jogador->tabuleiro, linha_rand, coluna_rand, navio->comprimento, orientacao)) {
                // Posiciona o navio
                for (int j = 0; j < navio->comprimento; j++) {
                    int l = linha_rand + (orientacao == ORIENTACAO_V ? j : 0);
                    int c = coluna_rand + (orientacao == ORIENTACAO_H ? j : 0);
                    
                    N *celula = obter_celula(&jogador->tabuleiro, l, c);
                    if (celula) {
                        celula->estado = N_navio;
                        celula->id_navio = i; 
                    }
                }
                navio->posicionado = 1;
                posicionado = 1; 
            }
        }
    }
    
}

int processar_tiro(Jogador *atirador, Jogador *alvo, int linha, int coluna) {
    // Ponteiros para as células no tabuleiro do alvo e no mapa de tiros do atirador 
    N *celula_alvo = obter_celula(&alvo->tabuleiro, linha, coluna);
    N *celula_tiro = obter_celula(&atirador->tiros, linha, coluna);

    if (!celula_alvo || !celula_tiro) {
        // Isso não deve acontecer se a validação de coordenada for correta, mas é um guarda.
        return 0; 
    }

    
    if (celula_tiro->estado == N_acerto || celula_tiro->estado == N_erro) {
        return 0;
        
        
    }

    // 2. Aplicar o tiro
    if (celula_alvo->estado == N_navio) {
        // ACERTOU
        celula_alvo->estado = N_acerto;
        celula_tiro->estado = N_acerto;

        int id_navio = celula_alvo->id_navio;
        Navio *navio_atingido = &alvo->frota.navios[id_navio];
        navio_atingido->acertos++;

        if (navio_esta_afundado(navio_atingido)) {
            // AFUNDOU
            if (frota_esta_afundada(&alvo->frota)) {
                return -2; // Sinaliza fim de jogo e Vitória 
            }
            return -1; // Sinaliza apenas que o navio afundou
        }
        return 1; // Sinaliza Acerto
    } else {
        
        celula_alvo->estado = N_agua; // Mantém água
        celula_tiro->estado = N_erro;
        return 0; 
    }
}
