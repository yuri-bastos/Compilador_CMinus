/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: util.h               	    */
/* 		Funções de Utilidade para manipulação       */
/*			 dos nós Árvore Sintática				*/
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

/* Procedimento printToken imprime um token e seu lexema */
void printToken(TokenType , const char* );

/* Procedimento printToken mas sem pular linha*/
void printTokenNoJumpLine( TokenType token, const char* tokenString );

void printTypes(TreeNode* tree);
/* Função newStmtNode cria um novo nó do tipo Statement */
TreeNode * newStmtNode(StmtKind);

/* Função newExpNode cria um novo nó do tipo Expressão */
TreeNode * newExpNode(ExpKind);

/* Função newDeclNode cria um novo nó do tipo Declaração */
TreeNode * newDeclNode(DeclKind);

/* Função copyString cria uma cópia de uma string existente */
char * copyString( char * );

/* Procedimento printTree imprime a árvore sintática utilizando identação para designar subárvores */
void printTree( TreeNode * );

/* getType retorna tipos de função e de variáveis */
ExpType getTypes(TreeNode* tree);

#endif
