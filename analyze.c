/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: analyze.c            	    */
/* 	      Implementação do Analisador Semantico 	*/
/*			    para o compilador de c-				*/
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/

/* IMPORTANTE:
Conferir Declaracoes:
 se tree->nodekind == StmtK



 */

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

/*Contador para as localizacoes de memoria das variaveis*/
static int currMem = 0; 


