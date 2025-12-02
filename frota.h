#ifndef FROTA_H
#define FROTA_H

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

// Inicializa a frota padrão obrigatória 
Frota criar_frota_padrao();

// Libera a memória da frota 
void destruir_frota(Frota *f);

// Verifica se um navio específico foi afundado
int navio_esta_afundado(const Navio *navio);

// Verifica se toda a frota do jogador foi afundada
int frota_esta_afundada(const Frota *frota);

#endif