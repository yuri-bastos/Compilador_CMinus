/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: symtab.c             	    */
/* 	   Implementação da Tabela de Simbolos para o  	*/
/*			      compilador de c-	  		    	*/
/* 													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/

#include "globals.h"
#include "symtab.h"

/* SIZE representa o tamanho maximo da tabela de hash */
#define SIZE 211

/* SHIFT e a potencia de 2 utilizada com indice alfa no calculo do hash */
#define SHIFT 4

/* Funcao de hash que retorna o indice que corresponde a dada chave key */
int hash(char* key){
    int temp = 0;
    int i = 0;
    while (key[i] != '\0'){ 
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }
    return temp;
}

/*  */
typedef struct linkedList{
    int lineno;
    struct linkedList *next;
} *lineList;

/*  */
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
    
    /* Ponteiro pro prox: */
    struct bList *next;
} *bucketList;


/* Tabela de Hash que estrutura a tabela de simbolos */
static bucketList hashTable[SIZE];

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
void insert_ST(char * name,  int lineno, int loc, ScopeName scope, ExpType dType, DeclKind iType){
    char *key = strcat(name,scope);
    int h = hash(key);
}





