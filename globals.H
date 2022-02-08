/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: globals.h	           	    */
/* 	     Tipos e Variáveis Globais para				*/
/*				Compilador de C-	    			*/
/*			 										*/
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/


#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/* Yacc/Bison gera internamente seus próprios valores para os tokens.
 * Otros arquivos podem acessar esses valores ao incluir o arquivo
 * cminus.tab.h gerado
 
 * A flag YYPARSER previne a inclusão do arquivo cminus.tab.h
 * na propria saída Bison.
 */

#ifndef YYPARSER
#include "cminus.tab.h"

/* ENDFILE é definido implicidamente pelo Yacc/Bison, e não é incluído no arquivo cminus.tab.h */
#define ENDFILE 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = número de palavras reservadas (Else, If, Int, While, Return, Void) */
#define MAXRESERVED 6

typedef int TokenType;

extern FILE* source; /* Arquivo de Entrada */
extern FILE* listing; /* Saída do programa */
extern FILE* code; /* código para máquina de estados */

extern int lineno; /* Linha do arquivo de entrada */

/**************************************************/
/********  Árvore Sintática para o Parsing ********/
/**************************************************/

typedef enum {StmtK,ExpK, DeclK} NodeKind;	//Tipo de nó 
typedef enum {IfK,WhileK,AssignK,ReturnK, CompK} StmtKind; //Nó do tipo Statement
typedef enum {OpK,ConstK,IdK,TypeK,ArrayK,CallK, CalcK} ExpKind; //Nó do tipo Expressão
typedef enum {VarDeclK,ArrayDeclK, ArrayParDeclK, FunDeclK, FunParDeclK} DeclKind; //Nó do tipo Declaração

/* ExpType é usado para verificar o tipo da expressão */
typedef enum {Void,Integer,ArrayInteger} ExpType;

#define MAXCHILDREN 3

typedef char* ScopeName;//String que armazena nomes de funcoes (escopos)

typedef struct ArrayAttribute { TokenType type; char * name; int size; } ArrAttr; //Atributos de um Vetor

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN]; // Nós filhos
     struct treeNode * sibling; //Nós com mesma raiz
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp; DeclKind decl;} kind; //Tipo de Nó
     union { TokenType type; TokenType op; int val; char * name; ArrAttr arr; ScopeName scope;} attr; //Todas as informações deste nó
     ExpType type; //Tipo da expressão
   } TreeNode;

/**************************************************/
/***********   Flags para o tracing       *********/
/**************************************************/

/* EchoSource = TRUE faz com que o programa de entrada seja passado
	para a saída com o número de cada linha durante o parsing.
 */
extern int EchoSource;

/* TraceScan = TRUE faz com que as informações do token a ser impresso sejam
 movidos para a saída assim que o token é reconhecido pelo scanner.
 */
extern int TraceScan;

/* TraceParse = TRUE faz com que a árvore sintática a ser impressa utilize
   a identação criada.
 */
extern int TraceParse;

/* TraceAnalyze = TRUE faz com que a tabela de símbolos seja inserida na saída, e então impressa
 */
extern int TraceAnalyze;

/* TraceAnalyze = TRUE faz com que a tabela de símbolos seja inserida na saída, e então impressa
 */
extern int TraceAnalyze_DETAIL;

/* TraceCode = TRUE faz com que os comentários sejam escritos no código da máquina de estados
 enquanto o código é gerado
 */
extern int TraceCode;

/* Error = TRUE impede a execução ao detectar um erro */
extern int Error; 
#endif
