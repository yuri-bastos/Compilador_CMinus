/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel Souza Paiva - 140620				*/
/*													*/
/* 				Arquivo: cminus.l                   */
/* 			Especificacao Lexica pra C-             */
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/
			
			///////////////
			//DEFINICOES//
			/////////////
%{
	#include "globals.h"
	#include "util.h"
	#include "scan.h"

char tokenString[MAXTOKENLEN+1];


%}

%option noyywrap yylineno 

digito       [0-9]
numero      {digito}+
letra      [a-zA-Z]
identificador  {letra}+
novalinha    \n
espaco  [\ \t]+






%%

			///////////////
			////REGRAS////
			/////////////

"else"          {printf(" %s -> ELSE \n", yytext) ; return ELSE;}
"Else"          {printf(" %s -> ELSE \n", yytext) ; return ELSE;}
"ELSE"          {printf(" %s -> ELSE \n", yytext) ; return ELSE;}
"if"            {printf(" %s -> IF \n", yytext) ; return IF;}
"If"            {printf(" %s -> IF \n", yytext) ; return IF;}
"IF"            {printf(" %s -> IF \n", yytext) ; return IF;}
"int"           {printf(" %s -> INT \n", yytext) ; return INT;}
"Int"           {printf(" %s -> INT \n", yytext) ; return INT;}
"INT"           {printf(" %s -> INT \n", yytext) ; return INT;}
"return"        {printf(" %s -> RETURN \n", yytext) ; return RETURN;}
"Return"        {printf(" %s -> RETURN \n", yytext) ; return RETURN;}
"RETURN"        {printf(" %s -> RETURN \n", yytext) ; return RETURN;}
"void"          {printf(" %s -> VOID \n", yytext) ; return VOID;}
"Void"          {printf(" %s -> VOID \n", yytext) ; return VOID;}
"VOID"          {printf(" %s -> VOID \n", yytext) ; return VOID;}
"while"         {printf(" %s -> WHILE \n", yytext) ; return WHILE;}
"While"         {printf(" %s -> WHILE \n", yytext) ; return WHILE;}
"WHILE"         {printf(" %s -> WHILE \n", yytext) ; return WHILE;}


"+"             {printf(" %s -> PLUS \n", yytext) ; return PLUS;}
"-"             {printf(" %s -> MINUS \n", yytext) ; return MINUS;}
"*"             {printf(" %s -> MULT \n", yytext) ; return MULT;}
"/"             {printf(" %s -> DIVI \n", yytext) ; return DIVI;}

";"             {printf(" %s -> SEMI \n", yytext) ; return SEMI;}
","             {printf(" %s -> COMMA \n", yytext) ; return COMMA;}
"="             {printf(" %s -> ASSIGN \n", yytext) ; return ASSIGN;}
"=="            {printf(" %s -> EQUAL \n", yytext) ; return EQUAL;}
"!="            {printf(" %s -> DIFF \n", yytext) ; return DIFF;}
"<"             {printf(" %s -> LT \n", yytext) ; return LT;}
"<="            {printf(" %s -> LET \n", yytext) ; return LET;}
">"             {printf(" %s -> GT \n", yytext) ; return GT;}
">="            {printf(" %s -> GET \n", yytext) ; return GET;}


"("             {printf(" %s -> LPAR \n", yytext) ; return LPAR;}
")"             {printf(" %s -> RPAR \n", yytext) ; return RPAR;}
"{"             {printf(" %s -> LBRC \n", yytext) ; return LBRC;}
"}"             {printf(" %s -> RBRC \n", yytext) ; return RBRC;}
"["             {printf(" %s -> LBRT \n", yytext) ; return LBRT;}
"]"             {printf(" %s -> RBRT \n", yytext) ; return RBRT;}

{numero}        {printf(" %s -> NUM \n", yytext) ; return NUM;}
{identificador}    {printf(" %s -> ID \n", yytext) ; return ID;}
{novalinha}       { lineno++;}
{espaco}    {}

"/*" { char c;
                  while (1) {
                    c = input();
                    if (c == EOF) break;
                    if (c == '\n') lineno++;
                    while ((c = input()) == '*');
                    if (c == '/') break;
                  }
                }
				
.               {printf("ERRO LÉXICO : %s LINHA: %d\n", yytext, lineno) ; return ERROR;}






%%

			///////////////
			////CODIGO////
			/////////////


TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}
