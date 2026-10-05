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
void liberar_tabuleiro(char **tab);

// Algoritimo

int *conseguir_jogada(bool *jgd_finalizada);
bool jogada_valida(char **tab, int *jogada);
void realizar_jogada(char **tabuleiro, int *casas_l, int *jogada, int j_index, char *simbolos);
bool houve_ponto(char **tab, int j_index, char *simbs);
bool tabuleiro_cheio(int casas_livres);
void passar_turno(int *j_index);
void exibir_partida_result(int code, char j_nomes[][NOMES_TAM_MAX]);

// UX

void exibir_vez_jgdr(char jgdrs[][NOMES_TAM_MAX], int j_index);
void mostrar_tab(char **tabuleiro);

//Uteis

void linha(char simb, int quant, bool pular);
void alerta(char *menssagem);
void limpar();
void flush_in();

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
    bool jgd_finalizada; // verifica se o jogador fez a jogada como desejava
    bool jgd_valida; // Verifica se a jogada desejada é válida

    tab = tabuleiro_init(&casas_livres);
    tab_status = -1;
    
    while(tab_status != 0 && tab_status != 1 && tab_status != -2)
    {
        do
        {
            do
            {
                limpar();
                exibir_vez_jgdr(jgdrs, j_index);
                mostrar_tab(tab);
                jogada = conseguir_jogada(&jgd_finalizada);

            } while(!jgd_finalizada);

            if(jogada == NULL) // Jogada inválida
            {
                alerta("\nTente novamente.\n");
                continue;
            }

            // Verificando se a jogada é válida.
            jgd_valida = jogada_valida(tab, jogada);
            if(!jgd_valida)
            {
                alerta("Essa jogada não é válida. Casa já ocupada. Tente novamente.\n");
            }

            // Enquanto a jogada não for válida, repete.
        } while(jogada == NULL || !jgd_valida);

        // Realiza uma jogada válida.
        realizar_jogada(tab, &casas_livres, jogada, j_index, simbolos);

        tab_status = -1; // Por padrão, após uma jogada, o jogo continua se nou houver nada.
        // Definindo resultado da jogada
        if(casas_livres <= 4)
        {
            if(houve_ponto(tab, j_index, simbolos))
            {
                tab_status = j_index; // O jogador de índice j_index ganhou.
            }
            else if(tabuleiro_cheio(casas_livres))
            {
                tab_status = -2; // Deu velha. Empate.
            }
        }

        passar_turno(&j_index);

        if(tab_status != -1) //  Se a partida não deve continaur...
        {
            limpar();
            mostrar_tab(tab);
            // Exibe uma menssagem ao usuário dependendo do resultado da jogada.
            exibir_partida_result(tab_status, jgdrs);
        }
    }

    alerta("\nQue bela partida!\n\n");
    return 0;
}

/* Definições */

/********************************************
        Funções para o algorítimo
********************************************/


/*
Função que lê do usuário uma Jogada, isto é, uma dupla de números inteiros i e j, onde i representará a linha e j a coluna onde o jogador deseja inserir seu Simbolo marcador. Se o usuário digitar um valor inválido para a linha ou coluna, o vetor retornado é NULL, indicando a necessidade de releitura. Além disso, se a jogada for finalizada da maneira como o usuário desejava, o parâmetro jgd_finalizada = true, caso contrário, false.
*/
int *conseguir_jogada(bool *jgd_finalizada)
{
    int *jogada = calloc(2, sizeof(int));
    *jgd_finalizada = false;
    int input = -1;


    printf("\nDigite a linha que deseja marcar (0 para recomeçar jogada): ");
    scanf("%d", &input);
    flush_in();

    if(input == 0)
    {
        jgd_finalizada = false;
        return jogada = NULL;
    }

    if(input < 0 || input > LINHAS_NUM)
    {
        printf("Erro ao ler linha. Digite um número entre 1 e 3.\n");
        jogada = NULL;
        return jogada;
    }
    jogada[0] = input - 1;

    // Lê a coluna
    input = -1;
    printf("\nDigite a coluna que deseja marcar (0 para recomeçar jogada): ");
    scanf("%d", &input);
    flush_in();

    if(input == 0)
    {
        *jgd_finalizada = false;
        return jogada = NULL;
    }

    if(input < 0 || input > LINHAS_NUM)
    {
        printf("Erro ao ler coluna. Digite um número entre 1 e 3.\n");
        jogada = NULL;
        return jogada;
    }
    jogada[1] = input - 1;

    *jgd_finalizada = true;
    return jogada;
}


/*
Função que, dado um vetor Jogada (veja conseguir_jogada()), valida se o Jogador em questão PODE fazer aquela jogada. Isto é, verifica se naquela posição do tabuleiro há um simbolo inicial ou não. Retorna true caso positivo e false caso negativo
*/
bool jogada_valida(char **tab, int *jogada)
{
    if(tab[jogada[0]][jogada[1]] == INIT_SIMBOLO)
    {
        return true;
    }
    return false;
}


/*
Substitui, na posição indicada pelo vetor "jogada" (veja conseguir_jogada()), o tabuleiro com um símbolo de um jogador. O jogador é identificado em "jogador_index", com: [0] = J1 e [1] = J2. O vetor de símbolos possúi, para cada jogador e com indice respectivo, seu simbolo característico. Ao fim, decrementa a quantidade de casas livres.
*/
void realizar_jogada(char **tabuleiro, int *casas_l, int *jogada, int j_index, char *simbolos)
{
    tabuleiro[jogada[0]][jogada[1]] = simbolos[j_index];
    *casas_l = *casas_l - 1;
}


/*
Função que atualiza o index do jogador que fará a jogada. Passa para o próximo. Isto, é se o último a jogar foi o 0, atualiza para 1; se foi o 1, atualiza para 0.
*/
void passar_turno(int *j_index)
{
    if(*j_index == 0)
        *j_index = 1;
    else
        *j_index = 0;
}

/*
Função que verifica se houve ponto. Isto é, se - graças a última jogada - há agra no tabuleiro uma linha, coluna ou diagonal completa com o símbolo do jogador que fez a última jogada. Retorna true em caso positivo e false caso contrário.
*/
bool houve_ponto(char **tab, int j_index, char *simbs)
{
    int j_simb = simbs[j_index]; // Representa o caractere do jogador atual.
    bool ponto = false;

    int soma_l = 0;
    int soma_col = 0;
    int soma_dg_p = 0;
    int soma_dg_s = 0;
    // Verificando linhas
    for(int l = 0; l < LINHAS_NUM; l++)
    {
        for(int c = 0; c < COLUNAS_NUM; c++)
        {
            soma_l += tab[l][c];
            soma_col += tab[c][l];
            if(l == c)
            {
                soma_dg_p += tab[l][c];
            }

            if(l + c == LINHAS_NUM - 1)
            {
                soma_dg_s += tab[l][c];
            }
        }

        if(soma_l % j_simb == 0 || soma_col % j_simb == 0)
        {
            return true;
        }

        soma_l = 0; soma_col = 0;
    }

    // Se não houve ponto em nenhuma linha ou coluna, verifica as diagonais.
    if(soma_dg_p % j_simb == 0 || soma_dg_s % j_simb == 0)
    {
        return true;
    }

    return false;
}

/*
Função que verifica se todas as casas estão preenchidas.
*/
bool tabuleiro_cheio(int casas_livres)
{
    if(casas_livres <= 0)
    {
        return true;
    }
    return false;
}


/*
Exibe ao usuário uma menssagem sobre o resultado da última jogada. Isto é, se há um vencedor (e qual é este), se o jogo vai apenas continuar ou se deu empate (velha).
*/
void exibir_partida_result(int code, char j_nomes[][NOMES_TAM_MAX])
{
    if(code != -2)
    {
        printf("\nFim de jogo!!\n\nO vencedor é: %s !!!!", j_nomes[code]);
    }
    else
    {
        printf("\nFim de jogo!!\n\nDeu VELHA!");
    }
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
        Funções para UX (user experience)
********************************************/

/*
Função que exibe de qual jogador é a vez. jgdrs deve ser um array que contenha os nomes dos jogadores. j_index é o ídice do jogador da vez.
*/
void exibir_vez_jgdr(char jgdrs[][NOMES_TAM_MAX], int j_index)
{
    printf("Vez de: %s", jgdrs[j_index]);
    printf("\n\n");
}



/*
Função que printa o tabuleiro.
*/
void mostrar_tab(char **tabuleiro)
{
    for(int l = 0; l < LINHAS_NUM; l++)
    {
        printf(" ");
        for(int c = 0; c < COLUNAS_NUM; c++)
        {
            printf(" %c ", tabuleiro[l][c]);
            if(c < 2)
            {
                printf("|");
            }
        }
        printf("\n");

        if(l < 2)
        {
            linha('-', 13, true);
        }
    }
    printf("\n"); // Pulando linha no final
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


/*
Função que limpa tudo que houver no buffer de entrada (stdin)
*/
void flush_in()
{
    int ch;
    do
    {
        ch = fgetc(stdin);
    } while(ch != EOF && ch != '\n');
}


/*
Função que exibe uma menssagem de alerta passada como parâmetro. Em seguida, pede e espera até que o usuário aperte Enter para continuar.
*/
void alerta(char *menssagem)
{
    printf("\a%s", menssagem);
    printf("Pressione Enter para continuar.");

    int ch = getchar();
    if(ch != '\n')
    {
        flush_in();
    }
}
