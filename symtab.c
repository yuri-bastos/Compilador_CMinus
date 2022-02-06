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
#define SIZE 211

/* SHIFT e a potencia de 2 utilizada com indice alfa no calculo do hash */
#define SHIFT 4

/* Funcao de hash que retorna o indice que corresponde a dada chave key */
int hash(char* key){
    int temp = 0;
    int i = 0;
    while (key[i] != '\0'){ 
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }
    return temp;
}

/* Lista encadeada que armazena o numero das linhas e a pos do prox item */
typedef struct linkedList{
    int lineno;
    struct linkedList *next;
} *lineList;

/* lista de baldes: contem listas dentro delas
 * Chaves:
    name = nome do simbolo
    lines = lista encadeada de linhas
    memloc = local de memoria
    scope = escopo do simbolo
    dataType = tipo do dado (int ou void)
    idType = tipo do id (fun var array)
    next = ponteiro pro prox elem
 */
typedef struct bList{
    /* Chaves: */
    char *name; //Nome do simbolo
    lineList lines; //Lista encadeada que marca as linhas de ocorrencia desse ID
    int memloc; //pos de memoria desse simbolo
    ScopeName scope; //Escopo do simbolo
    ExpType dataType; //Tipo do Dado(Int, Void) //Void, Integer, Array Integer
    DeclKind idType; //Tipo do ID(var fun array) //Usando como guia: DeclKind (Aqui eh o tipo que foi declarado para consultas futuras)
    //Obs.: usamos declKind pois o que conta em um simbolo eh como ele foi declarado, 
    //caso nao tenha declaracao previa, ja identifica um erro...
    
    /* Ponteiro pro prox: */
    struct bList *next;
} *bucketList;

/* Busca o end da lista de baldes de um Simbolo: 
 * caso encontre: retorna a lista
 * caso nao encontre: retorna NULL
 * 
 * Args:
 *  name = nome do simbolo
 *  scope = escopo desse simbolo
 */
bucketList search_ST(char *name, ScopeName scope){
    char *key = strcat(name,scope); //Concatenar o nome com o escopo para evitar colisoes...
    int h = hash(key);//h = valor de hashing p pos na tabela
    //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
    //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao
    bucketList l = hashTable[h]; //l = lista para o indice da tabela rel ao dado simbolo
    while(l != NULL){
        if ((strcmp(l->name, name) == 0) && (l->scope == scope)) break;
        l = l->next;
    }
    
    return l;
}

/* Tabela de Hash que estrutura a tabela de simbolos */
static bucketList hashTable[SIZE];

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
void insert_ST(char * name,  int lineno, int loc, ScopeName scope, ExpType dType, DeclKind iType){
    char *key = strcat(name,scope); //Concatenar o nome com o escopo para evitar colisoes...
    int h = hash(key);//h = valor de hashing p pos na tabela
    //IMPORTANTE: No caso de chamada ou declaracao de funcao, scope vira como "\0"
    //desse modo, seu hash é 0 e nao impacta no hash do nome da funcao

    bucketList l = search_ST(name,scope); //l recebe o end desse nome e escopo na tabelaHash

    if (l == NULL){ //Novo Simbolo -> Declarar (Contanto que seja uma declaracao)
        l->name = name;
        l->lines = (lineList) malloc(sizeof(struct linkedList)); //alocando espaco pra lista
        l->lines->lineno = lineno; //atribuindo a linha de declaracao
        l->lines->next = NULL;
        l->memloc = loc;
        l->scope = scope;
        l->dataType = dType;
        l->idType = iType;
        l->next = NULL;
        hashTable[h] = l;
    } else{ //Atualizar numero de linhas de ocorrencia
        lineList line = l->lines;
        while (line->next != NULL) line = line->next;
        line->next = (lineList) malloc(sizeof(struct linkedList));
        line->next->lineno = lineno;
        line->next->next = NULL;
    }    
}

/* Imprime a tabela de hash no formato da tabela de simbolos
 * propria e graficamente dita. usa o mesmo arquivo de saida
 * listing usado pelas demais partes do compilador
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

                fprintf(listing, "%-5d  ", l->memloc);//Imprimindo o local de memoria
                
                //Impressao dos num das linhas:
                lineList line = l->lines;
                while (line != NULL)
                {
                    fprintf(listing, "%2d", line->lineno);
                    line = line->next;
                }

                fprintf(listing,"\n");//Pulando linda da tabela impressa para o prox simbolo
                l = l->next;//Indo para o prox simbolo dessa entrada (colisoes)
            }
        }
    }
}