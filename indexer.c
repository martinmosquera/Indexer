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
    if(strcmp(dado,"")==0) return t;

    if(strcmp(t->filas[pos].keyFila,dado)==0){
        t->filas[pos].qtdRepeticiones++;
        return t;
    }
        int result = 0;
        result = (t->filas[pos].arv->profundidade/t->qtdOcupando);
        if(result > 7){
            t = aumentaTamnaho(t,dado);
        }
    if(strcmp(t->filas[pos].keyFila,"")==0){
        insereInNull(t,dado,pos);
        return t;
    }else if(strcmp(t->filas[pos2].keyFila,"")==0){
        insereInNull(t,dado,pos2);
        return t;
    }else if(strcmp(t->filas[res].keyFila,"")==0){
        insereInNull(t,dado,res);
        return t;
    }else{
        if(t->filas[pos].arv == NULL){
        t->filas[pos].arv = create_arv(t,dado);
        }
        Nodo* nodo;
        nodo = create_nodo(dado);
        insert(t->filas[pos].arv,nodo);
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

Nodo* inserereInArvore(Table* t,String dado){
    
    int pos = hash(t,dado);
    Nodo * nodo = t->filas[pos].arv->root;
    Nodo* ant;
    while(nodo != NULL){
        if(strcmp(nodo->keyNodo,dado)==0){
            nodo->qtdRepeticoes++;
            t->qtdLidas++;
            return nodo;
        }
        ant =nodo;
        nodo = nodo->dir;
    }
    ant->dir = (Nodo*)malloc(sizeof(Nodo));
    strcpy(ant->dir->keyNodo,dado);
    ant->dir->qtdRepeticoes =1;
    t->qtdOcupando++;
    t->qtdLidas++;
    return nodo;   
}

void printTable(Table* t){
    int num = getTamanho(t);
    for(int i =0;i<num;i++){
        if(strcmp(t->filas[i].keyFila,"")!=0) 
            printf("\n| Fila: %d  \t|   %s    | %d",i,t->filas[i].keyFila,t->filas[i].qtdRepeticiones);
    }
}

Arvore* create_arv(Table* t,String dado){
  Arvore* a = (struct arvore*)malloc(sizeof(Arvore));
  Nodo* n = (struct nodo*)malloc(sizeof(struct nodo));
  n->qtdRepeticoes = 1;
  n->ezq = NULL;
  n->dir = NULL;
  n->pai = NULL;
  n->color = BLACK;
  strcpy(n->keyNodo,dado);
  a->n = n;
  a->root = a->n;
  // se o dado que chega se repete mais que o dado do arvore troca???
  if(strcmp(dado,a->keyRoot)==0){
      a->root->qtdRepeticoes++;
      t->qtdLidas++;
  }else if(a->qtdRepeticoes == 1){

      if(strcmp(dado,a->keyRoot)>0){
          a = switchValueRoot(a,dado);
      }else{
          insert(a,n);
      }
  }
  return a;
}

Arvore* switchValueRoot(Arvore* a,String dado){
    strcpy(a->keyRoot,dado);
    a->qtdRepeticoes =1;
    return a;
}

void insert(Arvore* a, Nodo* n) {
  Nodo* aux = a->n;
  Nodo* mov = a->root;

  while(mov != a->n){
    aux = mov;
    if(strcmp(n->keyNodo,mov->keyNodo)<0)
      mov = mov->ezq;
    else
      mov = mov->dir;
  }
  n->pai = aux;

  if(aux == a->n)
    a->root = n;
  else if(strcmp(n->keyNodo,aux->keyNodo)<0)
    aux->ezq = n;
  else
    aux->dir = n;
  n->dir = a->n;
  n->ezq = a->n;

arruma_arriba(a, n);
}


void arruma_arriba(Arvore* a, Nodo *n) {
  while(n->pai->color == 1){
    if(n->pai == n->pai->pai->ezq){
      Nodo* aux = n->pai->pai->dir;
      if(aux->color == 1){
        n->pai->color = 0;
        aux->color = 0;
        n->pai->pai->color = 1;
        n = n->pai->pai;
      }else{
        if(n == n->pai->dir){
          n = n->pai;
          rota_esq(a,n);
        }
        n->pai->color = 0;
        n->pai->pai->color = 1;
        rota_dir(a,n->pai->pai);
        }
      }else{
        Nodo* aux = n->pai->pai->ezq;
        if(aux->color == 1){
          n->pai->color = 0;
          aux->color = 0;
          n->pai->pai->color = 1;
          n = n->pai->pai;
        }else{
          if(n == n->pai->ezq){
            n = n->pai;
            rota_dir(a,n);
          }
          n->pai->color = 0;
          n->pai->pai->color = 1;
          rota_esq(a,n->pai->pai);
      }
    }
  }
  a->root->color = 0;
}

int buscar (Nodo *a, String v) {
  if (a == NULL) { return 0; } /*Nao achou*/
  else if (v < a->keyNodo) {
    return buscar (a->ezq, v);
  }
  else if (v > a->keyNodo) {
    return buscar (a->dir, v);
  }
  else { return 1; } /*Achou*/
}

void rota_esq(Arvore *a, Nodo *n) {
  Nodo *aux = n->dir;
  n->dir = aux->ezq;
  if(aux->ezq != a->root) {
    aux->ezq->pai = n;
  }
  aux->pai = n->pai;
  if(n->pai == a->root) {
    a->root = aux;
  }
  else if(n == n->pai->ezq) {
    n->pai->ezq = aux;
  }
  else {
    n->pai->dir = aux;
  }
  aux->ezq = n;
  n->pai = aux;
}

void rota_dir(Arvore *a, Nodo *n) {
  Nodo* aux =  n->ezq;
  n->ezq = aux->dir;
  if(aux->dir != a->root){
    aux->dir->pai = n;
  }
  aux->pai = n->pai;
  if(n->pai == a->root){
    a->root = aux;
  }else if(n == n->pai->dir){
    n->pai->dir = aux;
  }else{
    n->pai->ezq = aux;
  }
  aux->dir = n;
  n->pai = aux;
}

Nodo* create_nodo(String val){
  Nodo* n = (Nodo*)malloc(sizeof(Nodo));
  n->ezq = NULL;
  n->dir = NULL;
  n->pai = NULL;
  n->qtdRepeticoes = 1;
  strcpy(n->keyNodo,val);
  n->color = 1;

return n;
}