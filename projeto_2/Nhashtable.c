/*  
    Ficheiro: Nhashtable.c
        Ficheiro secundario do projeto que contem as funcoes relacionadas com as hashtables.
    Diogo Lopes - tp96732
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Nestruturas.h"
#define MAX_TS 1013		/* 	tamanho da hashtable */
#define T_NOME 1024		/*  tamanho maximo do nome do jogo e das equipas */

/*	funcao responsavel de alocar a memoria necessaria para cada jogo e colocar 
os ponteiros com o endereço correto. recebe como argumentos todos os atribitos necessario: nr de 
inserçao do jogo, nome do jogo, nomes das equipas, e golos de cada equipa. e feito malloc para os 
ponteiros nome_jogo, eq1, eq2 e o da estrutura em si (link_j). Retorna o ponteiro do jogo*/
link_j Novo_j(int nr, char* nome, char* e1, char* e2, int g1, int g2)
{
	link_j x = (link_j) malloc(sizeof(struct node_j));
	x->nome_jogo = (char*) malloc(sizeof(char)*(strlen(nome)+1));
	strcpy(x->nome_jogo, nome);		/*  ponteiro nome_jogo tem agora o nome do jogo */
	x->eq1 = (char*) malloc(sizeof(char)*strlen(e1)+1);
	strcpy(x->eq1, e1);		/*  ponteiro eq2 tem agora o nome da equipa2 */
	x->eq2 = (char*) malloc(sizeof(char)*strlen(e2)+1);
	strcpy(x->eq2, e2);		/*  ponteiro eq2 tem agora o nome da equipa2 */
	x->nr_jogo = nr;		/* 	numero de insercao de cada jogo */
	x->golos_eq_1 = g1;		/* 	cada equipa fica com os respetivos golos marcados */
	x->golos_eq_2 = g2;
	x->next = NULL;			/*	o ponteiro nest e apenas inicializado */
	return x; 
}


/*	funcao responsavel de alocar a memoria necessaria para cada equipa e colocar 
os ponteiros com o endereço correto. recebe como argumentos todos o unico atribito necessario: 
nome da equipa. e feito malloc para o ponteiro nome_equipa e o da estrutura em si (link_j). 
Retorna o ponteiro do jogo*/
link_e Novo_e(char* nome)
{
	link_e x = (link_e) malloc(sizeof(struct node_e));
	x->nome_equipa = (char*) malloc(sizeof(char)*(strlen(nome)+1));
	strcpy(x->nome_equipa, nome);	/*  ponteiro nome_equipa tem agora o nome da equipa */
	x->nr_vitorias = 0;				/* 	numero de vitorias e inicializado a 0 */
	x->next = NULL;					/*	o ponteiro nest e apenas inicializado */
	return x;
}

/* funcao que coloca o jogo a ser inserido na lista ligada no inicio desta. */
link_j insere_inicio_j(link_j head, link_j new)
{
	new->next = head;	/* coloca o ponteiro next do novo jogo a apontar para a head da lista */
	return new;
}

/* funcao que coloca a equipa a ser inserida na lista ligada no inicio desta. */
link_e insere_inicio_e(link_e head, link_e new)
{
	new->next = head;	/* coloca o ponteiro next da nova equipa a apontar para a head da lista */
	return new;
}

/*	liberta os atributos do jogo que têm memoria alocada e do node da lista ligada */
void FREE_j(link_j j)
{
	free(j->nome_jogo);
	free(j->eq1);
	free(j->eq2);
	free(j);
}

/*	liberta os atributos da equipa que têm memoria alocada e do no da lista ligada */
void FREE_e(link_e e)
{
	free(e->nome_equipa);
	free(e);
}

/*	funcao que acha a posicao de um jogo dentro da lista ligada, muda os ponteiros dos nodes 
adjacentes e liberta o jogo */
link_j remove_j_l(link_j head, link_j j)
{
	link_j temp, prev;	/*	jogo temporario e o o jogo que se encontra antes na lista ligada */
	for (temp = head, prev = NULL; temp != NULL; prev = temp, temp = temp->next)
	{	/* o ciclo percorre a lista ligada com dois jogos, o que esta a ser avaliado se é o que 
			queremos eliminar e o anterior. quando o temp avanca para o seguinte, o prev toma o 
			valor do temp, iterando assim ate ao fim da lista */
		if(strcmp(key_j(temp), key_j(j)) == 0)	/* se os nomes forem iguais */
		{
			if(temp == head)		/*	se o jogo a remover for a head, basta colocar a head no */
				head = temp->next;	/*	a apontar para o seguinte jogo */
			else
				prev->next = temp->next;	/* 	se se enocontrar noutro lugar da lista, o */
			FREE_j(temp);					/*	ponteiro anterior comeca a apontar para o */
			return head;					/*	do a remover */
		}
	} 
	return head;					/*  retorna a head da lista ligada */
}

/*	funcao que percorre a lista ligada a procura do node com o atributo nome_jogo igual ao do
argumento da funcao e retorna o mesmo */
link_j procura_j_l(link_j head, char *v)
{
	link_j temp;
	for (temp = head; temp != NULL; temp = temp->next)
	{
		if(strcmp(key_j(temp), v) == 0)		/*  compara o ponteiro do temp com o argumento */
			return temp;
	}
	return NULL;
}

/*	funcao que percorre a lista ligada a procura do node com o atributo nome_equipa igual ao do
argumento da funcao e retorna o mesmo */
link_e procura_e_l(link_e head, char *v)
{
	link_e temp;
	for (temp = head; temp != NULL; temp = temp->next)
	{
		if(strcmp(key_e(temp), v) == 0)		/*  compara o ponteiro do temp com o argumento */
			return temp;
	}
	return NULL;
}

/*	funcao que faz o hash para ambas as hashtables	*/
int hash(char *v) 
{
	int h, a = 161803, b = 31415;

	for (h = 0; *v != '\0'; v++, a = a*b % (MAX_TS-1))
	{
		h = (a*h + *v) % MAX_TS;
	}
	return h;
}

/*	funcao que inicializa a hashtable para os jogos, colocando a nule todos os seus indices */
link_j* init_j()
{
	int i;
	link_j *heads = malloc(MAX_TS*sizeof(struct node_j));
	for ( i = 0; i < MAX_TS; i++)
	{
		heads[i] = NULL;
	}
    return heads;
}

/*	funcao que inicializa a hashtable para as equipas, colocando a nule todos os seus indices */
link_e* init_e()
{
	int i;
	link_e *heads = malloc(MAX_TS*sizeof(struct node_e));
	for ( i = 0; i < MAX_TS; i++)
	{
		heads[i] = NULL;
	}
    return heads;
}

/*	funcao que itera por todas as posicoes da hashtable dos jogos chamando outra 
funcao para apagar cada uma das listas ligadas */
void destroi_j(link_j *heads)
{
	int i;
    for ( i = 0; i < MAX_TS; i++)
	{
		destroi_l_j(heads[i]);
	}
	free(heads);
}

/*	funcao que itera por todas as posicoes da lista ligada libertando cada um dos 
nodes dos jogos */
void destroi_l_j(link_j j) {
	link_j n_aux, aux;
	if (!j) return;
	for (n_aux = j, aux = j->next; n_aux; aux = n_aux->next, FREE_j(n_aux), n_aux = aux){}
}

/*	funcao que itera por todas as posicoes da hashtable das equipas chamando outra 
funcao para apagar cada uma das listas ligadas */
void destroi_e(link_e *heads)
{
	int i;
    for ( i = 0; i < MAX_TS; i++)
	{
		destroi_l_e(heads[i]);
	}
	free(heads);
}

/*	funcao que itera por todas as posicoes da lista ligada libertando cada um dos 
nodes das equipas */
void destroi_l_e(link_e e) {
	link_e n_aux, aux;
	if (!e) return;
	for (n_aux = e, aux = e->next; n_aux; aux = n_aux->next, FREE_e(n_aux), n_aux = aux){}
}

/* funcao que junta outras duas funcoes para destruir ambas as hashtables */
void destroi_h(link_j *head_j, link_e *head_e)
{
    destroi_j(head_j);
	destroi_e(head_e);
}

/*	efetua a funcao hash para colocar o jogo no respetivo indice e 
coloca o no inicio da lista ligada */
void insere_j(link_j *hasht_j, link_j new)
{
	int i = hash(new->nome_jogo);
	hasht_j[i] = insere_inicio_j(hasht_j[i], new);
}

/*	efetua a funcao hash para colocar a equipa no respetivo indice e 
coloca a no inicio da lista ligada */
void insere_e(link_e *hasht_e, link_e new)
{
	int i = hash(new->nome_equipa);
	hasht_e[i] = insere_inicio_e(hasht_e[i], new);
}

/*	efetua a funcao hash para achar o seu indice na hashtable e 
chama a funcao para o remover de dentro da lista ligada */
void apaga_j_h(link_j *hasht_j, link_j j)
{
	int i = hash(key_j(j));
	hasht_j[i] = remove_j_l(hasht_j[i], j);
}

/*	efetua a funcao hash para achar o seu indice na hashtable e 
chama a funcao para o procurar dentro da lista ligada. retorna o jogo */
link_j procura_j_h(link_j *hasht_j, char *v)
{
	int i = hash(v);
	link_j j = procura_j_l(hasht_j[i], v);
	return j;
}

/*	efetua a funcao hash para achar o seu indice na hashtable e 
chama a funcao para o procurar dentro da lista ligada. retorna a equipa */
link_e procura_e_h(link_e *hasht_e, char *v)
{
	int i = hash(v);
	link_e e = procura_e_l(hasht_e[i], v);
	return e;
}


/*	funcao auxiliar ao qsort. faz as comparacoes entre os ponteiros dos nomes das equipas */
int sort(const void* e1, const void* e2)
{
	
	const char** na = (const char **)e1;
	const char** nb = (const char **)e2;
	return strcmp(*na, *nb);

}



