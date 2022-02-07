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
Como carambolas eu posso manter track do escopo atual pra checar o retorno???
Usar uma pilha?
O que é melhor??

-> Preciso conferir o escopo para um return...
-> CalcK -> Olhar 
-> FunDeclK -> Olhar tree->child[0] que tem o tipo de dado dela!! (Observar se ela ja existe e jogar erro de redefinicao de funcao)


 */

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

typedef struct ScopeSStack{
    ScopeName scope;
    struct ScopeSStack *prev;
} *scopeStack;






/*Contador para as localizacoes de memoria das variaveis*/
static int currMem = 0; 

/* "Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t"
 * 
 * Rotina transversal da arvore sintatica generica. Aplica o preProc antes de cada transversao
 * E aplica o postproc depois de fazer a transversao
 * 
 * Fonte: Compilador para TINY de Kenneth C. Louden
 */
static void traverse( TreeNode * tree, void (* preProc) (TreeNode *), void (* postProc) (TreeNode *) )
{ if (tree != NULL)
  { preProc(tree);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(tree->child[i],preProc,postProc);
    }
    postProc(tree);
    traverse(tree->sibling,preProc,postProc);
  }
}


void insertNode(TreeNode * tree)
{
    switch (tree->nodekind)
    {
    case StmtK:
        switch (tree->kind.stmt)
        {
        case ReturnK:
            
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }
}







/* "nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse"
 * 
 * Procedimento que nao faz nada, usado na funcao transverse apenas
 * quando nao se deseja que nada seja executado antes ou depois da transversao
 * 
 * Fonte: Compilador para TINY de Kenneth C. Louden
 */
static void nullProc(TreeNode * tree){
    if (tree==NULL) return;
    else return;
}


void build_ST(TreeNode * tree){ 
    traverse(tree, insertNode, nullProc);
}