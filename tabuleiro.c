#include "tipos.h"   
#include "tabuleiro.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>  

//                                     

typedef struct N {
    EstadoCelula estado; // Estado da célula/espaço (água, navio, acerto, erro)
    int id_navio;        
} N;                // Struct de cada posição no tabuleiro

typedef struct Tabuleiro {
    int linhas, colunas; // Dimensões do tabuleiro
    N *celulas;     // Ponteiro para o array de células/espaços
} Tabuleiro;             // Struct do tabuleiro

Tabuleiro criar_tabuleiro(int linhas, int colunas) {
    Tabuleiro t;
    t.linhas = linhas;
    t.colunas = colunas;
    
    // Alocação Dinâmica de Memória 
    t.celulas = (N *)malloc(linhas * colunas * sizeof(N)); 
    
    if (t.celulas == NULL) {
        perror("Erro ao alocar memoria para o tabuleiro");
        exit(EXIT_FAILURE);
    }

  
    for (int i = 0; i < linhas * colunas; i++) {
        t.celulas[i].estado = N_agua;
        t.celulas[i].id_navio = -1; // -1 indica sem navio
    }
    return t;
}

void destruir_tabuleiro(Tabuleiro *t) {
    // Liberação de Memória 
    if (t->celulas != NULL) {
        free(t->celulas);
        t->celulas = NULL;
    }
}



// Retorna um ponteiro para a célula, garantindo acesso via endereço 
N *obter_celula(Tabuleiro *t, int linha, int coluna) {
    // Validação de limites 
    if (linha < 0 || linha >= t->linhas || coluna < 0 || coluna >= t->colunas) {
        // Penalização por acesso fora dos limites é de -10 pontos
        return NULL; 
    }
    // Acesso por ponteiro
    return &t->celulas[linha * t->colunas + coluna];
}



int analisar_coordenada(const char *coord_str, int *linha, int *coluna, int linhas_max, int colunas_max) {
    if (coord_str == NULL || strlen(coord_str) < 2 || !isalpha(coord_str[0])) {
        return 0;
    }

   
    *coluna = toupper(coord_str[0]) - 'A';
    
    // Linha: lê o restante da string como número
   
    int linha_lida = atoi(&coord_str[1]); 
    *linha = linha_lida - 1; // Ajusta para índice base 0

    // 1. Validação de formato (apenas números após a letra)
    for (size_t i = 1; i < strlen(coord_str); i++) {
        if (!isdigit(coord_str[i])) return 0;
    }

    // 2. Validação de limites (Obrigatório)
    if (*coluna < 0 || *coluna >= colunas_max) return 0;
    if (*linha < 0 || *linha >= linhas_max) return 0;

    return 1;
}



// Exibe o tabuleiro no console 
void exibir_tabuleiro(const Tabuleiro *t, int is_tabuleiro_navios) {
    // Implementação da exibição do tabuleiro 
    printf("  ");
    // Exibe cabeçalho de colunas 
    for (int c = 0; c < t->colunas; c++) {
        printf("%c", 'A' + c);
    }
    printf("\n");

    for (int l = 0; l < t->linhas; l++) {
        // Exibe número da linha
        printf("%-2d", l + 1); 
        for (int c = 0; c < t->colunas; c++) {
            const N *c_ptr = obter_celula((Tabuleiro *)t, l, c);

            if (is_tabuleiro_navios) {
                // Visão do jogador 
                switch (c_ptr->estado) {
                    case N_navio: printf("S"); break; // Navio
                    case N_acerto: printf("X"); break; // Navio atingido 
                    default: printf("~"); break; // Água ou erro 
                }
            } else {
                // Visão de tiros 
                switch (c_ptr->estado) {
                    case N_acerto: printf("X"); break; // Tiro acertado 
                    case N_erro: printf("."); break; // Tiro errado (água) 
                    default: printf("~"); break; // Não explorado
                }
            }
        }
        printf("\n");
    }
}