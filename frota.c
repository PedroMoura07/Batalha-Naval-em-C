#include "tipos.h"     
#include "frota.h"      
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

//                                                                 FROTA E NAVIOS

typedef struct Navio {
    char nome[30];       // Nome do navio 
    int comprimento;     // Tamanho do navio 
    int acertos;         // Quantidade de vezes que foi atingido
    int posicionado;     // Flag (1=sim, 0=não) para indicar se está no tabuleiro
} Navio;                 // Struct do navio

typedef struct Frota {
    Navio *navios;       // Array de navios
    int contagem;        // Quantidade total de navios
} Frota;                 // Struct da frota

Frota criar_frota_padrao() {
    Frota f;
    // A frota mínima tem 6 navios
    f.contagem = 6;
    
    // Alocação dinâmica de memória para a frota
    f.navios = (Navio *)malloc(f.contagem * sizeof(Navio));
    if (f.navios == NULL) {
        perror("Erro ao alocar memoria para a frota");
        exit(EXIT_FAILURE);
    }

   
    
    // 0: Porta-aviões (5 espaços) 
    strcpy(f.navios[0].nome, "Porta-avioes");
    f.navios[0].comprimento = 5;
    f.navios[0].acertos = 0;
    f.navios[0].posicionado = 0;
    
    // 1: Encouraçado (4 espaços) 
    strcpy(f.navios[1].nome, "Encouracado");
    f.navios[1].comprimento = 4;
    f.navios[1].acertos = 0;
    f.navios[1].posicionado = 0;
    
    // 2 & 3: Cruzadores (3 espaços) 
    for (int i = 2; i <= 3; i++) {
        strcpy(f.navios[i].nome, "Cruzador");
        f.navios[i].comprimento = 3;
        f.navios[i].acertos = 0;
        f.navios[i].posicionado = 0;
    }
    
    // 4 & 5: Destroyers (2 espaços) 
    for (int i = 4; i <= 5; i++) {
        strcpy(f.navios[i].nome, "Destroyer");
        f.navios[i].comprimento = 2;
        f.navios[i].acertos = 0;
        f.navios[i].posicionado = 0;
    }

    

    return f;
}

void destruir_frota(Frota *f) {
    if (f->navios != NULL) {
        free(f->navios); // Liberação da memória alocada 
        f->navios = NULL;
    }
}

int navio_esta_afundado(const Navio *navio) {
    return navio->acertos >= navio->comprimento;
}

int frota_esta_afundada(const Frota *frota) {
    for (int i = 0; i < frota->contagem; i++) {
        // Se encontrar qualquer navio que não afundou, a frota não está afundada
        if (!navio_esta_afundado(&frota->navios[i])) {
            return 0;
        }
    }
    
    return 1; // O jogo termina quando todos os navios de um jogador forem afundados
}
