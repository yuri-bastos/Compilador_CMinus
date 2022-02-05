/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: scan.h              	    */
/* 	   Interface de Scanner para Compilador C-      */
/*			 										*/
/*													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN é o tamanho máximo de um token */
#define MAXTOKENLEN 40

/* O vetor tokenString guarda o lexema em cada token */
extern char tokenString[MAXTOKENLEN+1];

/* Função getToken retorna o próximo token da entrada */
TokenType getToken(void);

#endif
