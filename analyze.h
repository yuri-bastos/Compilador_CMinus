/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: analyze.h            	    */
/* 	   Interface do Analisador Semantico para o	    */
/*			 		compilador de c-				*/
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/

#ifndef _ANALYZE_H_
#define _ANALYZE_H_

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
static void semanticErr(TreeNode * tree, char *id,  ScopeName scope, char *msg);

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
static void preCheckScopeStart(TreeNode *tree);

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
static void postCheckScopeEnd(TreeNode *tree);

/*
 * static void checkMain()
 * 
 * Verifica a existencia da funcao main na tabela de simbolos
 * caso nao exista, lanca um erro e o sinaliza caso TraceAnalyze esteja ativado
 * 
 * IMPORTANTE: fazer essa checagem deve ser pelo menos, DEPOIS do fim do processo de insercao!
 */
static void checkMain();

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
static void traverse( TreeNode * tree, void (* preProc) (TreeNode *), void (* postProc) (TreeNode *) );

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
static void nullProc(TreeNode * tree);

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
void insertTNode(TreeNode * tree);

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
//void checkTNode(TreeNode * tree);

/* 
 * static void setupGlobals()
 * 
 * Inicializa as funcoes globais: int input() e void output(int)
 */
void insertGlobals(int loc);

void build_ST(TreeNode * tree);

void typeCheck_ST(TreeNode * tree);

#endif