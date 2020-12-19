/****************************************/
/*		FILE: ProjetoIAED1920.c			*/
/*	   autor: Diogo Lopes - 96732		*/
/*	Sistema de logistica para produtos	*/
/****************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	MAX_ID		10000	/*	numero maximo de produtos no sistema */
#define	MAX_INFO	64		/*	numero maximo de caracteres na descricao */
#define MAX_ORD 	500		/*	numero maximo de encomendas */
#define MAX_ORDQ	200		/* 	numero maximo de produtos numa encomenda */

/*		variavei globais			    */

int act = 0;				/*	numero de produtos no Sistema */
int order_act = 0;			/*	numero total de encomendas */


/*		estrutura para os produtos		*/

typedef struct
{
	int		prod_id;				/*	id do produto dentro do Sistema */
	char	prod_desc[MAX_INFO];	/*	descricao do produto */
	int 	prod_price;				/*	preco do produto */
	int 	prod_w;					/*	peso de uma unidade do produto */
	int 	prod_st;				/*	stock do produto no Sistema */
} prod;

/*	estrutura para as encomendas */

typedef struct
{
	int order_id;					/*	id da encomenda dentro do Conjunto de Encomendas (CdE) */
	int order_w;					/*	peso da encomenda */
	int order_prods[MAX_ORDQ];		/*	lista com os produtos da encomenda */
	int order_qnt[MAX_ORDQ];		/*	lista com a quantidade de cada produto (a quantidade de cada 
										produto tem o mesmo indice que na lista com os produtos da encomenda) */
	int order_prod_act;				/*	numero de produtos em cada encomenda */
} order;


/*	auxiliares */

/*	encontrar o indice da encomenda no CdE */
int existE(int a, int active_E, int ide, order t[])
{
	while (a < active_E)
	{
		if (ide == t[a].order_id)
			break;
		a++;
	}
	return a;
}

/*	encontrar o indice do produto no Sistema */
int existP(int b, int active_P, int idp, prod v[])
{
	while (b < active_P)
	{
		if (idp == v[b].prod_id)
			break;
		b++;
	}
	return b;
}

/*  encontrar o indice do produto dentro da encomenda */
int existPinE(int c, int active_P_E, int idp, int E, order t[])
{
	while (c < active_P_E)
	{
		if (idp == t[E].order_prods[c])
			break;
		c++;
	}
	return c;
}


	
/*  algoritmos de ordenacao */

void insertionSort( prod vetor[], int l, int r)
{
	int a, b;
    
    for(a = l + 1; a <= r; a++)
    {
        prod v;
        strcpy(v.prod_desc, vetor[a].prod_desc);
        v.prod_price = vetor[a].prod_price;
        v.prod_st = vetor[a].prod_st;
        
        b =  a - 1;
        /*  move os produtos cujo preco seja superior ao do produto v para a posicao seguinte */
        while(b >= l && v.prod_price < vetor[b].prod_price)
        {
            strcpy(vetor[b + 1].prod_desc, vetor[b].prod_desc);
            vetor[b + 1].prod_price = vetor[b].prod_price;
            vetor[b + 1].prod_st = vetor[b].prod_st;
            b--;
        }
        /*  quando achar um preco inferior ao de v, essa sera a posicao de v */
        vetor[b + 1].prod_price = v.prod_price; 
        strcpy(vetor[b + 1].prod_desc, v.prod_desc);
        vetor[b + 1].prod_st = v.prod_st;
    }
}


/******************************************/

int partition(prod a[], int l, int r)
{
	int i = l - 1;
	int j = r;
	prod v;
	prod ta;
	prod tb;

	v.prod_id = a[r].prod_id; /*  escolher um pivot */
	strcpy(v.prod_desc, a[r].prod_desc);
	v.prod_price = a[r].prod_price;
	v.prod_st = a[r].prod_st;

	while (i < j)											/*  enquanto o iterador da esqurda for inferior ao da esquerda */
	{
		while(strcmp(a[++i].prod_desc, v.prod_desc) < 0);	/*  enquanto o produto do iterador da esquerda for alfabeticamente inferior ao do pivot */
		while(strcmp(v.prod_desc, a[--j].prod_desc) < 0)	/*  enquanto o produto do iterador da direita for alfabeticamente inferior ao do pivot */
		{
			if (j == l)										/*  se o iterador da direita for igual ao da esquerda, para */
				break;
		}
		if (i < j)	/* efetua a troca dos atributos */
		{
			ta.prod_id = a[i].prod_id;
			strcpy(ta.prod_desc, a[i].prod_desc);
			ta.prod_price = a[i].prod_price;
			ta.prod_st = a[i].prod_st;

			a[i].prod_id = a[j].prod_id;
			strcpy(a[i].prod_desc, a[j].prod_desc);
			a[i].prod_price = a[j].prod_price;
			a[i].prod_st = a[j].prod_st;

			a[j].prod_id = ta.prod_id;
			strcpy(a[j].prod_desc, ta.prod_desc);
			a[j].prod_price = ta.prod_price;
			a[j].prod_st = ta.prod_st;
		}
	}			/*  efetua a troca de forma a que o o produto da posicao i fique na posicao final */															
	tb.prod_id = a[i].prod_id;
	strcpy(tb.prod_desc, a[i].prod_desc);
	tb.prod_price = a[i].prod_price;
	tb.prod_st = a[i].prod_st;

	a[i].prod_id = a[r].prod_id;
	strcpy(a[i].prod_desc, a[r].prod_desc);
	a[i].prod_price = a[r].prod_price;
	a[i].prod_st = a[r].prod_st;

	a[r].prod_id = tb.prod_id;
	strcpy(a[r].prod_desc, tb.prod_desc);
	a[r].prod_price = tb.prod_price;
	a[r].prod_st = tb.prod_st;

	return i;	/*  retorna o i de forma a ser usado na recursao */ 
}

void quicksort(prod a[], int l, int r)
{
	int i;
	if (r <= l)
		return;
	i = partition(a, l, r);
	quicksort(a, l, i-1);
	quicksort(a, i+1, r);
}

/******************************************/


/* 		funcoes							*/

/*	adicionar um produto ao Sistema */
void addProd(prod v[])
{	
	scanf(" %[^:]:%d:%d:%d" ,v[act].prod_desc, &v[act].prod_price, &v[act].prod_w, &v[act].prod_st);
	v[act].prod_id = act;					/*	o id do produto sera igual ao numero de produtos que havia ate este ser criado */
	printf("Novo produto %d.\n", v[act].prod_id);
	act++;									/* o numero de produtos do Sistema incrementa 1 */
}


/*	adicionar mais unidades de um produto ao Sistema */
void addStock(prod v[])
{
	int idp, qtd, PP;					
	int e = 0;
	scanf("%d:%d",&idp, &qtd);

	PP = existP(e, act, idp, v);										/*	PP sera o indice do produto dentro do Sistema */

	if (idp != v[PP].prod_id || (strcmp(v[PP].prod_desc, "") == 0))		/*	caso o produto nao exista o funcao acaba */
	{
		printf("Impossivel adicionar produto %d ao stock. Produto inexistente.\n", idp);
		return;
	}
	else
	{
		v[PP].prod_st += qtd;
		return;
	}
}


/*  adicionar uma encomenda ao CdE */
void newOrder(order t[])
{
	t[order_act].order_id = order_act;					/*	o id da encomenda sera igual ao numero de encomendas que havia ate esta ser criado */
	t[order_act].order_w = 0;							/*	inicializa o peso da encomneda a 0 */
	t[order_act].order_prod_act = 0;					/*  inicializa o numero de produtos na encomenda a 0 */
	printf("Nova encomenda %d.\n", t[order_act].order_id);
	order_act++;										/*	o numero de encomendas no CdE incrementa 1 */
}


/*	adicionar um produto a uma encomenda */
void addProdToOrder(prod v[],order t[])
{
	int i = 0, e = 0, o = 0;
	int EE, PP, LL;
	int ide, idp, qtd;
	scanf("%d:%d:%d",&ide, &idp, &qtd);

	EE = existE(i, order_act, ide, t);						/*	EE sera o indice da encomenda dentro do CdE */
	
	if (ide != t[EE].order_id || order_act == 0)		
	{
		printf("Impossivel adicionar produto %d a encomenda %d. Encomenda inexistente.\n", idp, ide);
		return;
	}

	PP = existP(e, act, idp, v);								/*  PP sera o indice do produto dentro do Sistema */

	if (idp != v[PP].prod_id || (strcmp(v[PP].prod_desc, "") == 0))
	{
		printf("Impossivel adicionar produto %d a encomenda %d. Produto inexistente.\n", idp, ide);
		return;
	}

	if (v[PP].prod_st < qtd)										/* verifica se ha quantidade suficiente no sistema para adicionar a encomenda */
	{
		printf("Impossivel adicionar produto %d a encomenda %d. Quantidade em stock insuficiente.\n", idp, ide);
		return;
	}
	
	if ((v[PP].prod_w * qtd) + t[EE].order_w > MAX_ORDQ)			/*	se o peso do produto * a quantidade a adicionar + o peso existente na encomenda 
																	    for superior ao peso maximo possivel, da erro */
	{															
		printf("Impossivel adicionar produto %d a encomenda %d. Peso da encomenda excede o maximo de 200.\n", idp, ide);
		return;
	}

	LL = existPinE(o, t[EE].order_prod_act, idp, EE, t);			/*	LL sera o indice do produto dentro da encomenda */	

	/* adicionar os atributos do produto ao seu respetivo indice dentro do indice LL da encomenda */ 
	if (v[PP].prod_id == t[EE].order_prods[LL])						/*	verificar se o produto ja existe na encomenda */
	{	
		t[EE].order_qnt[LL] += qtd;
		t[EE].order_w += (v[PP].prod_w * qtd);
		v[PP].prod_st -= qtd; 
		t[EE].order_prod_act++;
	}
	else
	{ 

		t[EE].order_prods[t[EE].order_prod_act] = v[PP].prod_id;
		t[EE].order_qnt[t[EE].order_prod_act] += qtd;
		t[EE].order_w += (v[PP].prod_w * qtd);
		v[PP].prod_st -= qtd;
		t[EE].order_prod_act++; 
	}
	
} 	


/*	remover uma certa quantidade de um produto no Sistema */
void removeStockProd(prod v[])
{
	int idp, qtd;
	int PP, e = 0;
	scanf("%d:%d",&idp, &qtd);

	PP = existP(e, act, idp, v);

	if (idp != v[PP].prod_id || (strcmp(v[PP].prod_desc, "") == 0))
	{
		printf("Impossivel remover stock do produto %d. Produto inexistente.\n", idp);
		return;
	}
	
	if ((v[PP].prod_st - qtd) < 0)			/*	verificar se a quantidade que se quer remover e superior a quantidade existente */
	{
		printf("Impossivel remover %d unidades do produto %d do stock. Quantidade insuficiente.\n", qtd, idp);
		return;
	}
	else
		(v[PP].prod_st -= qtd);				/* se houver, reduz a quantidade */
}


/*	remover um produto de uma encomenda */
void removeProdFromOrder(prod v[], order t[])
{
	int EE, PP, LL, ide, idp, u;
	int i = 0, e = 0, o = 0;
	scanf("%d:%d",&ide, &idp);

	EE = existE(i, order_act, ide, t);
	
	if (ide != t[EE].order_id || order_act == 0)
	{
		printf("Impossivel remover produto %d a encomenda %d. Encomenda inexistente.\n", idp, ide);
		return;
	}

	PP = existP(e, act, idp, v);

	if (idp != v[PP].prod_id || (strcmp(v[PP].prod_desc, "") == 0))
	{
		printf("Impossivel remover produto %d a encomenda %d. Produto inexistente.\n", idp, ide);
		return;
	}

	LL = existPinE(o, t[EE].order_prod_act, idp, EE, t);

	if (idp != t[EE].order_prods[LL] || t[EE].order_prod_act == 0)		/*	se nao houver o produto na encomenda, para */
		return;
	
	else
	{
		v[PP].prod_st += t[EE].order_qnt[LL]; 							/*	repoe a quantidade que estava na encomenda no Sistema */
		t[EE].order_w -= (v[PP].prod_w * t[EE].order_qnt[LL]);			/*	reduz o peso da encomenda */

		for (u = LL; u < t[EE].order_prod_act + 1; u++)
		{

			t[EE].order_prods[u] = t[EE].order_prods[u + 1];			  /*  desloca os produtos a seguir ao que se pretende retirar um indice para tras 
																			  de forma sobrepor o produto no indice LL */
			t[EE].order_qnt[u] = t[EE].order_qnt[u + 1];				  /*  desloca as quantidades a seguir a que se pretende retirar um indice para tras
																			  de forma sobrepor o produto no indice LL e para manter os indices iguais em ambas funcoes */
		}
	}	
}


/*  calcular o custo de uma encomenda */
void costOrder(prod v[], order t[])
{
	int EE, x, ide;
	int total = 0, i = 0;	/*  total e a variavel que vai conter a soma total de o custo de cada produto * a sua quantidade dentro da encomenda */
	scanf("%d", &ide);

	EE = existE(i, order_act, ide, t);

	if (ide != t[EE].order_id || order_act == 0)
	{
		printf("Impossivel calcular custo da encomenda %d. Encomenda inexistente.\n", ide);
		return;
	}
	else
	{	
		{
			for (x = 0; x < t[EE].order_prod_act; x++)					/*  itera todos os produtos dentro da encomenda e rastreia o Sistema a procura do preco
																			que corresponde ao cada um dos produtos, somadando ao total */
			{
				total += (t[EE].order_qnt[x] * v[t[EE].order_prods[x]].prod_price);
			}
		}
		printf("Custo da encomenda %d %d.\n", ide, total);
	}
}


/*  mudar o preco de um produto no Sistema */
void changeProdPrice(prod v[])
{
	int idp, preco, PP;
	int e = 0;
	scanf("%d:%d", &idp, &preco);

	PP = existP(e, act, idp, v);

	if (idp != v[PP].prod_id || (strcmp(v[PP].prod_desc, "") == 0))
	{
		printf("Impossivel alterar preco do produto %d. Produto inexistente.\n", idp);	
		return;
	}
	else
	{
		v[PP].prod_price = preco;
	}
}


/*  retorna a descricao e a quantidade de um produto numa encomenda */
void listProdQ(prod v[], order t[])
{
	int EE, PP, LL, ide, idp;
	int i = 0, e = 0, o = 0;
	scanf("%d:%d",&ide, &idp);

	EE = existE(i, order_act, ide, t);
	
	if (ide != t[EE].order_id || order_act == 0)
	{
		printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ide);
		return;
	}

	PP = existP(e, act, idp, v);

	if (idp != v[PP].prod_id || (strcmp(v[PP].prod_desc, "") == 0))
	{
		printf("Impossivel listar produto %d. Produto inexistente.\n", idp);
		return;
	}

	LL = existPinE(o, t[EE].order_prod_act, idp, EE, t);

	if (t[EE].order_prods[LL] != v[PP].prod_id)						/*	se o produto nao existir na encomenda imprime a descricao e a quantididade 0 */
		printf("%s %d.\n", v[PP].prod_desc, 0);	
	else	
		printf("%s %d.\n", v[PP].prod_desc, t[EE].order_qnt[LL]);	/*  se existir imprime a descricao e a quantidade respetiva na encomenda */
}


/*  retorna a encomenda com a maior quantidade de um certo produto */
void maxProd(prod v[], order t[])
{
	int PP, i, x, idp;
	int e = 0;
	int idMin = MAX_ORD;		/*  inicializa o id da encomenda no maximo possivel */
	int qntMax = 0;				/*  inicializa a quantidade no minimo */
	scanf("%d", &idp);

	PP = existP(e, act, idp, v);

	if (idp != v[PP].prod_id || (strcmp(v[PP].prod_desc, "") == 0))
	{
		printf("Impossivel listar maximo do produto %d. Produto inexistente.\n", idp);
		return;
	}

	if (order_act == 0)			/* se nao exixtirem encomendas a funcao acaba */
		return;

	for (i = 0; i < order_act; i++)						/*	itera todas as encomendas */
	{
		for(x = 0; x < t[i].order_prod_act; x++)		/*  itera todos os produtos na encomenda */
		{
			if (idp == t[i].order_prods[x] && qntMax < t[i].order_qnt[x])	/* se achar o produto dentro da encomenda, verifica se a quantidade e superior */
			{
				idMin = t[i].order_id;					/*  o id mais baixo e atualizado para o mesmo da quantidade pois o ciclo itera sempre do id menor para o maior */
				qntMax = t[i].order_qnt[x];				/*  a quantidade maxima e atualizada para a do produto dentro da encomenda */
			}
		}
	}	
	if (qntMax == 0)									/*  se a quantidade maxima se manter a 0 o produto nao esta em nenhuma encomenda */
		return;
	printf("Maximo produto %d %d %d.\n", idp, idMin, qntMax);
}


/* da output a uma lista com a descricao, preco e stock disponivel de cada produto do sistema */
void listProdInSys(prod v[])
{
	prod r[MAX_ID];
	int x, z;
	
	for (z = 0; z < act; z++)		/*  ciclo para passar os conteudos do vetor Sistema para um vetor auxiliar */ 
	{
		r[z].prod_id = v[z].prod_id;
		strcpy(r[z].prod_desc, v[z].prod_desc);
		r[z].prod_price = v[z].prod_price;
		r[z].prod_st = v[z].prod_st;
	}
	/* 	algoritmo de ordenacao */
	insertionSort(r, 0, act - 1);	  

	printf("Produtos\n");
	for (x = 0; x < act; x++)
	{
		printf("* %s %d %d\n", r[x].prod_desc, r[x].prod_price, r[x].prod_st);
	}
}


void listProdInOrd(prod v[], order t[])
{
	prod c[MAX_ORDQ];      							/*  vetor auxiliar que ira conter os produtos da encomenda em questao */
	int EE, x, z, n, ide;
	int i = 0;
	scanf("%d", &ide);

	EE = existE(i, order_act, ide, t);
	
	if (ide != t[EE].order_id || order_act == 0)
	{
		printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ide);
		return;
	}

	if (t[EE].order_prod_act == 0 || t[EE].order_w == 0)
	{
		printf("Encomenda %d\n", ide);
		return;
	}
	/*  ciclo para passar os conteudos para o vetor auxiliar */
	for (n = 0; n < t[EE].order_prod_act; n++)		/*  iterar todos os produtos do sistema */
	{
		for (z = 0; z < act; z++)					/*  iterar todos os produtos da encomenda */
		{
			if (t[EE].order_prods[n] == v[z].prod_id)		/*  copia os atributos necessarios */ 
			{
				strcpy(c[n].prod_desc, v[z].prod_desc);
				c[n].prod_price = v[z].prod_price;
				c[n].prod_st = t[EE].order_qnt[n];
			}
		}
	}
	/*  algoritmo de ordenacao  pela descricao */
	quicksort(c, 0, t[EE].order_prod_act - 1); 

	printf("Encomenda %d\n", ide);	  				/*  da print ao header */
	for (x = 0; x < t[EE].order_prod_act; x++)	
	{
		if (c[x].prod_st != 0)  /*  verifica se e um produto validoa quantidade do produto na encomenda */
			printf("* %s %d %d\n", c[x].prod_desc, c[x].prod_price, c[x].prod_st);
	}
	return;	
}


/*	funcao principal que seleciona cada operacao consoante o input do utilizador */
int main() 
{	
	prod system[MAX_ID];		/*	vetor com todos os produtos existente no Sistema */
	order orders[MAX_ORD];  	/*  vetor com todas as encomendas no CdE */
    char sel = getchar();
    while (sel != 'x'){			/*  ate a instrucao ser 'x' o a funcao esta ativa */
        switch (sel) 			/* 	escolhe a instruao conforme o primeiro caracter do input do utilizador */ 
        {
            case 'a':
                addProd(system); 
                break; 
            case 'q':
                addStock(system);
                break; 
            case 'N':
                newOrder(orders);
                break; 
            case 'A':
                addProdToOrder(system, orders);
                break; 
            case 'r':
                removeStockProd(system);
                break; 
            case 'R':
                removeProdFromOrder(system, orders);
                break;
            case 'C':
                costOrder(system, orders);
                break;
            case 'p':
                changeProdPrice(system);
                break;
            case 'E':
                listProdQ(system, orders);
                break;
            case 'm':
                maxProd(system, orders);
                break; 
            case 'l':
                listProdInSys(system);
                break; 
            case 'L':
                listProdInOrd(system, orders);
                break;    
            }
            sel = getchar();	/*  seleciona a proxima instrucao */
        }  
    return 0;
}

