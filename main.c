
//                                           -------------------------MAIN.C----------------------------

#include "jogo.h"
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