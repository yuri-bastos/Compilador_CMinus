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

//################################### INCLUDEs ##########################################
#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

//############################### Variaveis globais #####################################

/*Contadores para localizao de memoria e escopo atual*/
static int currMem = 0;//Armazena a localizacao de memoria atual
static ScopeName currScope = "global";//guarda o escopo atual
static int declExist = FALSE;//contabiliza se ja houve pelo menos uma declaracao

//############################# Saida de Erros Semanticos ###############################

/*
 * static void semanticErr(TreeNode * tree, char *id,  ScopeName *scope, char *msg)
 * ARGS:
 *  - tree: no de arvore
 *  - id: nome do identificador
 *  - scope: escopo do id
 *  - msg: mensagem extra (exibida somente se TraceAnalyze detalhado nao for solicitado)
 * 
 * Funcao utilitaria que emite no arquivo listing o erro semantico ocorrido, com ou sem uma 
 * mensagem especificando o erro, dependendo da config de TraceAnalyze_DETAIL, que indica se
 * havera ou nao um tracing mais detalhado
 */
static void semanticErr(TreeNode * tree, char *id,  ScopeName scope, char *msg)
{
    if(!TraceAnalyze_DETAIL) fprintf(listing, "ERRO SEMANTICO: %s LINHA: %d\n", id, tree->lineno);
    else fprintf(listing, "ERRO SEMANTICO: %s LINHA: %d em %s -> \"%s\"\n", id, tree->lineno, scope, msg);
    Error = TRUE;
}

//############################## Proc. de Checagem do Escopo ##############################

/*
 * static void preCheckScopeStart(TreeNode *tree)
 * ARGS: 
 *  - tree = no de arvore
 * 
 * Verifica se um nova funcao foi declarada
 * Se sim, atualiza o escopo atual com o nome da funcao declarada
 * Se nao, nada acontece
 * 
 * IMPORTANTE: Fazer a checagem ANTES de verificar os irmaos e filhos do no
 * AVISO: Esta funcao considera que o no de arvore nao eh nulo!!
 */
static void preCheckScopeStart(TreeNode *tree)
{   if (tree != NULL)
        if (tree->nodekind == DeclK)
            if (tree->kind.decl == FunDeclK)
                currScope = tree->attr.name;
}

/*
 * static void postCheckScopeEnd(TreeNode *tree)
 * ARGS: 
 *  - tree = no de arvore
 * 
 * Verifica se o no atual declarou uma nova funcao
 * Se sim, retorna o escopo para o escopo global
 * Se nao, nada acontece
 * 
 * IMPORTANTE: Fazer a checagem DEPOIS de verificar os irmaos e filhos do no
 * AVISO: Esta funcao considera que o no de arvore nao eh nulo!!
 */
static void postCheckScopeEnd(TreeNode *tree)
{   if (tree != NULL)
        if (tree->nodekind == DeclK)
            if (tree->kind.decl == FunDeclK)
                currScope = "global"; //retorna ao escopo global (uma vez que nao ha declaracao de funcao dentro de outra funcao)
}

/*
 * static void checkMain()
 * 
 * Verifica a existencia da funcao main na tabela de simbolos
 * caso nao exista, lanca um erro e o sinaliza caso TraceAnalyze esteja ativado
 * 
 * IMPORTANTE: fazer essa checagem deve ser pelo menos, DEPOIS do fim do processo de insercao!
 */
static void checkMain()
{
    if (Error != TRUE)
    {
        if (search_ST("main", "\0") == 0)
        {
            if(TraceAnalyze) fprintf(listing, "ERRO SEMANTICO: funcao main() nao declarada!\n");
            Error = TRUE;
        }
    }
}

//########################## Percorrer a Arvore Sintatica ##################################

/* 
 * static void transverse( TreeNode * tree, void (* preProc) (TreeNode *), void (* postProc) (TreeNode *) )
 * ARGS:
 *  - trre: no de arvore
 *  - void (*preProc): APLICADO ANTES -> Procedimento que recebe arg do tipo ponteiro para TreeNode
 *  - void (*postProc): APLICADO DEPOIS -> Procedimento que recebe arg do tipo ponteiro para TreeNode
 * 
 * Rotina transversal da arvore sintatica generica. Aplica o preProc antes de cada transversao
 * E aplica o postproc depois de fazer a transversao
 *    
 * "Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t"
 * 
 * Fonte: Compilador para TINY de Kenneth C. Louden
 */
static void traverse( TreeNode * tree, void (* preProc) (TreeNode *), void (* postProc) (TreeNode *) )
{ if (tree != NULL)
  { 
    
    preCheckScopeStart(tree);
    preProc(tree);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(tree->child[i],preProc,postProc);
    }
    postProc(tree);
    postCheckScopeEnd(tree);
    traverse(tree->sibling,preProc,postProc);
  }
}

/* 
 * static void nullProc(TreeNode * tree)
 * ARGS:
 *  - tree: no de arvore
 * 
 * Procedimento que nao faz nada, usado na funcao transverse apenas
 * quando nao se deseja que nada seja executado antes ou depois da transversao
 * 
 * "nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse"
 * 
 * Fonte: Compilador para TINY de Kenneth C. Louden
 */
static void nullProc(TreeNode * tree){
    if (tree==NULL) return;
    else return;
}

//############################# Manipulacao da Arvore e Tabela ############################################

/* 
 * void insertTNode(TreeNode * tree)
 * ARGS: 
 *  - tree = Ponteiro para o no da arvore
 * 
 * Insere o no de arvore na tabela de simbolos, checando:
 *  -> UNICIDADE: 
 *    >Se o ID (var ou fun) esta sendo redeclarado naquele escopo
 *    >Se o ID esta sendo declarado como var mas ja existe com fun
 *  -> FLUXO DE CONTROLE:
 *    >Se o ID foi chamado mas nao declarado no escopo atual (nem globalmente)
 *    >Se while nao apresenta valor logico 
 * -> TIPO (Ao declarar):
 *    >Se a var esta sendo declarada como tipo de dado VOID
 * tambem checa se o retorno de uma funcao eh compativel com
 * sua declaracao.
 * 
 */  
void insertTNode(TreeNode * tree)
{
    if(Error == TRUE) return; //Parar insercao se já tiver dado erro
    TreeNode * treeAux;
    funParsList declParams = NULL, currParams = NULL;
    switch (tree->nodekind)
    {
    case DeclK:
         
        switch (tree->kind.decl)
        {
        case FunDeclK://Verificar se ja foi declarada 
            //tree->child[1] = guarda os parametros
            //cada irmao eh um parametro
            
            if(search_ST(tree->attr.name, "\0") == 0)//Funcoes sao armazenadas no escopo "\0"
            {//nao foi declarada, inserir entao
                
                if(tree->child[1]->attr.name != NULL){//Se existe parametros
                    declParams = (funParsList) malloc(sizeof(struct parListK));
                    currParams = declParams;
                    treeAux = tree->child[1];//percorrer os parametros (tem que garantir que eles n sao tipo void...)
                    while (treeAux != NULL)
                    {
                        if(treeAux->child[0]->type == Void){//declaracao de paramentro do tipo void!
                            semanticErr(treeAux, treeAux->attr.name, currScope, "Declaracao de Parametro Invalida: Parametro de Variavel ou Parametro Array nao podem ser tipo void!");
                            return;
                        } else {//ok, inserir na lista
                            currParams->par = treeAux->kind.decl;
                            currParams->next = (funParsList) malloc(sizeof(struct parListK));;
                            currParams = currParams->next; 
                        }
                        treeAux = treeAux->sibling;
                    }
                }else{
                    declParams = NULL;
                }
                insert_ST(tree->attr.name, tree->lineno, currMem++, "\0", tree->child[0]->type, FunDeclK, tree, declParams);
            
            } else{
                semanticErr(tree, tree->attr.name, currScope, "Redeclaracao de funcao");
            }
            break;

        case VarDeclK://Verificar: Declaracao como funcao ou variavel, verificar declaracao como void
            if(getTypes(tree) == Void) 
                semanticErr(tree, tree->attr.name, currScope, "Declaracao invalida: Variavel nao pode ser do tipo void !");      
            else if(search_ST(tree->attr.name, currScope) != 0)
                semanticErr(tree, tree->attr.name, currScope, "Redeclaracao de variavel");
            else if(search_ST(tree->attr.name, "\0") != 0)
                semanticErr(tree, tree->attr.name, currScope, "Declaracao invalida, esta variavel ja foi declarada como ID de funcao");
            else
                insert_ST(tree->attr.name, tree->lineno, currMem++, currScope, Integer, VarDeclK, tree, NULL);
            break;
        
        case ArrayDeclK://Vetores
            if(getTypes(tree) == Void)
                semanticErr(tree,tree->attr.arr.name, currScope, "Declaracao invalida: Array nao pode ser do tipo void !");
            else if(search_ST(tree->attr.arr.name, currScope) != 0){//ja declarado
                if(getIDType(tree->attr.arr.name, currScope) != ArrayDeclK)//se o que existe nao for do tipo array...
                    semanticErr(tree, tree->attr.arr.name, currScope, "Redeclaracao de Variavel como Array");
                else
                    semanticErr(tree, tree->attr.arr.name, currScope, "Redeclaracao de Array");
            }
            else if(search_ST(tree->attr.arr.name, "\0") != 0)
                semanticErr(tree, tree->attr.arr.name, currScope, "Declaracao invalida, este array ID ja foi declarado como ID de funcao");
            else 
                insert_ST(tree->attr.arr.name, tree->lineno, currMem++, currScope, ArrayInteger, ArrayDeclK, tree, NULL);
            break;

        case ArrayParDeclK://Vetores como parametro
            if(getTypes(tree) == VOID) //Checando se nao eh void...
                semanticErr(tree, tree->attr.name, currScope, "Declaracao invalida: Array nao pode ser do tipo void !");
            else if(search_ST(tree->attr.name, currScope) != 0){//ja declarado
                if(getIDType(tree->attr.name, currScope) != ArrayDeclK)//se o que existe nao for do tipo array...
                    semanticErr(tree, tree->attr.name, currScope, "Redeclaracao de Variavel como Array");
                else
                    semanticErr(tree, tree->attr.name, currScope, "Redeclaracao de Array");
            }
            else if(search_ST(tree->attr.name, "\0") != 0)
                semanticErr(tree, tree->attr.name, currScope, "Declaracao invalida, este array ID ja foi declarado como ID de funcao");
            else 
                insert_ST(tree->attr.name, tree->lineno, currMem++, currScope, Integer, ArrayParDeclK, tree, NULL);
            break;

        case FunParDeclK://Verificar: Declaracao como funcao ou variavel, verificar declaracao como void
            if (tree->child[0] == NULL) break;
            
            if(tree->child[0]->attr.name != NULL && tree->child[0]->type == Void)
                semanticErr(tree, tree->attr.name, currScope, "Declaracao invalida: Variavel nao pode ser do tipo void !");      
            else if(search_ST(tree->attr.name, currScope) != 0)
                semanticErr(tree, tree->attr.name, currScope, "Redeclaracao de variavel");
            else if(search_ST(tree->attr.name, "\0") != 0)
                semanticErr(tree, tree->attr.name, currScope, "Declaracao invalida, esta variavel ja foi declarada como ID de funcao");
            else
                insert_ST(tree->attr.name, tree->lineno, currMem++, currScope, Integer, VarDeclK, tree, NULL);
            break;
        

        default:
            break;
        }
        break;

    case ExpK:

        switch (tree->kind.exp)
        {
        case IdK: //Caso de uso de variavel
            if(search_ST(tree->attr.name, currScope) == 0 && search_ST(tree->attr.name, "global") == 0)
                semanticErr(tree, tree->attr.name, currScope, "Uso de ID nao declarado!!");
            else if(tree->child[0] == NULL && getIDType(tree->attr.name, currScope) == ArrayDeclK)
                {fprintf(listing, "%d", getIDType(tree->attr.name, currScope));
                semanticErr(tree, tree->attr.name, currScope, "Chamada de ID do tipo Array SEM especificacao de index (Esperado: ID[expressao])");
            }else
                insert_ST(tree->attr.name, tree->lineno, currMem++, currScope, 0, 0, NULL, NULL);
            break;
        
        default:
            break;
        }

    default:
        break;
    }
}

/* 
 * void checkTNode(TreeNode * tree)
 * ARGS: 
 *  - tree = Ponteiro para o no da arvore
 * 
 * Checa o no de arvore atual em busca de erros de tipo e
 * tambem checa se o retorno de uma funcao eh compativel com
 * sua declaracao.
 * 
 * IMPORTANTE: So funciona se ja tiver sido conferido se a dada funcao EXISTE
 */  
void checkTNode(TreeNode * tree)
{
    if(Error == TRUE) return; //Ignorar checagens se já tiver dado erro
    
    switch (tree->nodekind)
    {
    case StmtK:
        switch (tree->kind.stmt)
        {
        case AssignK://Conferir retorno            
            if(tree->child[1] == NULL) break; //SO PRA GARANTIR

            if (tree->child[1]->kind.exp == CallK)
            {//Se for do tipo chamada de funcao, verificar o retorno das mesmas 
                if(getDataType(tree->child[1]->attr.name, "\0") == Void)
                    semanticErr(tree, tree->child[1]->attr.name, currScope, "Funcao de retorno void nao pode ser atribuida a variavel do tipo int");
            } 
            break;

        case WhileK://Conferir expressao do While (OK)       
            if(tree->child[0] == NULL) break; //SO PRA GARANTIR
           
            if (tree->child[0]->kind.exp == CallK)
            {//Se for do tipo chamada de funcao, verificar o retorno das mesmas 
                if(getDataType(tree->child[0]->attr.name,"\0") == Void)
                    semanticErr(tree, "while", currScope, "Funcao de retorno void nao pode ser expressao de while");
            } 
            break;
        
        case ReturnK://Conferir retorno (OK)
            if (getDataType(currScope, "\0") == Void)
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
                if(getDataType(tree->child[0]->attr.name, "\0") == Void)
                    semanticErr(tree, tree->attr.name, currScope, "Funcao de retorno void nao pode ser expressao de vetor");
            } 
            break;

        case CalcK://Conferir operandos
            if(tree->child[0] == NULL && tree->child[2] == NULL) break; //SO PRA GARANTIR
            if (tree->child[0]->kind.exp == CallK)
            {//Se for do tipo chamada de funcao, verificar o retorno das mesmas 
                if(getDataType(tree->child[0]->attr.name, "\0") == Void)
                    semanticErr(tree, tree->child[0]->attr.name, currScope, "Funcao de retorno void nao pode ser operando");
            } 
            if (tree->child[2]->kind.exp == CallK)
            {
                if(getDataType(tree->child[2]->attr.name, "\0") == Void)
                    semanticErr(tree, tree->child[2]->attr.name, currScope, "Funcao de retorno void nao pode ser operando");
            } 
            break;
        
        case CallK:
            if(search_ST(tree->attr.name, "\0") == 0){
                semanticErr(tree, tree->attr.name, currScope, "Chamada de funcao nao declarada");
                return;
            } 
            insert_ST(tree->attr.name, tree->lineno, 0, "\0", 0, 0, tree, NULL);
            
        break;

        default:
            break;
        }
        break;
    
    default:
        break;
    }
}


//############################ Chamadas da Analise Semantica #####################################

void build_ST(TreeNode * tree){
    insert_ST("input", -1,currMem++,"\0",Integer,FunDeclK, NULL, NULL);
    insert_ST("output", -1,currMem++,"\0",Void,FunDeclK, NULL, NULL);
    traverse(tree, nullProc, insertTNode);
}

void check_ST(TreeNode * tree){
    checkMain();
    traverse(tree, nullProc, checkTNode);
}