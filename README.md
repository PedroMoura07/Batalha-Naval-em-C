-------------------Descrição do Projeto-------------------

Este projeto implementa uma versão totalmente funcional do clássico jogo Batalha Naval em linguagem C, utilizando conceitos fundamentais como:

- Estruturas (struct)

- Ponteiros

- Alocação dinâmica de memória

- Múltiplos arquivos (.h e .c)

- Enums

- Modularização do código

- Validação de entrada

- Tabuleiros independentes para navios e tiros

O jogo é jogado entre dois jogadores , com posicionamento automático dos navios.



Modelos de Dados (Design das Estruturas):
1. Estado das Células:

   typedef enum { N_agua, N_navio, N_acerto, N_erro } EstadoCelula;

2.Tabuleiro:
    
  typedef struct Tabuleiro {
    int linhas, colunas;
    N *celulas;
} Tabuleiro;

3.Navio:

typedef struct Navio {
    char nome[30];
    int comprimento;
    int acertos;
    int posicionado;
} Navio;

4.Frota de navios:

typedef struct Frota {
    Navio *navios;
    int contagem;
} Frota;

5. Jogador:

   typedef struct Jogador {
    Tabuleiro tabuleiro;
    Tabuleiro tiros;
    Frota frota;
    char apelido[50];
} Jogador;

6.Estrtura geral:

  typedef struct Jogo {
    Jogador j1, j2;
    int jogador_atual;
    int fim_de_jogo;
} Jogo;



------------------- Decisões de design----------------------

1.Uso de Enumerações
2.Tabuleiro como array linear
  
-Facilita alocação dinâmica e melhora desempenho.
Permite indexação simples e eficiente.

3.Dois tabuleiros por jogador

E separar:
-tabuleiro de navios
-mapa de tiros
-Evita que o jogador veja o posicionamento inimigo.

4.Posicionamento automático da frota

Implementado com:

-validação de espaço
-repetição até encontrar posição válida
-orientação horizontal/vertical aleatória


5.Validação robusta das coordenadas

-A função analisar_coordenada():
-garante formato correto
-valida limites
-impede erros de acesso inválido

6.Separação por módulos

O projeto segue uma arquitetura limpa:

-tabuleiro.c = gerenciamento do tabuleiro
-frota.c = criação e verificação dos navios
-jogo.c = mecânicas principais
-main.c = menu e loop do jogo
