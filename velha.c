#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/* Defines */

#define LINHAS_NUM 3
#define COLUNAS_NUM 3
#define INIT_SIMBOLO '+'

#define NOMES_TAM_MAX 15
#define J1_NOME "Jogador 1"
#define J2_NOME "Jogador 2"
#define J1_SIMBOLO 'X'
#define J2_SIMBOLO 'O'

/* Protótipos */

// Estruturas

char **tabuleiro_init(int *casas_livres);
void mostrar_tab(char **tabuleiro);
void liberar_tabuleiro(char **tab);

// Algoritimo

int *conseguir_jogada();
bool validar_jogada(int *jogada);
void realizar_jogada(char **tabuleiro, int *casa_livres, int *jogada, int j_index, char *simbolos);
int ler_tabuleiro(char **tabuleiro, int casas_livres, int j_index);
void exibir_jogada_result(int code, char j_nomes[][NOMES_TAM_MAX]);

//Uteis

void linha(char simb, int quant, bool pular);

/* Main */

int main()
{
    
    char jgdrs[][NOMES_TAM_MAX] = {J1_NOME, J2_NOME}; // Array com nome dos jogadores
    char simbolos[] = {J1_SIMBOLO, J2_SIMBOLO};
    int j_index = 0; // Index do jogador da vez. O jogador 1[0] começa.
    
    char **tab; // Representará o tabuleiro
    int casas_livres;
    int tab_status; // Definirá se o jogo deve continuar ou acabar
    int *jogada;

    tab = tabuleiro_init(&casas_livres);
    tab_status = -1;
    
    while(tab_status != 0 && tab_status != 1 && tab_status != -2)
    {
        mostrar_tab(tab);
        jogada = conseguir_jogada();

        if(validar_jogada(jogada))
        {
            realizar_jogada(tab, &casas_livres, jogada, j_index, simbolos);
            tab_status = ler_tabuleiro(tab, casas_livres, j_index);
            exibir_jogada_result(tab_status, jgdrs);
        }
    }



    return 0;
}

/* Definições */

/********************************************
        Funções para o algorítimo
********************************************/

/*
Função que printa o tabuleiro.
*/
void mostrar_tab(char **tabuleiro)
{
    for(int l = 0; l < LINHAS_NUM; l++)
    {
        printf("|");
        for(int c = 0; c < COLUNAS_NUM; c++)
        {
            printf(" %c |", tabuleiro[l][c]);
        }
        printf("\n");
        linha('-', 13, true);
    }
}


/*
Função que lê do usuário uma Jogada, isto é, uma dupla de números inteiros i e j, onde i representará a linha e j a coluna onde o jogador deseja inserir seu Simbolo marcador. Essa função é responsável por ler valores válidos para i e j mas sem se preocupar com a configuração o tabuleiro. A função retorna um vetor jogada[2], onde [0] = i e [1] = j.
*/
int *conseguir_jogada()
{

}


/*
Função que, dado um vetor Jogada (veja conseguir_jogada()), valida se o Jogador em questão PODE fazer aquela jogada. Isto é, verifica se naquela posição do tabuleiro há um simbolo inicial ou não. Retorna true caso positivo e false caso negativo
*/
bool validar_jogada(int *jogada)
{

}


/*
Substitui, na posição indicada pelo vetor "jogada" (veja conseguir_jogada()), o tabuleiro com um símbolo de um jogador. O jogador é identificado em "jogador_index", com: [0] = J1 e [1] = J2. O vetor de símbolos possúi, para cada jogador e com indice respectivo, seu simbolo característico. Ao fim, decrementa a quantidade de casas livres.
*/
void realizar_jogada(char **tabuleiro, int *casa_livres, int *jogada, int jogador_index, char *simbolos)
{

}


/*
Função que atualiza o index do jogador que fará a jogada. Passa para o próximo. Isto, é se o último a jogar foi o 0, atualiza para 1; se foi o 1, atualiza para 0.
*/
void passar_turno(int *j_index)
{

}


/*
Verifica se há no tabuleiro uma coluna, linha ou diagonal completa com o mesmo símbolo significando um Ponto. Se não houve Ponto, verifica a quantidade de casas livres para determinar ou não um Empate. 

Retorno (código de status): 
    - Se houve Ponto, retorna o indice do jogador que fez a última jogada. (Vencedor)
    - Se não houve Ponto e restam casas, retorna -1. (Continuar)
    - Se não houve Ponto e acabaram-se as casas, retorna -2 (deu Velha ou empate).
*/
int ler_tabuleiro(char **tabuleiro, int casas_livres, int jogador_index)
{

}


/*
Exibe ao usuário uma menssagem sobre o resultado da última jogada. Isto é, se há um vencedor (e qual é este), se o jogo vai apenas continuar ou se deu empate (velha).
*/
void exibir_jogada_result(int code, char j_nomes[][NOMES_TAM_MAX])
{

}


/********************************************
 Funções para as estruturas
 ********************************************/

/*
Função que inicializa o tabuleiro - uma matriz de caracteres - preenchendo cada uma de suas células com o símbolo inicial. Além disso, modifica o valor em "casas_livres" para o total de casas inicializadas.

Retorno:
    - Retorna o ponteiro para a matriz tabuleiro inicializada.
*/
char **tabuleiro_init(int *casas_livres)
{
    // ALocando linhas (vetores de caracteres)
    char **tab = calloc(LINHAS_NUM, sizeof(char*));
    if(tab == NULL)
    {
        printf("Erro ao alocar o tabuleiro.\n");
        exit(1);
    }

    for(int l = 0; l < LINHAS_NUM; l++)
    {
        // Alocando cada vetor de caractere com suas colunas
        tab[l] = calloc(COLUNAS_NUM, 1);
        if(tab[l] == NULL)
        {
            printf("Erro ao alocar vetores e colunas do tabuleiro.\n");
            exit(1);
        }
        for(int c = 0; c < COLUNAS_NUM; c++)
        {
            tab[l][c] = INIT_SIMBOLO;
        }
    }

    *casas_livres = LINHAS_NUM*COLUNAS_NUM;
    return tab;
}


/*
Libera a matriz tabuleiro
*/
void liberar_tabuleiro(char **tab)
{
    for(int l = 0; l < LINHAS_NUM; l++)
    {
        free(tab[l]);
    }
    free(tab);
    tab = NULL;
}

/********************************************
            Funções uteis
********************************************/

/*
Função que escre o caractere passado em "simb" "quant" vezes. O parâmetro pular determina se, após a escrita da linha, deve-se adicinar o "\n".
*/
void linha(char simb, int quant, bool pular)
{
    for(int i = 0; i < quant; i++)
    {
        printf("%c", simb);
    }
    if(pular)
    {
        printf("\n");
    }
}

/*
Função que limpa o terminal
*/
void limpar()
{
    printf("\033c");
}