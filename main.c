/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: main.c	            	    */
/* 	   Programa principal do Compilador de C-	    */
/*			 										*/
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/

#include "globals.h"

#define NO_PARSE FALSE
#define NO_ANALYZE FALSE
#define NO_CODE TRUE

#include "util.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"
#include "symtab.h"

/* Variáveis globais */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* Flags de Trace */
int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceAnalyze_DETAIL = TRUE;
int TraceCode = FALSE;
int Error = FALSE;

int main( int argc, char * argv[] )
{ TreeNode * syntaxTree;
	char pgm[120]; /* nome do arquivo fonte */
	if (argc != 2)
		{ fprintf(stderr,"Use: %s <filename>\n",argv[0]);
			exit(1);
		}
	strcpy(pgm,argv[1]) ;
	if (strchr (pgm, '.') == NULL)
		 strcat(pgm,".cm");
	source = fopen(pgm,"r");
	if (source==NULL)
	{ fprintf(stderr,"Arquivo %s não encontrado\n",pgm);
		exit(1);
	}
	listing = stdout; /* carrega o arquivo para saída */
	fprintf(listing,"\n		Compilando em C-: %s\n\n>Analise Lexica/Sintatica:\n\n",pgm);
#if NO_PARSE
	while (getToken()!=ENDFILE);
#else
	syntaxTree = parse();
	if (TraceParse) {
		if(Error == FALSE){
			fprintf(listing,"\n----------------------------------------------------------------------\n%42s\n----------------------------------------------------------------------\n", "ARVORE SINTATICA");
			printTree(syntaxTree);
			fprintf(listing,"\n----------------------------------------------------------------------\n");
		}
	}
#endif
#if NO_ANALYZE
	/*  */
#else
	if(Error == FALSE){
		fprintf(listing,"\n\n>Analise Semantica:\n\n");
		build_ST(syntaxTree);
		check_ST(syntaxTree);
		if (TraceAnalyze) {
			if(Error == FALSE)
			{
				fprintf(listing,"\n----------------------------------------------------------------------\n%44s\n----------------------------------------------------------------------\n", "TABELA DE SIMBOLOS");
				print_ST(listing);
			}
		}
	}
	
#endif
	fclose(source);
	return 0;
}