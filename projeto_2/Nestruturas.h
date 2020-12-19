/*  estrutura de um jogo, contém a sua posicao na "fila" de insercao, o nome, as duas equipas, 
os golos de ambas equipas e um ponteiro para o proximo jogo */
typedef struct node_j* link_j;

struct node_j
{
	int nr_jogo;
	char *nome_jogo;
	char *eq1;
	char *eq2;
	int golos_eq_1;
	int golos_eq_2;
	struct node_j *next;
}node_j;

/*  estrutura de uma equipa, contém o nome, o numero de vitorias da equipa e um ponteiro 
para a proxima equipa */
typedef struct node_e* link_e;

struct node_e
{
	char *nome_equipa;
	int nr_vitorias;
	struct node_e *next;
}node_e;

/*	estruturas auxiliares que retornam o nome de um jogo ou equipa dado um node */
typedef char* Key;
#define key_j(A) (A->nome_jogo)
#define key_e(A) (A->nome_equipa)

/* prototipos das funcoes relacionadas as hashtables */
link_e Novo_e(char* nome);
link_j Novo_j(int nr_j, char* nome, char* e1, char* e2, int g1, int g2);
link_j insere_inicio_j(link_j head, link_j new);
link_e insere_inicio_e(link_e head, link_e new);
void FREE_j(link_j j);
void FREE_e(link_e e);
link_j remove_j_l(link_j head, link_j j);
link_j procura_j_l(link_j j, char *v);
link_e procura_e_l(link_e e, char *v);
int hash(char *v) ;
link_j* init_j();
link_e* init_e();
void destroi_j(link_j *heads);
void destroi_l_j(link_j j);
void destroi_e(link_e *heads);
void destroi_l_e(link_e e);
void destroi_h(link_j *head_j, link_e *head_e);
void insere_j(link_j *hasht_j, link_j new);
void insere_e(link_e *hasht_e, link_e new);
void apaga_j_h(link_j *hasht_j, link_j j);
link_j procura_j_h(link_j *hasht_j, char *v);
link_e procura_e_h(link_e *hasht_e, char *v);
int sort(const void* e1, const void* e2);