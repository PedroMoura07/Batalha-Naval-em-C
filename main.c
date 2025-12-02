

#ifndef TIPOS_H
#define TIPOS_H






typedef enum { N_agua, N_navio, N_acerto, N_erro } EstadoCelula;

typedef enum { ORIENTACAO_H, ORIENTACAO_V } Orientacao;


//                                        ----------------------TABULEIRO--------------------------

typedef struct N {
    EstadoCelula estado; // Estado da célula/espaço (água, navio, acerto, erro)
    int id_navio;        
} N;                // Struct de cada posição no tabuleiro

typedef struct Tabuleiro {
    int linhas, colunas; // Dimensões do tabuleiro
    N *celulas;     // Ponteiro para o array de células/espaços
} Tabuleiro;             // Struct do tabuleiro


//                                       ------------------------------FROTA E NAVIOS------------------------

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


//                                       ----------------------------JOGADOR E JOGO---------------------------

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

#endif 

//                                         -----------------------TABULEIRO.H------------------------

#ifndef TABULEIRO_H
#define TABULEIRO_H


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


//                                      ----------------------------TABULEIRO.C----------------------------




#include "tipos.h"   
#include "tabuleiro.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>  



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

//                                                -------------------FROTA.H---------------------

#ifndef FROTA_H
#define FROTA_H



// Inicializa a frota padrão obrigatória 
Frota criar_frota_padrao();

// Libera a memória da frota 
void destruir_frota(Frota *f);

// Verifica se um navio específico foi afundado
int navio_esta_afundado(const Navio *navio);

// Verifica se toda a frota do jogador foi afundada
int frota_esta_afundada(const Frota *frota);

#endif



//                                           ----------------------FROTA.C---------------------------

#include "tipos.h"     
#include "frota.h"      
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>



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



//                                             -----------------------JOGO.H-------------------------

#ifndef JOGO_H
#define JOGO_H



// Inicializa a estrutura Jogo e seus componentes
void inicializar_jogo(Jogo *j, int tamanho_tabuleiro, const char *nick1, const char *nick2);

// Libera toda a memória alocada para o jogo
void destruir_jogo(Jogo *j);

// Posicionamento
void posicionar_frota_auto(Jogador *jogador, int tamanho);


// Função do tiro
int processar_tiro(Jogador *atirador, Jogador *alvo, int linha, int coluna);




#endif


//                                            -----------------------JOGO.C---------------------------



#include "tipos.h"
#include "tabuleiro.h"
#include "frota.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>


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




//                                           -------------------------MAIN.C----------------------------


#include "tipos.h"
#include "jogo.h"
#include "tabuleiro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void iniciar_loop_turnos(Jogo *j) {
    char coord_str[5];
    int linha, coluna;
    int resultado_tiro;
    
    while (j->fim_de_jogo == 0) {
        // Define Atirador e Alvo
        Jogador *atirador = (j->jogador_atual == 1) ? &j->j1 : &j->j2;
        Jogador *alvo = (j->jogador_atual == 1) ? &j->j2 : &j->j1;

        printf("Vez do %s (jogador %d)\n", atirador->apelido, j->jogador_atual);
        printf("============================================\n");
        
        // 1. Exibe os tabuleiros
        printf("--- Seu Tabuleiro (Navios e Acertos) ---\n");
        exibir_tabuleiro(&atirador->tabuleiro, 1);

        printf("--- Tabuleiro do Alvo (Seus Tiros) ---\n");
        exibir_tabuleiro(&atirador->tiros, 0);

        do {
            printf("\n%s, informe a coordenada do tiro: ", atirador->apelido);
            if (scanf("%4s", coord_str) != 1) {
                 
                while(getchar() != '\n');
                continue; 
            }
            // 2. Analisar e Validar Coordenada
            if (!analisar_coordenada(coord_str, &linha, &coluna, alvo->tabuleiro.linhas, alvo->tabuleiro.colunas)) {
                printf("Coordenada invalida ou fora dos limites! Tente novamente.\n");
                resultado_tiro = -99; 
            } else {
                // 3. Processar Tiro
                resultado_tiro = processar_tiro(atirador, alvo, linha, coluna);
            }

            // 4. Tratar o Resultado
            switch (resultado_tiro) {
                case 1:
                    printf(" ACERTOU! Tem direito a mais um tiro.\n");
                    
                    break;
                case -1:
                    printf(" AFUNDOU um navio! Tem direito a mais um tiro.\n");
                    
                    break;
                case -2:
                    printf(" VITÓRIA! %s afundou a frota inimiga. Fim de jogo! \n", atirador->apelido);
                    j->fim_de_jogo = 1;
                    resultado_tiro = 0; 
                    break;
                case 0:
                    
                    if (atirador->tiros.celulas[linha * atirador->tiros.colunas + coluna].estado == N_erro) {
                        printf(" ÁGUA! Errou o alvo. Passa o turno.\n");
                    } else {
                        printf(" Tiro repetido ou inválido. Escolha outra coordenada.\n");
                    }
                    break;
                case -100: 
                    break;
                default:
                    printf("Erro de lógica desconhecido.\n");
                    break;
            }
        } while (resultado_tiro == 1 || resultado_tiro == -1); 

        // 5. Alternar Jogador (Só se o jogo nao tiver terminado)
        if (j->fim_de_jogo == 0) {
            j->jogador_atual = (j->jogador_atual == 1) ? 2 : 1;
        }
    }
    printf("--- JOGO FINALIZADO ---\n");
}
void executar_novo_jogo() {
    Jogo jogo;
    char apelido1[50], apelido2[50];
    int tamanho_tabuleiro = 10; 
    char modo_posicionamento;

    printf("Informe nome ou apelido do Jogador 1: ");
    scanf("%s", apelido1);

    printf("Informe nome ou apelido do Jogador 2: ");
    scanf("%s", apelido2);
    
    // Configurações
    
    tamanho_tabuleiro = 10; 
    modo_posicionamento = 'A'; 

    inicializar_jogo(&jogo, tamanho_tabuleiro, apelido1, apelido2);
    
 
    if (modo_posicionamento == 'A' || modo_posicionamento == 'a') {
        posicionar_frota_auto(&jogo.j1, tamanho_tabuleiro);
        posicionar_frota_auto(&jogo.j2, tamanho_tabuleiro);
    } else {
        
        printf("Posicionamento manual nao implementado neste exemplo simplificado.\n");
        destruir_jogo(&jogo);
        return;
    }

    iniciar_loop_turnos(&jogo);

   
    destruir_jogo(&jogo);
}

int main() {
    int opcao;
    
    do {
        // 1. Menu Inicial 
        printf("\n=== BATALHA NAVAL ===\n");
        printf("1) Novo jogo\n");
        printf("2) Sair\n");
        
        
        if (scanf("%d", &opcao) != 1) {
            
            while(getchar() != '\n'); 
            opcao = 0;
            continue;
        }

        switch (opcao) {
            case 1:
                executar_novo_jogo();
                break;
            case 2:
                printf("Saindo do jogo. \n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 2);
    
    return 0;
}