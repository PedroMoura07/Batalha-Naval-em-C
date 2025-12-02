#ifndef TABULEIRO_H
#define TABULEIRO_H

typedef struct N {
    EstadoCelula estado; // Estado da célula/espaço (água, navio, acerto, erro)
    int id_navio;        
} N;                // Struct de cada posição no tabuleiro

typedef struct Tabuleiro {
    int linhas, colunas; // Dimensões do tabuleiro
    N *celulas;     // Ponteiro para o array de células/espaços
} Tabuleiro;             // Struct do tabuleiro


// Aloca e inicializa o tabuleiro 
Tabuleiro criar_tabuleiro(int linhas, int colunas);

// Libera a memória do tabuleiro 
void destruir_tabuleiro(Tabuleiro *t);

// Converte uma coordenada para índices 
int analisar_coordenada(const char *coord_str, int *linha, int *coluna, int linhas_max, int colunas_max);

// Obtém um ponteiro para a célula/espaço na posição 
N *obter_celula(Tabuleiro *t, int linha, int coluna);

// Exibe o tabuleiro no console 
void exibir_tabuleiro(const Tabuleiro *t, int is_tabuleiro_navios);

#endif