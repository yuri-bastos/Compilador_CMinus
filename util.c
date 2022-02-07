/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: util.c               	    */
/* 		Funções de Utilidade para manipulação       */
/*			 dos nós Árvore Sintática				*/
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedimento printToken imprime um token*/
 void printToken( TokenType token, const char* tokenString )
 { switch (token)
   { case ELSE: fprintf(listing, "ELSE\n"); break;
     case IF: fprintf(listing, "IF\n"); break;
     case INT: fprintf(listing, "INT\n"); break;
     case RETURN: fprintf(listing, "RETURN\n"); break;
     case VOID: fprintf(listing, "VOID\n"); break;
     case WHILE: fprintf(listing, "WHILE\n"); break;
     case PLUS:     fprintf(listing, "+\n");  break;
     case MINUS:    fprintf(listing, "-\n");  break;
     case MULT:    fprintf(listing, "*\n");  break;
     case DIVI:     fprintf(listing, "/\n");  break;
     case LT:       fprintf(listing, "<\n");  break;
     case LET:       fprintf(listing, "<=\n"); break;
     case GT:       fprintf(listing, ">\n");  break;
     case GET:       fprintf(listing, ">=\n"); break;
     case EQUAL:       fprintf(listing, "==\n"); break;
     case DIFF:       fprintf(listing, "!=\n"); break;
     case ASSIGN:   fprintf(listing, "=\n");  break;
     case SEMI:     fprintf(listing, ";\n");  break;
     case COMMA:    fprintf(listing, ",\n");  break;
     case LPAR:   fprintf(listing, "(\n");  break;
     case RPAR:   fprintf(listing, ")\n");  break;
     case LBRT: fprintf(listing, "[\n");  break;
     case RBRT: fprintf(listing, "]\n");  break;
     case LBRC:   fprintf(listing, "{\n");  break;
     case RBRC:   fprintf(listing, "}\n");  break;
     case ENDFILE:  fprintf(listing,"%s %s\n",  "ENDFILE", "EOF"); break;
     case NUM: fprintf(listing, "NUM, val = %s\n",tokenString); break;
     case ID: fprintf(listing, "ID, name = %s\n",tokenString); break;
     case ERROR: fprintf(listing, "%s",tokenString); break;
     default: 
       fprintf(listing,"Token Desconhecido: %d\n",token);
   }
 }

/* Função newStmtNode cria um novo nó do tipo Statement */
TreeNode * newStmtNode(StmtKind kind) {
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL) fprintf(listing,"Out of memory error na linha %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Função newExpNode cria um novo nó do tipo Expressão */
TreeNode * newExpNode(ExpKind kind) {
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL) fprintf(listing,"Out of memory error na linha %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

/* Função newDeclNode cria um novo nó do tipo Declaração */
TreeNode * newDeclNode(DeclKind kind) {
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error na linha %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DeclK;
    t->kind.decl = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

/* Função copyString cria uma cópia de uma string existente */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error na linha %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* A variável indentno salva o número de identação para imprimir árvore e subárvores */
static int indentno = 0;

/* Macros para aumentar/diminuir a identação */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces faz identações imprimindo espaços */
static void printSpaces(void) {
  int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* printType imprime tipos de função e de variáveis */
void printTypes(TreeNode* tree) {
  if (tree->child[0] != NULL) {
    switch (tree->child[0]->type) {
      case Integer:
        fprintf(listing,"int");
        break;
      case Void:
        fprintf(listing,"void");
        break;
      case ArrayInteger:
        fprintf(listing,"int array");
        break;
      default: return;
    }
  } else {
    switch (tree->type) {
      case Integer:
        fprintf(listing,"int");
        break;
      case Void:
        fprintf(listing,"void");
        break;
      case ArrayInteger:
        fprintf(listing,"int array");
        break;
      default: return;
    }
  }
}

/* Procedimento printTree imprime a árvore sintática utilizando identação para designar subárvores */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {

    // IfK, WhileK, CompoundK, ReturnK
    if (tree->nodekind==StmtK) {
      printSpaces();
      switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case WhileK:
          fprintf(listing, "While\n");
          break;
        case CompK:
          fprintf(listing, "Composicao Decl\n");
          break;
        case ReturnK:
          fprintf(listing, "Return\n");
          break;
        case AssignK:
          fprintf(listing, "Atribuicao de valor\n");
          break;
        default:
          fprintf(listing, "stmtNode Desconhecido\n");
          break;
        }
    }
    // OpK, ConstK, IdK, TypeK, ArrayK, CallK, CalcK
    else if (tree->nodekind==ExpK) {
      if (tree->kind.exp != TypeK) printSpaces();
      switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Operador: ");
          printToken(tree->attr.op, "\0");
          break;
        case ConstK:
          fprintf(listing,"Constante: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Identificador: %s\n",tree->attr.name);
          break;
        case TypeK: break;
        case ArrayK:
          fprintf(listing,"Array \n");
          break;
        case CallK:
          fprintf(listing, "Chamada de Funcao : %s\n", tree->attr.name);
          break;
        case CalcK:
          fprintf(listing, "Operacao : ");
          printToken(tree->child[1]->attr.op, "\0");
          break;
        default:
          fprintf(listing,"ExpNode desconhecido\n");
          break;
      }
    }
    // VarDeclK, FunDeclK, ArrayDeclK, ArrayParDeclK, FunParDeclK
    else if (tree->nodekind==DeclK) {
      printSpaces();
      switch (tree->kind.decl) {
        case FunDeclK :
          fprintf(listing, "Declaracao de Funcao:  ");
          printTypes(tree);
          fprintf(listing, " %s()\n", tree->attr.name);
          break;
        case VarDeclK :
          fprintf(listing, "Declaracao de Variavel:  ");
          printTypes(tree);
          fprintf(listing, " %s;\n", tree->attr.name);
          break;
        case ArrayDeclK :
          fprintf(listing, "Declaracao de Array:  ");
          printTypes(tree);
          fprintf(listing," %s[%d];\n", tree->attr.arr.name, tree->attr.arr.size);
          break;
        case ArrayParDeclK :
          fprintf(listing, "Parametro de Array: %s\n", tree->attr.name);
          break;
        case FunParDeclK :
          fprintf(listing, "Parametro: ");
          printTypes(tree);
          if (tree->attr.name != NULL) {
            fprintf(listing, " %s\n", tree->attr.name);
          } else {
            fprintf(listing, " void\n");
          }
          break;
        default:
          fprintf(listing, "Declaracao Desconhecida\n");
          break;
        }
      }
    else fprintf(listing,"Tipo desconhecido\n");
    for (i=0;i<MAXCHILDREN;i++) if (tree->child[i] != NULL) printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
