# Compilador_CMinus

### ETAPA 5: ANALISE SEMANTICA

## ARQUIVOS:


	analyze.c   ---->   Arquivo com funções para analise semantica da arvore sintatica
	analyze.h   ---->   Arquivo com interface analyze
	cminus.l	---->	Análise Léxica Editado (USAR NO FLEX)
	cminus.y	----> 	Análise Sintática Editada (USAR NO BISON)
	globals.h	---->	Arquivo com variáveis globais
	main.c		---->	Arquivo com função main
	parse.h		---->	Arquivo com interface parse
	scan.h		---->	Arquivo com interface scanner
	symtab.c	---->	Arquivo com funcoes para geracao da Tabela de Simbolos
	symtab.h    ---->	Arquivo com interface symtab
	util.c		---->	Arquivo com funções para geração da Árvore Sintática
	util.h		---->	Arquivo com interface das funções de Árvore Sintática
	
	mdc.cm		---->	Arquivo de teste (MDC)
	sort.cm		---->	Arquivo de teste (sort)
	

	


## COMANDOS:


	(navegue até a pasta de cminus.l)

	flex cminus.l
	bison -d cminus.y
	gcc *.c -o Compile
	Compile.exe mdc.cm (ou outro arquivo de teste)

## OBS:
  
	Nao foi possivel finalizar a Etapa 6 de Geracao do Codigo Intermediario
