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
->
Depois:
CalcK -> Olhar os tipos dos dois filhos (se forem funcoes!!):
(Se kind.exp for CallK, procurar o tipo dela no escopo), se for VOID da erro


CallK ->Ver se a funcao existe!

-> FunDeclK -> Olhar tree->child[0] que tem o tipo de dado dela!! (Observar se ela ja existe e jogar erro de redefinicao de funcao)
->DeclK -> tree->child[0]->type armazena o tipo 


-> ASSIGN -> Olhar a expressao seguinte 
 */

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

static void semanticErr(TreeNode * tree, char *id,  ScopeName *scope, char *msg)
{
    if(!TraceAnalyze_DETAIL) fprintf(listing, "ERRO SEMANTICO: %s LINHA: %d");
    else fprintf(listing, "ERRO SEMANTICO: %s LINHA: %d em %s -> \"%s\"", id, tree->lineno, scope, msg);
    Error = TRUE;
}


/*Contadores para localizao de memoria e escopo atual*/
static int currMem = 0;
static ScopeName currScope = "global";

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

/* 
 * COMENTE IMEDIATAMENTE
 *
 * 
 */  
void checkTNode(TreeNode * tree)
{
    if(Error = TRUE) return; //Ignorar checagens se já tiver dado erro

    switch (tree->nodekind)
    {
    case StmtK:
        switch (tree->kind.stmt)
        {
        case AssignK://Conferir retorno
            if(tree->child[1] == NULL) break; //SO PRA GARANTIR
            if (tree->child[1]->nodekind == CallK)
            {//Se for do tipo chamada de funcao, verificar o retorno das mesmas 
                if(search_ST(tree->child[1]->attr.name, "\0")->dataType == Void)
                    semanticErr(tree, tree->attr.name, currScope, "Funcao de retorno void nao pode ser atribuida a variavel do tipo int");
            } 
            break;

        case ReturnK://Conferir retorno
            if (search_ST(currScope,"\0")->dataType == Void && tree->child[0] != NULL)
            {//Se nn é NULL, entao retorna int
                semanticErr(tree, "return", currScope, "Retorno de valor em funcao de tipo void");
            }
            break;
        
        default:
            break;
        }
        break;
    
    case ExpK:
        switch (tree->kind.exp)
        {
        case IdK://Conferir se houve chamada de id com funcao que retorna void (EXTRA)
            if(tree->child[0] == NULL) break; //SO PRA GARANTIR
            if (tree->child[0]->nodekind == CallK)
            {//Se for do tipo chamada de funcao, verificar o retorno das mesmas 
                if(search_ST(tree->child[0]->attr.name, "\0")->dataType == Void)
                    semanticErr(tree, tree->attr.name, currScope, "Funcao de retorno void nao pode ser expressao de vetor");
            } 
            break;

        case CalcK://Conferir operandos
            if(tree->child[0] == NULL && tree->child[1] == NULL) break; //SO PRA GARANTIR
            if (tree->child[0]->nodekind == CallK)
            {//Se for do tipo chamada de funcao, verificar o retorno das mesmas 
                if(search_ST(tree->child[0]->attr.name, "\0")->dataType == Void)
                    semanticErr(tree, tree->attr.name, currScope, "Funcao de retorno void nao pode ser operando");
            } else if (tree->child[1]->nodekind == CallK)
            {
                if(search_ST(tree->child[1]->attr.name, "\0")->dataType == Void)
                    semanticErr(tree, tree->attr.name, currScope, "Funcao de retorno void nao pode ser operando");
            }
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
    //traverse(tree, insertNode, nullProc);
}

void typeCK_ST(TreeNode * tree){
    traverse(tree, nullProc, checkTNode);
}