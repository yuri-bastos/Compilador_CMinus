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

/* Funcao de hash que retorna o indice que corresponde a dada chave key */
int hash(char* key);

/* Busca o end da lista de baldes de um Simbolo: 
 * caso encontre: retorna a lista
 * caso nao encontre: retorna NULL
 */
int search_ST(char *name, ScopeName scope);

/* Insere na tabela de simbolos (tabela hash) um simbolo, colocando
 * seu escopo, local de memoria e atualizando os numeros de linha.
 * Caso o simbolo ja exista, atualiza apenas a lista de numero de linhas...
 * 
 * Funcinamento: Realizar busca na tabela de simbolos para ver se aquele ID
 * existe. Caso exista, atualizar n da linha, caso a busca retorne NULL,
 * realizar procedimento de insercao atualizando a bucketList do indice 
 * na tabela hash com o id encontrado, caso seja valido...
 */
void insert_ST(char * name,  int lineno, int loc, ScopeName scope, ExpType dType, DeclKind iType);

/* Imprime a tabela de hash no formato da tabela de simbolos
 * propria e graficamente dita. usa o mesmo arquivo de saida
 * listing usado pelas demais partes do compilador
 */
void print_ST(FILE *listing);

#endif