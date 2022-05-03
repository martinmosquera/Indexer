#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "indexer.h"

void imprimeHead(){
    printf("\n==========================================================");
    printf("\n================BENVINDO AO SISTEMA INDEXER===============\n");
}

void showMenu(){
    printf("\n\t= Arquivo carregado com sucesso\n");
    printf("\n\t= Digite o numero correspondete a uma das Opcoes:\n");
    printf("\n\t| 1) - N palavras mais Frequentes\n");
    printf("\n\t| 2) - Frequenca da palavra\n");
    printf("\n\t| 3) - Procurar palavra em arquivos\n");
    printf("\n\t| 4) - Sair\n");
    printf("\n\t\t\t| R :");
}

ARQ getFile(char* nome){
   ARQ file = fopen(nome,"r");
   if(!file)
   return NULL;
   else return file;
}

Indexer carregaTabela(ARQ file){
   String palavra;
   Indexer indexer;
   indexer = initTable(97);
   int count=0;
   while (feof(file)== 0){
      fscanf(file,"%s",palavra);
      char* word;
      word = getStringClean(palavra);
      if(strcmp(word,"")==0) continue;
      indexer = insereFila(indexer,word);
      count++;
   }
   fclose(file);
   return indexer;
}

Indexer carregaTabelaFreqN(ARQ arquivo,int num){
   String palavra;
   int count=0; 
   Indexer indexer = initTable(97);
   Fila filas[num];
    indexer->taux = filas;
    while(feof(arquivo) == 0){
        fscanf(arquivo,"%s",palavra);
        if(strcmp(getStringClean(palavra),"")==0) continue;
        indexer = insereFila(indexer,palavra);
        count++;
    }
    
    printf("numero de busca: %d",num);
    fclose(arquivo);
    return indexer;
}

Indexer carregaTabelaFeqP(ARQ arquivo,String palavra){
  printf("Palavra a perquisar: %s",palavra);
  int count=0;
  Indexer indexer;
   indexer = initTable(97);
    while(feof(arquivo) == 0){
        fscanf(arquivo,"%s",palavra);
        if(strcmp(getStringClean(palavra),"")==0) continue;
        printf("%s",palavra);
        indexer = insereFila(indexer,palavra);
        count++;
    } 
    fclose(arquivo);
    return indexer;
}

Indexer carregaTabelaSearch(ARQ Arquivos[],String palavra,int tam){
   printf("Palavra a perquisar: %s",palavra);
   Indexer indexer;
   for(int j =0;j< tam;j++){
        ARQ arquivo = Arquivos[j];
       int count=0;
        indexer = initTable(97);
        while(feof(arquivo) == 0){
        fscanf(arquivo,"%s",palavra);
        if(strcmp(getStringClean(palavra),"")==0) continue;
        indexer = insereFila(indexer,palavra);
        count++;
    } 
    fclose(arquivo);
   }
    return indexer;
}

char* getStringClean(String dado){
    char c;
    if(strlen(dado)<3) return "";
    int tam = strlen(dado);
    for(int i=0;i<tam;i++){
        c = dado[i];
        if(c < 65 || (c > 90 && c <97) || c > 122){
            dado = "";
            return dado;
        }
        if(c>54 && c <91){
            c = tolower(c);
            dado[i] = c; 
        }
    }
    if(strcmp(dado,"the")==0) strcpy(dado,"");
    if(strcmp(dado,"for")==0) strcpy(dado,"");
    if(strcmp(dado,"but")==0) strcpy(dado,"");
    if(strcmp(dado,"all")==0) strcpy(dado,"");
    if(strcmp(dado,"are")==0) strcpy(dado,"");
    if(strcmp(dado,"and")==0) strcpy(dado,"");
    if(strcmp(dado,"was")==0) strcpy(dado,"");
    if(strcmp(dado,"this")==0) strcpy(dado,"");
    return dado;
}


Table* initTable(int tam){
    Table* t = (Table*)malloc(sizeof(Table));
    if(t== NULL) return NULL;
    t->qtdLidas =0;
    t->qtdOcupando =0;
    t->taux = NULL;
    for(int i=0;i<tam;i++){
      t->filas[i].arv = NULL;
      strcpy(t->filas[i].keyFila,"");
      t->filas[i].qtdRepeticiones = 0;
    }
    TAM ta = getSize(tam);
    t->tamanho = ta;
    return t;
}

void freeTable(Table* t){
    if(t != NULL){
        int tam= getTamanho(t);
        for(int i=0;i<tam;i++){
            if((t->filas[i].arv) != NULL){
                free(t->filas[i].arv);  
            }
        }
        free(t);
        printf("tabela liberada com sucesso!\n");
    }
}

Table* insereFila(Table* t, String dado){
    int pos = hash(t,dado);
    int pos2 = hash_Alex(t,dado);
    int tam = getTamanho(t);
    int res = (pos*pos2)%tam;
    if(t == NULL) return NULL;
    
    // verifica se nao tem nada na fila
  if(strcmp(t->filas[pos].keyFila,"")==0){
        insereInNull(t,dado,pos);
        return t;
    }
    // verifica se es o mesmo dado
    if(strcmp(t->filas[pos].keyFila,dado)==0){
        t->filas[pos].qtdRepeticiones++;
        return t;
    }

    // verifica o tamanho da tabela =>  4 > n/m < 8  n -> produndiade m->qtd elementos 
    int result = 0;
    result = (t->filas[pos].arv->profundidade/t->qtdOcupando);
    if(result > 4){
      t = aumentaTamnaho(t,dado);
    }
    // procura pela segunda posicao posivel
    if(strcmp(t->filas[pos2].keyFila,"")==0){
        insereInNull(t,dado,pos2);
        return t;
        // procura pela ultima posicao posivel
    }else if(strcmp(t->filas[res].keyFila,"")==0){
        insereInNull(t,dado,res);
        return t;
    }else{
        if(t->filas[pos].arv->root == NULL){
        
        t = insere_arv(t,t->filas[pos].arv->root,dado);
        printf("%s\n",t->filas[pos].arv->keyRoot);
        }
    }
        
    return t;
}

int hash(Table *t,String dado){
    int len = strlen(dado);
    unsigned int h = SEED;
    int i;
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    h ^= 2166136261UL;
    String data;
    strcpy(data,dado);
    for(i = 0; i < len; i++)
    {
        h ^= data[i];
        h *= 16777619;
    }
    i = getTamanho(t);
    return (h%i);
}

int hash_Alex(Table *t,String dado){
    char c;
    int p = 31;
    int tamanho = getTamanho(t);
    int hash_value = 0, p_pow = 1;

    while ((c = *dado++)){
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) %tamanho;
        p_pow = (p_pow * p) %tamanho;
    }
    return(hash_value);
}

void insereInNull(Table *t,String entrando,int pos){
    
       (t->filas[pos].arv) = (struct arvore*)malloc(sizeof(struct arvore));
        strcpy(t->filas[pos].keyFila,entrando); 
        strcpy(t->filas[pos].arv->keyRoot,entrando);
        t->filas[pos].arv->profundidade =0;
        t->filas[pos].qtdRepeticiones = 1;    
        t->qtdLidas++;
        t->qtdOcupando++;
}

Table* aumentaTamnaho(Table* t,String dado){
    int tamIni,tamFinal;
    tamIni = getTamanho(t);
    t->tamanho = (t->tamanho+1);
    tamFinal = getTamanho(t);
    Table* newTable = initTable(tamFinal);
    newTable = insereFila(newTable,dado);
    Nodo* nodo,*aux; 
    for(int i = 0; i<tamIni;i++){
        // se o arv nao es null
        if(t->filas[i].arv->profundidade>0){
            nodo = t->filas[i].arv->root;
            while(nodo != NULL){
                insereFila(newTable,nodo->keyNodo);
                aux = nodo->dir;
                free(aux);
            }
        }
        free(t->filas[i].arv);
        insereFila(newTable,t->filas[i].keyFila);
    }
    free(t);
    return(newTable);
}

int getTamanho(Table* t){

    int i;
    if(t->tamanho == XSMALL) i = 97;
    else if(t->tamanho == SMALL) i = 1033;
    else if(t->tamanho == MEDIUM) i = 10093;
    else if(t->tamanho == LARGE) i = 50053;
    else if(t->tamanho == XLARGE) i = 100043;
    return i;
}

TAM getSize(int num){
    TAM t;
    if(num == 97) t = XSMALL;
    else if (num == 1033) t= SMALL;
    else if(num == 10093) t = MEDIUM;
    else if(num == 50053) t = LARGE;
    else if(num == 100043) t = XLARGE;
    return t;
}


//// reimplementacao do arvore red black

void trocar_cor(Nodo* c){
    c->color = !c->color;
    if(c->ezq != NULL)
        c->ezq->color = !c->ezq->color;
    if(c->dir != NULL)
        c->dir->color = !c->dir->color;    
}

int cor(Nodo* c){
    if(c->color==0)
        return BLACK;
    else    
        return c->color;
}

Nodo* rotacaoEzq(Nodo* nodo){
    Nodo* aux = nodo->dir;
    nodo->dir = aux->ezq;
    aux ->ezq = nodo;
    aux->color = nodo->color;
    nodo->color = RED;
    return aux;
}

Nodo* rotacaoDir(Nodo* nodo){
    Nodo* aux = nodo->ezq;
    nodo->ezq = aux->dir;
    aux ->dir = nodo;
    aux->color = nodo->color;
    nodo->color = RED;
    return aux;
}

Nodo* move2EzqR(Nodo* nodo){
    trocar_cor(nodo);
    if(cor(nodo->dir->ezq)==RED){
        nodo->dir = rotacaoDir(nodo->dir);
        nodo = rotacaoEzq(nodo);
        trocar_cor(nodo);
    }
    return nodo;
}

Nodo* move2DirR(Nodo* nodo){
    trocar_cor(nodo);
    if(cor(nodo->ezq->ezq)==RED){
        nodo = rotacaoDir(nodo);
        trocar_cor(nodo);
    }
    return nodo;
}


Nodo* balancear(Nodo* nodo){

    if(cor(nodo->dir)==RED)
        nodo = rotacaoEzq(nodo);
    if(nodo->ezq != NULL && cor(nodo->dir)==RED && cor(nodo->ezq->ezq) == RED)
        nodo = rotacaoDir(nodo);
    if(cor(nodo->ezq)==RED && cor(nodo->dir)==RED)
        trocar_cor(nodo);
    return nodo;
}

Table* insere_arv(Table* t,struct nodo *raiz,String valor){
    raiz = insereNO(t,raiz,valor);
    if((raiz)!=NULL)
        (raiz)->color = BLACK;
    int pos = hash(t,valor);
    t->filas[pos].arv->root = raiz;
    return t;
}

Nodo* insereNO(Table* t,Nodo* nodo,String valor){
    
    if(nodo == NULL){
        Nodo* novo;
        novo = (struct nodo*)malloc(sizeof(struct nodo));
        if(novo == NULL)
            return NULL;
        strcpy(novo->keyNodo,valor);
        novo->color = RED;
        novo->dir = NULL;
        novo->ezq = NULL;
        t->qtdLidas++;
        t->qtdOcupando++;
        return novo;
    }
    if(strcmp(valor,nodo->keyNodo)==0){
      nodo->qtdRepeticoes++;

    }
    if(strcmp(valor,nodo->keyNodo)<0){
        nodo->ezq = insereNO(t,nodo->ezq,valor);
    }
        
    else{
        nodo->dir = insereNO(t,nodo->dir,valor);
    }
        
    if(nodo->ezq != NULL && nodo->dir != NULL){

        if(cor(nodo->dir)== RED && cor(nodo->ezq) == BLACK){
            nodo = rotacaoEzq(nodo);
        }
            
        if(cor(nodo->ezq)==RED && cor(nodo->dir)==RED){
            trocar_cor(nodo);
        }
    }
    
    if(nodo->ezq != NULL && nodo->ezq->ezq != NULL){
        if(cor(nodo->ezq)==RED && cor(nodo->ezq->ezq)==RED){
        nodo = rotacaoDir(nodo);
    }
    }
    
    return nodo;
}

void printArv(Nodo* nodo){
    if(!nodo) return;
    printArv(nodo->ezq);
    printf(" valor %s \n",nodo->keyNodo);
    printArv(nodo->dir);
}