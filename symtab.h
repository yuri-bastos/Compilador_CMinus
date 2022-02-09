/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: symtab.h             	    */
/* 	    Interface da Tabela de Simbolos para o  	*/
/*			      compilador de c-	  		    	*/
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

/* Lista encadeada que armazena o numero das linhas e a pos do prox item */

typedef struct lineListK{
    int lineNum;
    struct lineListK *next;
} *lineList;

typedef struct parListK{
    DeclKind par;
    struct parListK *next;
} *funParsList;

/* lista de baldes: contem listas dentro delas
 * Chaves:
    name = nome do simbolo
    lines = lista encadeada de linhas
    memloc = local de memoria
    scope = escopo do simbolo
    dataType = tipo do dado (int ou void)
    idType = tipo do id (fun var array)
    next = ponteiro pro prox elem
 */
typedef struct bList{
    /* Chaves: */
    char *name; //Nome do simbolo
    lineList lines; //Lista encadeada que marca as linhas de ocorrencia desse ID
    int memloc; //pos de memoria desse simbolo
    ScopeName scope; //Escopo do simbolo
    ExpType dataType; //Tipo do Dado(Int, Void) //Void, Integer, Array Integer
    DeclKind idType; //Tipo do ID(var fun array) //Usando como guia: DeclKind (Aqui eh o tipo que foi declarado para consultas futuras)
    //Obs.: usamos declKind pois o que conta em um simbolo eh como ele foi declarado, 
    //caso nao tenha declaracao previa, ja identifica um erro...
    
    TreeNode * node;//No da arvore, usado para verificao de funcoes
    funParsList funcPars;//Lista encadeada que armazena os parametros de uma funcao

    /* Ponteiro pro prox: */
    struct bList *next;
} *bucketList;

/* Funcao de hash que retorna o indice que corresponde a dada chave key */
int hash(char* key);

/* Busca a existencia ou nao de um simbolo: 
 * caso encontre: retorna 1
 * caso nao encontre: retorna 0
 * 
 * Args:
 *  name = nome do simbolo
 *  scope = escopo desse simbolo
 */
int search_ST(char *name, ScopeName scope);

/* Insere na tabela de simbolos (tabela hash) um simbolo, colocando
 * seu escopo, local de memoria e atualizando os numeros de linha.
 * Caso o simbolo ja exista, atualiza apenas a lista de numero de linhas...
 * 
 * Args:
 *  name = nome do simbolo
 *  lineno = num da linha em que aparece
 *  loc = local de memoria (use memloc)
 *  scope = escopo onde se localiza
 *  dType = tipo do dado (Int, Void)
 *  iType = tipo do id (Var, Fun, Array)
 * 
 * Funcinamento: Realizar busca na tabela de simbolos para ver se aquele ID
 * existe. Caso exista, atualizar n da linha, caso a busca retorne NULL,
 * realizar procedimento de insercao atualizando a bucketList do indice 
 * na tabela hash com o id encontrado, caso seja valido...
 */
void insert_ST(char * name,  int lineno, int loc, ScopeName scope, ExpType dType, DeclKind iType, TreeNode * declNode, funParsList params);

/* Imprime a tabela de hash no formato da tabela de simbolos
 * propria e graficamente dita. usa o mesmo arquivo de saida
 * listing usado pelas demais partes do compilador
 * 
 *  Args:
 *   listing = arquivo de saida de trace
 */
void print_ST(FILE *listing);

ExpType getDataType(char *name, ScopeName scope);

DeclKind getIDType(char *name, ScopeName scope);

DeclKind getIDTypeGlobal(char *name, ScopeName scope);

TreeNode * getFuncNode(char *funcName);

funParsList getFuncParams_ST(char *funcName);

/*
 * 0 -> sem erros
 * 1 -> Muitos parametros para chamar a funcao
 * 2 -> Tipos dos parametros incompativeis
 */
int compParamLists(char *funcName, funParsList callParams);



#endif