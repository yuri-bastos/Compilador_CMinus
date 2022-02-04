# Compilador_CMinus
//////////////// ETAPA 3 ////////////////////////
/////////// Árvore Sintática ////////////////////
/////////////////////////////////////////////////

		ARQUIVOS:



	cminus.l	---->	Análise Léxica Editado (USAR NO FLEX)
	cminus.y	----> 	Análise Sintática Editada (USAR NO BISON)
	globals.h	---->	Arquivo com variáveis globais
	main.c		---->	Arquivo com função main
	parse.h		---->	Arquivo com interface parse
	scan.h		---->	Arquivo com interface scanner
	util.c		---->	Arquivo com funções para geração da Árvore Sintática
	util.h		---->	Arquivo com interface das funções de Árvore Sintática

	mdc.cm		---->	Arquivo de teste (MDC)
	sort.cm		---->	Arquivo de teste (sort)

	lex.yy.c	---->	Arquivo gerado no final da Etapa 1 REGERADO
	cminus.tab.h	---->	Arquivo gerado no final da Etapa 2 REGERADO
	cminus.tab.c	---->	Arquivo gerado no final da Etapa 2 REGERADO
	

	


		COMANDOS:


	(navegue até a pasta de cminus.l)

	flex cminus.l
	bison -d cminus.y
	gcc *.c -o Compile
	Compile.exe mdc.cm (ou outro arquivo de teste)
