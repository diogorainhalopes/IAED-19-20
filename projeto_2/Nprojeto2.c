/*  
    Ficheiro: Nprojeto2.c
        Ficheiro principal do projeto que contem a função main.
        comandos descritos em 
        https://github.com/pedromigueladao/IAED1920/blob/master/projectos/p2/README.md
    Diogo Lopes - tp96732

    Desc: SISTEMA DE GESTÃO DE UMA BASE DE DADOS DE JOGOS DE FUTEBOL AMIGAVEIS. a interacao com
    o programa ocorre atraves de um conjunto de linhas compostas por uma letra (comando) e um
    numero de argumentos dependentes de cada comando
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Nestruturas.h"    /*  header que contem prototipos das 
                                funcoes auxiliares aos comandos principais */
#define MAX_TS 1013         /*  tamanho da hashtable */
#define T_NOME 1024         /*  tamanho maximo do nome do jogo e das equipas */
#define ARRAY_AUX 10000     /*  tamanho do array auxiliar da função g que tera os 
                                nomes das equipas */

/*  todas as funcoes recebem um numero inteiro que servira para indicar o numero da linha de input
    de cada comando quando uma funcao necessitar de dar um print (NL) */

/* funcao que adiciona um jogo ao sistema;  recebe um nome, duas equipas, e o resultado do jogo;
    e tambem dado um numero ao jogo para que seja possivel identificar a ordem de inserção de cada
    um. Consoante a equipa que ganhar adiciona uma vitoria a vencedora */
void a(link_j *jogos, link_e *equipas, int num, int nr_jogo)
{
    link_j xjogo;   /*  jogo que sera adicionada a hashtable */
    link_e e;       /*  variavel que servira para adicionar a vitoria a equipa vencedora */
    char nome[T_NOME], e1[T_NOME], e2[T_NOME];  /*  nome do jogo e das equipas */
    int g1, g2;     /* golos de cada equipa */
    scanf(" %[^:\n]:%[^:\n]:%[^:\n]:%d:%d", nome, e1, e2, &g1, &g2);

    if (procura_j_h(jogos, nome) != NULL)       /*  procura o jogo na hashtable e se nao existir */
    {                                           /*  retorna um erro */

        printf("%d Jogo existente.\n", num);
        return;
    }
    /* procura as equipas e se uma delas nao existir retorna um erro */
    if (procura_e_h(equipas, e1) == NULL)       
    {
        printf("%d Equipa inexistente.\n", num);
        return;
    }
    if (procura_e_h(equipas, e2) == NULL)
    {
        printf("%d Equipa inexistente.\n", num);
        return;
    }
    xjogo = Novo_j(nr_jogo, nome, e1, e2, g1, g2);  /*  cria uma novo jogo */
    insere_j(jogos, xjogo);                         /*  insere-o na hashtable */
    if (xjogo->golos_eq_1 > xjogo->golos_eq_2)      /*  verifica que equipa ganhou, procura-a */
    {                                               /*  dentro da hashtable, adiciona uma vitoria */
        e = procura_e_h(equipas, xjogo->eq1);       /*  ou nao adiciona nada no caso de empate */
        e->nr_vitorias += 1;
    }
    if (xjogo->golos_eq_1 < xjogo->golos_eq_2)
    {
        e = procura_e_h(equipas, xjogo->eq2);
        e->nr_vitorias += 1;

    }
    return;
}

/* funcao que cria uma equipa e adiciona-a a hashtable das equipas */
void A(link_e *equipas, int num)
{
    link_e e;   /* equipa que sera adicionada a hashtable */
    char nome[T_NOME];  /* nome da equipa */
    scanf(" %[^:\n]", nome);
    if (procura_e_h(equipas, nome) != NULL) /* procura a equipa e caso ja exista retorna erro */
    {
        printf("%d Equipa existente.\n", num);
        return;
    }
    e = Novo_e(nome);       /* e criada a equipa */
    insere_e(equipas, e);   /* e e inserida na hashtable */
}


/* funcao que procura um jogo na hastable e da print no terminal com o seguinte formato:
    "NL nome-jogo nome-equipa1 nome-equipa2 golos-equipa1 golos-equipa2" */
void p(link_j *jogos, int n)
{
    link_j j;           /* variavel auxiliar a procura */
    char nome[T_NOME];
    scanf(" %[^:\n]", nome);
    j = procura_j_h(jogos, nome);   /*  procura o jogo e caso nao exista da print a um erro */
    if (j == NULL)
    {
        printf("%d Jogo inexistente.\n", n);
        return;
    }
    else
    {
        printf("%d %s %s %s %d %d\n", n, j->nome_jogo, j->eq1, j->eq2, 
                                        j->golos_eq_1, j->golos_eq_2);
    }
}

/* funcao que procura uma equipa dado um nome e da print para o terminal com o nr de jogos ganhos */
void P(link_e *equipas, int n)
{
    link_e e;           /* variavel auxiliar a procura */
    char nome[T_NOME];
    scanf(" %[^:\n]", nome);
    e = procura_e_h(equipas, nome);
    if (e == NULL)      /* caso a procura retorne nula, e feito print de um erro */
    {
        printf("%d Equipa inexistente.\n", n);
        return;
    }
    else
    {
        printf("%d %s %d\n", n, e->nome_equipa, e->nr_vitorias);
    }
}


/*funcao que remove um jogo da hashtable dado um nome e reduz o numero de vitoria 
da equipa vencedora caso seja necessario */
void r(link_j *jogos, link_e *equipas, int n)
{   /* variaveis auxiliares a procura */
    link_e e;
    link_j j;
    char nome[T_NOME];
    scanf(" %[^:\n]", nome);
    j = procura_j_h(jogos, nome);           /*  caso a procura retorne nula */
    if (j == NULL)                          /*  e feito o print do erro */
    {
        printf("%d Jogo inexistente.\n", n);
        return;
    }
    if (j->golos_eq_1 > j->golos_eq_2)      /*  verifica se a equipa1 ganhou o jogo */
    {                                       /*  e retira-lhe uma vitoria */
        e = procura_e_h(equipas, j->eq1);
        e->nr_vitorias -= 1;
    }
    if (j->golos_eq_1 < j->golos_eq_2)      /* verifica se a equipa2 ganhou o jogo */
    {                                       /* e retira-lhe uma vitoria */
        e = procura_e_h(equipas, j->eq2);
        e->nr_vitorias -= 1;
                                            /*  no caso de o jogo ter sido um empate */
    }                                       /*  nao acontece nada */
    apaga_j_h(jogos, j);                    /*  por fim apaga o jogo da hashtable */

}

/* funcao que altera o resultado de um jogo e altera o numero de vitorias da equipa vencedora
    caso seja necessario */
void s(link_j *jogos, link_e *equipas, int n)
{
    /* variaveis auxiliares a funcao */
    int ng1, ng2, ri, nrf;  /* (int) por ordem: novos golos eq1, novos golos eq1,  */
    char nome[T_NOME];      /* resultado final inicial, novo resultado final  */
    link_e eq_a, eq_b;
    link_j j;
    scanf(" %[^:\n]:%d:%d", nome, &ng1, &ng2);
    j = procura_j_h(jogos, nome);           /*  caso o jogo que o resultado se quer alterar seja */
    if (j == NULL)                          /*  inexistente retorna erro */
    {
        printf("%d Jogo inexistente.\n", n);
        return;
    }
    nrf = ng1 - ng2;    /*  nrf tem a diferenca de golos das equipas no novo resultado */
    ri = (j->golos_eq_1 - j->golos_eq_2);   /*  ri tem adiferenca de golos do resultado inicial */
    eq_a = procura_e_h(equipas, j->eq1);    /*  equipa1 do jogo */
    eq_b = procura_e_h(equipas, j->eq2);    /*  equipa2 do jogo */
    j->golos_eq_1 = ng1;                    /*  altera o numero de golos da equipa1 */
    j->golos_eq_2 = ng2;                    /*  altera o numero de golos da equipa2 */
    if(ri > 0)  /*  caso ri seja positivo a equipa1 ganhou no jogo no resultado inicial */
    {
        if(nrf > 0) /*  caso nrf seja positivo a equipa1 ganhou no jogo no novo resultado */
        {           /*  e o numero de vitoria fica inalterado */
            return;
        }
        if(nrf == 0)    /*  caso nrf seja 0 o jogo fica empatado e o nr de vitorias da equipa1 */
        {               /*  diminui */
            eq_a->nr_vitorias -= 1;
            return;
        }
        if(nrf < 0) /*  caso nrf seja negativo a equipa2 ganhou o jogo no novo resultado */
        {           /* e retirada a vitoria a equipa1 e dada a equipa2 */
            eq_a->nr_vitorias -= 1;
            eq_b->nr_vitorias += 1;

            return;
        }
    }
    if((ri) == 0)   /*  caso ni seja 0 o jogo ficou empatado inicialmente */
    {
        if(nrf > 0) /*  caso nrf seja positivo a equipa1 e a vencedora no novo resultado e e lhe */
        {           /*  adicionada uma vitoria */
            eq_a->nr_vitorias += 1;
            return;
        }
        if(nrf == 0)    /*  se o nrf e 0 significa que o novo resultado tambem e um empate e nada */
        {               /*  e alterado */  
            return;
        }
        if(nrf < 0) /*  caso nrf seja negativo a equipa2 ganhou no jogo o novo resultado e e */
        {           /* lhe adicionada uma vitoria */
            eq_b->nr_vitorias += 1;
            return;
        }
    }
    if((ri) < 0)    /*  caso ri seja negativo a equipa2 ganhou no jogo no resultado inicial */
    {
        if(nrf > 0) /* se o novo resultado e negativo a vitoria e retirada a equipa2 e dada a */
        {           /* equipa1 */
            eq_b->nr_vitorias -= 1;
            eq_a->nr_vitorias += 1;
            return;
        }
        if(nrf == 0)    /* se o novo resultado e um empate o numero de vitorias da equipa2 e */
        {               /* diminuido em 1 */
            eq_b->nr_vitorias -= 1;
            return;
        }
        if(nrf < 0) /*  caso nrf seja negativo a equipa2 ganhou no jogo no novo resultado */
        {           /* e nada e alterado */
            return;
        }
    }
    
}

/* nesta funcao todos os jogo serao listados por ordem de entrada no sistema, com o formato de saida
"NL nome-jogo nome-equipa1 nome-equipa2 golos-equipa1 golos-equipa2" */
void l(link_j *jogos, int n, int nr_j)
{
    int w, i, k;    /*  variaves de iteracao da hashtable */
    link_j jogo;    /*  jogo auxiliar para a funcao printf */

    for(w = 0; w < MAX_TS; w++)     /*  iterar a hashtable por completo comecando na primeira */
    {                               /*  posicao, ou seja, hash = 0, e achando a "head" da lista */
        if(jogos[w] != NULL)        /*  ligada desse mesmo hash. Caso a head nao exista avança */
        {                           /*  para o proximo valor hash. A funcao acaba qnd o ultimo */
            break;                  /*  seja iterado por completo */
        }
        if(w == MAX_TS - 1)
        {
            return;
        }
    }
    for(i = 0; i < nr_j; i++)       /*  nr_j tem o numero do ultimo jogo inserido. Este ciclo */
    {                               /*  percorre todos os numeros ate chegar ao ultimo. cada jogo */
        for(k = 0; k < MAX_TS; k++) /*  possui um numero que lhe foi atribuido quando foi criado */
        {                           /*  de seguida e percorrida a hashtable e as listas liagadas */
            jogo = jogos[k];        /*  a procura do jogo com o numero de jogo que i */
            while(jogo!= NULL)
            {
                if(jogo->nr_jogo == i)
                {
                printf("%d %s %s %s %d %d\n", n, jogo->nome_jogo, jogo->eq1, jogo->eq2, 
                                                jogo->golos_eq_1, jogo->golos_eq_2);
                }
                jogo = jogo->next;
            }
        }
    }
}

/* nesta funcao sao listadas as equipas com o maior numero de vitorias por ordem lexicografica 
a listagem tem um cabeçalho "NL Melhores numero-de-vitorias" e de seguida "NL * nome-equipa"*/
void g(link_e *equipas, int n)
{
    char *v[ARRAY_AUX];     /*  vetor auxiliar de ponteiros de char que tera os nomes das equipas */
    link_e equipa_aux;      /*  com o maior numero de vitorias */
    int M = 0, i = 0, k = 0, w;

    for(w = 0; w < MAX_TS; w++) /*  ciclo que itera toda a hashtable pata verificar que exite pelo */
    {                           /*  menos uma equipa. Caso na exista a funcao acaba */
        if(equipas[w] != NULL)
        {
            break;
        }
        if(w == MAX_TS - 1)
        {
            return;
        }
    }

    for(w = 0; w < MAX_TS; w++)     /*  ciclo que procura o maior numero de vitorias iterando a */
    {                               /*  a hastable toda e todas as suas listas ligadas e sempre */
        equipa_aux = equipas[w];    /*  que e encontrada uma equipa com um numero de vitorias ao */
        while(equipa_aux != NULL)   /*  que estava guardado, este e substituido pelo maior */
        {
            if(equipa_aux->nr_vitorias > M)
            {
               M = equipa_aux->nr_vitorias;
            }
            equipa_aux = equipa_aux->next;
        }
    }

    for(w = 0; w < MAX_TS; w++)     /*  percorre a hastable toda a procura de todas as equipas */
    {                               /*  com o numero de vitorias igual ao encontrado no ciclo */
        equipa_aux = equipas[w];    /*  anterior */
        while(equipa_aux != NULL)   
        {
            if(equipa_aux->nr_vitorias == M)    /*  quando acha, o nome dessa equipa e armazenado */
            {                                   /*  dentro do vetor auxiliar */
                v[k] = malloc(sizeof(char) * (strlen(equipa_aux->nome_equipa) + 1));
                strcpy(v[k], equipa_aux->nome_equipa);
                k++;
            }
            equipa_aux = equipa_aux->next;
        }
    }
    qsort(v, k, sizeof(char*), sort);   /*  o qsort ira ordenar lexicograficamente os nomes das */
                                        /*  com o maior numero de vitorias */
    printf("%d Melhores %d\n", n, M);   /*  é feito o printf do cabeçalho das lista */
    for(i = 0; i < k; i++)              /*  este ciclo ira fazer print de todas as euipas ja */
    {                                   /*  ordenadas */
        printf("%d * %s\n", n, v[i]);   /*  a medida que sao feitos os prints, tambem e libertada */ 
        free(v[i]);                     /*  a memoria alocada para o vetor auxiliar */
    }
}

/* funcao principal com um switch para cada comando conforme o input no terminal.
aqui sao feitas as inicializacoes das hashtable e as suas libertaçoes */
int main()
{
    link_j *jogos = init_j();       /*  e alocada a memoria para a hashtable dos jogos */
    link_e *equipas = init_e();     /*  e alocada a memoria para a hashtable das equipas */
    char sel = getchar();
    int NL = 1;                     /*  variavel com o numero da linha do input */
    int nr_j = 0;                   /*  variavel com o numero do proximo jogo a ser inserido */
    while (sel != 'x')              /*  ate ser executado o comando x, o programa continua ativo */
    {			
        switch (sel) 			
        {
            case 'a':
                a(jogos, equipas, NL, nr_j);
                NL++; 
                nr_j++;
                break; 
            case 'l':
                l(jogos, NL, nr_j);
                NL++;
                break; 
            case 'p':
                p(jogos, NL);
                NL++;
                break;  
            case 'r':
                r(jogos, equipas, NL);
                NL++;
                break; 
            case 's':
                s(jogos, equipas, NL);
                NL++;
                break; 
            case 'A':
                A(equipas,  NL);
                NL++;
                break; 
            case 'P':
                P(equipas, NL);
                NL++;
                break; 
            case 'g':
                g(equipas, NL);
                NL++;
                break;  
            }
        sel = getchar();
    }
    destroi_h(jogos, equipas);      /*  quando o programa acaba a memoria de ambas as hashtable e */
    return 0;                       /*  libertada */
}

