/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel Souza Paiva - 140620				*/
/*													*/
/* 				Arquivo: cminus.y                   */
/* 		  Especificacao Yacc/Bison pra C-           */
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/
			
			///////////////
			//DEFINICOES//
			/////////////
	%{
		#define YYPARSER /* Distingue saída do YACC de outros arquivos */
		#include "globals.h"
		
		#include "util.h"
		#include "scan.h"
		#include "parse.h" 
		#include <stdio.h>
		#include <stdlib.h>
		#include <ctype.h>
		#include <string.h>
		
		#define YYSTYPE TreeNode *
		static int savedNumber;
		static char * savedName;
		static int savedLineNo; 
		static TreeNode * savedTree;
		static int yylex(void); // evitar conflito com flex
		
		
	%}
	
	%token IF INT RETURN VOID WHILE ELSE
	%token ID NUM 
	%token LT LET GT GET EQUAL SEMI DIFF
	%token LPAR RPAR LBRC RBRC LBRT RBRT
	%left PLUS MINUS
	%left MULT DIVI COMMA
	%right ASSIGN 
	%token ERROR

%%

			///////////////
			////REGRAS////
			/////////////

	
	programa : declaracao-lista {savedTree = $1;}
		;
	
declaracao-lista : declaracao-lista declaracao { YYSTYPE t = $1; if (t != NULL) { while (t->sibling != NULL) { t = t->sibling; } t->sibling = $2; $$ = $1; } else { $$ = $2; } }
				 | declaracao { $$ = $1; }
				 ;
					 
declaracao : var-declaracao { $$ = $1; }
		   | fun-declaracao { $$ = $1; }
		   ;
			   
id : ID { savedName = copyString(tokenString); savedLineNo = lineno; }
   ;
	   
num : NUM { savedNumber = atoi(tokenString); savedLineNo = lineno; }
	;
	
			   
var-declaracao : tipo-especificador id SEMI { $$ = newDeclNode(VarDeclK); $$->child[0] = $1; $$->lineno = lineno; $$->attr.name = savedName; }
			   | tipo-especificador id LBRT num RBRT SEMI { $$ = newDeclNode(ArrayDeclK); $$->child[0] = $1; $$->lineno = lineno; $$->type = ArrayInteger; $$->attr.arr.name = savedName; $$->attr.arr.size = savedNumber; }
			   ;
				   
tipo-especificador : INT { $$ = newExpNode(TypeK); $$->type = Integer; }
				   | VOID { $$ = newExpNode(TypeK); $$->type = Void; }
				   ;
					   
fun-declaracao : tipo-especificador id { $$ = newDeclNode(FunDeclK); $$->lineno = lineno; $$->attr.name = savedName; } LPAR params RPAR composto-decl { $$ = $3; $$->child[0] = $1; $$->child[1] = $5; $$->child[2] = $7; }
			   ;
	
params : param-lista { $$ = $1; }
	   | VOID { $$ = newDeclNode(FunParDeclK); $$->type = Void; }
	   ;
	
param-lista : param-lista COMMA param { YYSTYPE t = $1; if (t != NULL) { while (t->sibling != NULL) { t = t->sibling; } t->sibling = $3; $$ = $1; } else { $$ = $2; } }
			| param { $$ = $1; }
			;
				
param : tipo-especificador id { $$ = newDeclNode(FunParDeclK); $$->child[0] = $1; $$->attr.name = savedName; }
	  | tipo-especificador id LBRT RBRT { $$ = newDeclNode(ArrayParDeclK); $$->child[0] = $1; $$->attr.name = copyString(savedName); }
	  ;
		  
composto-decl : LBRC local-declaracoes statement-lista RBRC { $$ = newStmtNode(CompK); $$->child[0] = $2; $$->child[1] = $3; }
			  ;
				  
local-declaracoes : local-declaracoes var-declaracao { YYSTYPE t = $1; if (t != NULL) { while (t->sibling != NULL) { t = t->sibling; } t->sibling = $2; $$ = $1; } else { $$ = $2; } }
				  |  { $$ = NULL; }
				  ;
					  
statement-lista : statement-lista statement { YYSTYPE t = $1; if (t != NULL) { while (t->sibling != NULL) { t = t->sibling; } t->sibling = $2; $$ = $1; } else { $$ = $2; } }
				|  { $$ = NULL; }
				;
					
statement : expressao-decl { $$ = $1; }
		  | composto-decl { $$ = $1; }
		  | selecao-decl { $$ = $1; }
		  | iteracao-decl { $$ = $1; }
		  | retorno-decl { $$ = $1; }
		  ;
	
expressao-decl : expressao SEMI { $$ = $1; }
			   | SEMI { $$ = NULL; }
			   ;
				   
selecao-decl : IF LPAR expressao RPAR statement { $$ = newStmtNode(IfK); $$->child[0] = $3; $$->child[1] = $5; }
			 | IF LPAR expressao RPAR statement ELSE statement { $$ = newStmtNode(IfK); $$->child[0] = $3; $$->child[1] = $5; $$->child[2] = $7; }
			 ;
				 
iteracao-decl : WHILE LPAR expressao RPAR statement { $$ = newStmtNode(WhileK);$$->child[0] = $3;$$->child[1] = $5; }
			  ;
				  
retorno-decl : RETURN SEMI { $$ = newStmtNode(ReturnK); $$->type = Void; }
			 | RETURN expressao SEMI { $$ = newStmtNode(ReturnK); $$->child[0] = $2; }
			 ;
				 
expressao : var ASSIGN expressao { $$ = newStmtNode(AssignK);$$->child[0] = $1;$$->child[1] = $3; }
		  | simples-expressao { $$ = $1; }
		  ;
			  
var : id { $$ = newExpNode(IdK); $$->attr.name = savedName; }
	| id  { $$ = newExpNode(IdK); $$->attr.name = savedName; } LBRT expressao RBRT { $$ = $2; $$->child[0] = $4; }
	;
		
simples-expressao : soma-expressao relacional soma-expressao { $$ = newExpNode(CalcK);$$->child[0] = $1; $$->child[1] = $2; $$->child[2] = $3; }
				  | soma-expressao { $$ = $1; }
				  ;
					  
relacional : LET { $$ = newExpNode(OpK); $$->attr.op = LET; }
		   | LT { $$ = newExpNode(OpK);$$->attr.op = LT; }
		   | GT { $$ = newExpNode(OpK);$$->attr.op = GT; }
		   | GET { $$ = newExpNode(OpK); $$->attr.op = GET; }
		   | EQUAL { $$ = newExpNode(OpK); $$->attr.op = EQUAL; }
		   | DIFF { $$ = newExpNode(OpK); $$->attr.op = DIFF; }
		   ;
			   
soma-expressao : soma-expressao soma termo { $$ = newExpNode(CalcK); $$->child[0] = $1; $$->child[1] = $2; $$->child[2] = $3; }
			   | termo { $$ = $1; }
			   ;
				   
soma : PLUS { $$ = newExpNode(OpK); $$->attr.op = PLUS; }
	 | MINUS { $$ = newExpNode(OpK); $$->attr.op = MINUS; }
	 ;
		 
termo : termo mult fator { $$ = newExpNode(CalcK); $$->child[0] = $1; $$->child[1] = $2; $$->child[2] = $3; }
	  | fator { $$ = $1; }
	  ;
		  
mult : MULT { $$ = newExpNode(OpK); $$->attr.op = MULT; }
	 | DIVI { $$ = newExpNode(OpK); $$->attr.op = DIVI; }
	 ;
		 
fator : LPAR expressao RPAR { $$ = $2; }
	  | var { $$ = $1; }
	  | ativacao { $$ = $1; }
	  | num { $$ = newExpNode(ConstK); $$->type = Integer; $$->attr.val = atoi(tokenString); }
	  ;
		  
ativacao : id { $$ = newExpNode(CallK); $$->attr.name = savedName; } LPAR args RPAR { $$ = $2; $$->child[0] = $4; }
		 ;
			 
args : arg-lista { $$ = $1; }
	 | { $$ = NULL; }
	 ;
		 
arg-lista : arg-lista COMMA expressao { YYSTYPE t = $1; if (t != NULL) {while (t->sibling != NULL) { t = t->sibling; }t->sibling = $3;$$ = $1;} else {$$ = $3;} }
		  | expressao { $$ = $1; }
		  ;
	
%%

			///////////////
			////CODIGO////
			/////////////
	
	
	int yyerror(char * message) 
	{
	  fprintf(listing,"ERRO SINTÁTICO: ");
	  printToken(yychar,tokenString);
	  fprintf(listing," LINHA: %d\n",lineno);
	  Error = TRUE;
	  return 0;
	}
	
	static int yylex(void) 
	{
		return getToken();
	}
	
	TreeNode * parse(void) 
	{
		yyparse();
		return savedTree;
	}
			