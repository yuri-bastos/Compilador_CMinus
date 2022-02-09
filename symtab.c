/****************************************************/
/*						Alunos:						*/
/*													*/
/*			Yuri Sodre Bastos  - 140547				*/
/*			Daniel de Souza Paiva - 140620			*/
/*													*/
/* 				Arquivo: symtab.c             	    */
/* 	   Implementação da Tabela de Simbolos para o  	*/
/*			      compilador de c-	  		    	*/
/* 													*/
/* 				Baseado no Compilador		    	*/
/* 		  para TINY de Kenneth C. Louden 			*/
/****************************************************/
#include "globals.h"
#include "symtab.h"

/* SIZE representa o tamanho maximo da tabela de hash */
#define SIZE 311

/* SHIFT e a potencia de 2 utilizada com indice alfa no calculo do hash */
#define SHIFT 4

/* Funcao de hash que retorna o indice que corresponde a dada chave key */
int hash(char* key){
    int temp = 0;
    int i = 0;
    while (key[i] != '\0'){ 
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        i++;
    }
    return temp;
}

/* Tabela de Hash que estrutura a tabela de simbolos */
bucketList hashTable[SIZE];

/* Busca a existencia ou nao de um simbolo: 
 * caso encontre: retorna 1
 * caso nao encontre: retorna 0
 * 
 * Args:
 *  name = nome do simbolo
 *  scope = escopo desse simbolo
 */
int search_ST(char *name, ScopeName scope){
    char key[300] = "";
    strcat(key,name);//concatenando o nome na chave
    strcat(key,scope);//concatenando o escopo na chave
    
    int h = hash(key);//h = valor de hashing p pos na tabela
    //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
    //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao
    bucketList l = hashTable[h]; //l = lista para o indice da tabela rel ao dado simbolo
    while(l != NULL){
        if ((strcmp(l->name, name) == 0) && (l->scope == scope)) return 1;
        l = l->next;
    }
    
    return 0;
}

/* Insere na tabela de simbolos (tabela hash) um simbolo, colocando
 * seu escopo, local de memoria e atualizando os numeros de linha.
 * Caso o simbolo ja exista, atualiza apenas a lista de numero de linhas...
 * 
 * Args:
 *  name = nome do simbolo
 *  lineno = num da linha em que aparece
 *  loc = local de memoria (use memloc)
 *  scope = escopo onde se localiza
 *  dType = tipo do dado (Int, Void)
 *  iType = tipo do id (Var, Fun, Array)
 * 
 * Funcinamento: Realizar busca na tabela de simbolos para ver se aquele ID
 * existe. Caso exista, atualizar n da linha, caso a busca retorne NULL,
 * realizar procedimento de insercao atualizando a bucketList do indice 
 * na tabela hash com o id encontrado, caso seja valido...
 */
void insert_ST(char * name,  int linenum, int loc, ScopeName scope, ExpType dType, DeclKind iType, TreeNode * declNode, funParsList params){    
    
    char key[300] = "";
    strcat(key,name);//concatenando o nome na chave
    strcat(key,scope);//concatenando o escopo na chave
    int h = hash(key);//h = valor de hashing p pos na tabela
    //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
    //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao

    bucketList l = hashTable[h]; //l = lista para o indice da tabela rel ao dado simbolo
    while(l != NULL){
        if ((strcmp(l->name, name) == 0) && (l->scope == scope)) break;
        l = l->next;
    }

    if (l == NULL){ //Novo Simbolo -> Declarar (Contanto que seja uma declaracao)
        l = (bucketList) malloc(sizeof(struct bList));
        l->name = name;
        l->lines = (lineList) malloc(sizeof(struct lineListK)); //alocando espaco pra lista
        l->lines->lineNum = linenum; //atribuindo a linha de declaraca    o
        l->lines->next = NULL;
        l->memloc = loc;
        l->scope = scope;
        l->dataType = dType;
        l->idType = iType;
        l->next = NULL;
        hashTable[h] = l;
        l->node = declNode;
        l->funcPars = params;

    } else{ //Atualizar numero de linhas de ocorrencia
        lineList line = l->lines;
        while (line->next != NULL) line = line->next;
        line->next = (lineList) malloc(sizeof(struct lineListK));
        line->next->lineNum = linenum;
        line->next->next = NULL;
    }    
}

/* Imprime a tabela de hash no formato da tabela de simbolos
 * propria e graficamente dita. usa o mesmo arquivo de saida
 * listing usado pelas demais partes do compilador
 * 
 *  Args:
 *   listing = arquivo de saida de trace
 */
void print_ST(FILE *listing){
    int i;
    char *idtStr, *dtStr;
    
    //cabecalho:
    fprintf(listing,"ENTRADA  NOME ID        ESCOPO    TIPO ID  TIPO DADO  MEMLOC  N.LINHAS\n");
    fprintf(listing,"-------  -------------  --------  -------  ---------  ------  --------\n");
    
    //impressao das linhas e colunas da tabela
    for (i = 0; i < SIZE; i++)
    {
        if (hashTable[i] != NULL)
        {
            bucketList l = hashTable[i];
            while (l != NULL) //Percorrendo a lista da entrada atual da tabela
            {   
                if(( strcmp(l->name, "input") != 0 || l->lines->next != NULL) && ( strcmp(l->name, "output") != 0 || l->lines->next != NULL))//Impedir que input e output aparecam caso nao tenham sido usadas
                {
                    fprintf(listing, "%-7d  ", i);//imprimindo a entrada na tabela hash
                    fprintf(listing, "%-13s  ", l->name);//imrpimindo o nome do ID
                    fprintf(listing, "%-8s  ", l->scope);//imprimindo o escopo
                    
                    //Encontrando qual o tipo do id para impressao
                    switch (l->idType)
                    {
                        case FunDeclK:
                            idtStr = "fun";
                            break;
                        
                        case VarDeclK:
                            idtStr = "var";
                            break;

                        case ArrayDeclK:
                            idtStr = "arr";
                            break;

                        case ArrayParDeclK:
                            idtStr = "arr";
                            break;
                        default:
                            idtStr = "\0";
                            break;
                    }
                    fprintf(listing, "%-7s  ", idtStr);//Imprimindo o tipo do id

                    //Encontrando o tipo do dado para impressao
                    switch (l->dataType)
                    {
                        case Integer:
                            dtStr = "int";
                            break;

                        case Void:
                            dtStr = "void";
                            break;
                        
                        case ArrayInteger:
                            dtStr = "int array";
                            break;

                        default:
                            dtStr = "\0";
                            break;
                    }
                    fprintf(listing, "%-9s  ", dtStr);//Imprimindo o tipo do dado

                    fprintf(listing, "%-6d  ", l->memloc);//Imprimindo o local de memoria
                    
                    //Impressao dos num das linhas:
                    lineList line = l->lines;
                    while (line != NULL)
                    {
                        if (line->lineNum != -1)
                        {
                            fprintf(listing, "%2d ", line->lineNum);
                        }                        
                        line = line->next;
                    }

                    fprintf(listing,"\n");//Pulando linda da tabela impressa para o prox simbolo
                }
                l = l->next;//Indo para o prox simbolo dessa entrada (colisoes)
            }
        }
    }
}

ExpType getDataType(char *name, ScopeName scope)
{
    char key[300] = "";
    strcat(key,name);//concatenando o nome na chave
    strcat(key,scope);//concatenando o escopo na chave
    
    int h = hash(key);//h = valor de hashing p pos na tabela
    //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
    //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao
    bucketList l = hashTable[h]; //l = lista para o indice da tabela rel ao dado simbolo
    while(l != NULL){
        if ((strcmp(l->name, name) == 0) && (l->scope == scope)) return l->dataType;
        l = l->next;
    }

    return -1;//error
}

DeclKind getIDType(char *name, ScopeName scope)
{
    char key[300] = "";
    strcat(key,name);//concatenando o nome na chave
    strcat(key,scope);//concatenando o escopo na chave
    
    int h = hash(key);//h = valor de hashing p pos na tabela
    //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
    //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao
    bucketList l = hashTable[h]; //l = lista para o indice da tabela rel ao dado simbolo
    while(l != NULL){
        if ((strcmp(l->name, name) == 0) && (l->scope == scope)) return l->idType;
        l = l->next;
    }

    return -1;//error
}

DeclKind getIDTypeGlobal(char *name, ScopeName scope)
{
    char key[300] = "";
    char glb[300] = "";
    strcat(key,name);//concatenando o nome na chave
    strcat(key,scope);//concatenando o escopo na chave
    
    int h = hash(key);//h = valor de hashing p pos na tabela
    //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
    //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao
    bucketList l = hashTable[h]; //l = lista para o indice da tabela rel ao dado simbolo
    while(l != NULL){
        if ((strcmp(l->name, name) == 0) && (l->scope == scope)) return l->idType;
        l = l->next;
    }
    if (l == NULL)
    {
        strcat(glb,name);//concatenando o nome na chave
        strcat(glb,scope);//concatenando o escopo na chave
        
        int h = hash(glb);//h = valor de hashing p pos na tabela
        //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
        //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao
        bucketList l = hashTable[h]; //l = lista para o indice da tabela rel ao dado simbolo
        while(l != NULL){
            if ((strcmp(l->name, name) == 0) && (l->scope == scope)) return l->idType;
            l = l->next;
        }
    }
    
    return -1;//error
}

TreeNode * getFuncNode(char *funcName)
{
    char key[300] = "";
    strcat(key,funcName);//concatenando o nome na chave
    strcat(key,"\0");//concatenando o escopo na chave
    
    int h = hash(key);//h = valor de hashing p pos na tabela
    //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
    //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao
    
    bucketList l = hashTable[h]; //l = lista para o indice da tabela rel ao dado simbolo
    while(l != NULL){
        if ((strcmp(l->name, funcName) == 0) && (l->scope == "\0")) return l->node;
        l = l->next;
    }

    return NULL;//error
}

funParsList getFuncParams_ST(char *funcName)
{
    char key[300] = "";
    strcat(key,funcName);//concatenando o nome na chave
    strcat(key,"\0");//concatenando o escopo na chave
    
    int h = hash(key);//h = valor de hashing p pos na tabela
    //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
    //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao
    
    bucketList l = hashTable[h]; //l = lista para o indice da tabela rel ao dado simbolo
    while(l != NULL){
        if ((strcmp(l->name, funcName) == 0) && (l->scope == "\0")) return l->funcPars;
        l = l->next;
    }
    
    return NULL;//Nao encontrado->ERROR
}

/*
 * 0 -> sem erros
 * 1 -> Muitos parametros para chamar a funcao
 * 2 -> Tipos dos parametros incompativeis
 */
int compParamLists(char *funcName, funParsList callParams)
{
    int sizeDecl = 0, sizeCall = 0;
    funParsList funcPars = getFuncParams_ST(funcName);
    funParsList aux1 = funcPars, aux2 = callParams;

    while (aux1->next != NULL)
    {
        sizeDecl++;
        aux1=aux1->next;
    }
    while (aux2->next != NULL)
    {
        sizeCall++;
        aux2=aux2->next;
    }

    if (sizeDecl != sizeCall) return 1; //erro do tipo muitos parametros chamados
    else
    {   aux1 = funcPars;
        aux2 = callParams;
        while (aux1->next != NULL)
        {
            if(aux1->par != aux2->par) return 2; //erro do tipo parametros incompativeis
            aux1=aux1->next;
            aux2=aux2->next;
        }
    }
    return 0;//passou
}

