#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "indexer.h"

void imprimeHead()
{
  printf("\n==========================================================");
  printf("\n================BENVINDO AO SISTEMA INDEXER===============\n");
}

void showMenu()
{
  printf("\n\t= Digite o numero correspondete a uma das Opcoes:\n");
  printf("\n\t| 1) - N palavras mais Frequentes\n");
  printf("\n\t| 2) - Frequenca da palavra\n");
  printf("\n\t| 3) - Procurar palavra em arquivos\n");
  printf("\n\t| 4) - Sair\n");
  printf("\n\t\t\t| R :");
}

ARQ getFile(char *nome)
{
  ARQ file = fopen(nome, "r");
  if (!file)
    return NULL;
  else
    return file;
}

Indexer carregaTabela(ARQ file)
{
  String palavra;
  Table* t;
  t = initTable(11);
  int count = 0;
  while (feof(file) == 0)
  {
    fscanf(file, "%s", palavra);
    char *word;
    // strcpy(word,getStringClean(palavra));
    word = getStringClean(palavra);
    strcpy(palavra,word);
    if (strcmp(palavra, "") == 0)
    continue;
    else{
      t = insereFila(t, palavra);
      count++;
      if(count>500) break;
    }
    
  }
  fclose(file);
  return t;
}

Indexer carregaTabelaFreqN(ARQ arquivo, int num)
{
  String palavra;
  int count = 0;
  Indexer indexer = initTable(11);
  Fila filas[num];
  indexer->taux = filas;
  while (feof(arquivo) == 0)
  {
    fscanf(arquivo, "%s", palavra);
    if (strcmp(getStringClean(palavra), "") == 0)
      continue;
    indexer = insereFila(indexer, palavra);
    count++;
  }
  fclose(arquivo);
  return indexer;
}

Indexer carregaTabelaFeqP(ARQ arquivo, String palavra)
{
  printf("Palavra a perquisar: %s", palavra);
  int count = 0;
  Indexer indexer;
  indexer = initTable(11);
  while (feof(arquivo) == 0)
  {
    fscanf(arquivo, "%s", palavra);
    if (strcmp(getStringClean(palavra), "") == 0)
      continue;
    printf("%s", palavra);
    indexer = insereFila(indexer, palavra);
    count++;
  }
  fclose(arquivo);
  return indexer;
}

Indexer carregaTabelaSearch(ARQ Arquivos[], String palavra, int tam)
{
  printf("Palavra a perquisar: %s", palavra);
  Indexer indexer;
  for (int j = 0; j < tam; j++)
  {
    ARQ arquivo = Arquivos[j];
    int count = 0;
    indexer = initTable(11);
    while (feof(arquivo) == 0)
    {
      fscanf(arquivo, "%s", palavra);
      if (strcmp(getStringClean(palavra), "") == 0)
        continue;
      indexer = insereFila(indexer, palavra);
      count++;
    }
    fclose(arquivo);
  }
  return indexer;
}

char *getStringClean(String dado)
{
  char c;
  if (strlen(dado) < 3)
    return "";
  int tam = strlen(dado);
  for (int i = 0; i < tam; i++)
  {
    c = dado[i];
    if (c < 65 || (c > 90 && c < 97) || c > 122)
    {
      dado = "";
      return dado;
    }
    if (c > 54 && c < 91)
    {
      c = tolower(c);
      dado[i] = c;
    }
  }
  if (strcmp(dado, "the") == 0)
    strcpy(dado, "");
  if (strcmp(dado, "for") == 0)
    strcpy(dado, "");
  if (strcmp(dado, "but") == 0)
    strcpy(dado, "");
  if (strcmp(dado, "all") == 0)
    strcpy(dado, "");
  if (strcmp(dado, "are") == 0)
    strcpy(dado, "");
  if (strcmp(dado, "and") == 0)
    strcpy(dado, "");
  if (strcmp(dado, "was") == 0)
    strcpy(dado, "");
  if (strcmp(dado, "this") == 0)
    strcpy(dado, "");
  return dado;
}

Table *initTable(int tam)
{
  Table *t = (Table *)malloc(sizeof(Table));
  if (t == NULL)
    return NULL;
  t->qtdLidas = 0;
  t->qtdOcupando = 0;
  t->taux = NULL;
  t->filas = (Fila*)calloc(sizeof(Fila),tam);
  for (int i = 0; i < tam; i++)
  {
    t->filas[i].arv = NULL;
    strcpy(t->filas[i].keyFila, "");
    t->filas[i].qtdRepeticiones = 0;
  }
  TAM ta = getSize(tam);
  t->tamanho = ta;
  return t;
}

void freeTable(Table *t)
{
  if (t != NULL)
  {
    int tam = getTamanho(t);
    for (int i = 0; i < tam; i++)
    {
      if ((t->filas[i].arv) != NULL)
      {
        free(t->filas[i].arv);
      }
    }
    free(t->filas);
    free(t);
    printf("tabela liberada com sucesso!\n");
  }
}

Table *insereFila(Table *t, String dado)
{
  int pos = hash(t, dado);
  int pos2 = hash_Alex(t, dado);
  int tam = getTamanho(t);
  int res = (pos * pos2) % tam;
  if (t == NULL)
    return NULL;
  // verifica se nao tem nada na fila
  if (strcmp(t->filas[pos].keyFila, "") == 0)
  {
    insereInNull(t, dado, pos);
    return t;
  }
  else if (strcmp(t->filas[pos].keyFila, dado) == 0)
  {
    t->filas[pos].qtdRepeticiones++;
    return t;
  }
  // procura pela segunda posicao posivel
  else if (strcmp(t->filas[pos2].keyFila, "") == 0)
  {
    insereInNull(t, dado, pos2);
    return t;
    // procura pela ultima posicao posivel
  }
  else if (strcmp(t->filas[(pos * pos2 * res) % tam].keyFila, "") == 0)
  {
    insereInNull(t, dado, (pos * pos2 * res) % tam);
    return t;
  }
  else
  {
    t = insere_arv(t, t->filas[pos].arv->root, dado);
    int tamanho = getTamanho(t);
     int result = altura_arvore(t->filas[pos].arv->root);
    printf("o resultado %d \n",result);
    if (result > (tamanho/2))
    {
      printf("entro en el aumento");
      t = aumentaTamnaho(t); 
      printf("paso el aumento");
    }
  }

  //   raiz = t->filas[pos].arv->root;
  return t;
}

int hash(Table *t, String dado)
{
  int len = strlen(dado);
  unsigned int h = SEED;
  int i;
  // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
  h ^= 2166136261UL;
  String data;
  strcpy(data, dado);
  for (i = 0; i < len; i++)
  {
    h ^= data[i];
    h *= 16777619;
  }
  i = getTamanho(t);
  return (h % i);
}

int hash_Alex(Table *t, String dado)
{
  char c;
  int p = 31;
  int tamanho = getTamanho(t);
  int hash_value = 0, p_pow = 1;

  while ((c = *dado++))
  {
    hash_value = (hash_value + (c - 'a' + 1) * p_pow) % tamanho;
    p_pow = (p_pow * p) % tamanho;
  }
  return (hash_value);
}

void insereInNull(Table *t, String entrando, int pos)
{

  (t->filas[pos].arv) = (struct arvore *)malloc(sizeof(struct arvore));
  strcpy(t->filas[pos].keyFila, entrando);
  t->filas[pos].arv->root = NULL;
  // strcpy(t->filas[pos].arv->keyRoot,entrando);
  t->filas[pos].arv->profundidade = 0;
  t->filas[pos].qtdRepeticiones = 1;
  t->qtdLidas++;
  t->qtdOcupando++;
}

Table *aumentaTamnaho(Table *t)
{
  int tamIni, tamFinal;
  tamIni = getTamanho(t);
  if(t->tamanho < INOMINUS)
  t->tamanho = (t->tamanho + 1);
  else t->tamanho = INOMINUS;
  tamFinal = getTamanho(t);
  if(tamIni == tamFinal) return t;
  Table *newTable = initTable(tamFinal);
  int pos,pos2,res,pos3;
  for (int i = 0; i < tamIni; i++)
  {
    // se o arv nao es null
    insereFila(newTable, t->filas[i].keyFila);
    pos = hash(newTable,t->filas[i].keyFila);
    pos2 = hash_Alex(newTable,t->filas[i].keyFila);
    res = (pos*pos2)%tamFinal;
    pos3 = (pos*pos2*res)%tamFinal;
    if(strcmp(newTable->filas[pos].keyFila,t->filas[i].keyFila)==0)
      newTable->filas[pos].qtdRepeticiones = t->filas[i].qtdRepeticiones;
    else if (strcmp(newTable->filas[pos2].keyFila,t->filas[i].keyFila)==0)
      newTable->filas[pos2].qtdRepeticiones = t->filas[i].qtdRepeticiones;   
    else if (strcmp(newTable->filas[pos3].keyFila,t->filas[i].keyFila)==0)
      newTable->filas[pos3].qtdRepeticiones = t->filas[i].qtdRepeticiones;   
    else{
        Nodo* nodo = getNodo(newTable->filas[pos].arv->root,t->filas[i].keyFila);
        nodo->qtdRepeticoes = t->filas[i].qtdRepeticiones;
    }
    if (t->filas[i].arv->root != NULL)
    {
      free_arv(newTable, t->filas[i].arv->root);
    }
    
    free(t->filas[i].arv);
  }
  free(t->filas);
  free(t);
  return (newTable);
}

int getTamanho(Table *t)
{

  int i;
  if (t->tamanho == XXXSMALL)
    i = 11;
  else  if (t->tamanho == XXSMALL)
    i = 53;
  else if (t->tamanho == XSMALL)
    i = 97;
  else if (t->tamanho == SMALL)
    i = 1033;
  else if (t->tamanho == MEDIUM)
    i = 10093;
  else if (t->tamanho == LARGE)
    i = 50053;
  else if (t->tamanho == XLARGE)
    i = 100043;
  else if (t->tamanho == XXLARGE)
    i = 10000019;
  else if (t->tamanho == XXXLARGE)
    i = 50000047;
  else if (t->tamanho == INOMINUS)
    i = 200000083;

  return i;
}

TAM getSize(int num)
{
  TAM t;
  if (num == 11)
    t = XXXSMALL;
  else if (num == 53)
    t = XXSMALL;
  else if (num == 97)
    t = XSMALL;
  else if (num == 1033)
    t = SMALL;
  else if (num == 10093)
    t = MEDIUM;
  else if (num == 50053)
    t = LARGE;
  else if (num == 100043)
    t = XLARGE;
  else if (num == 10000019)
    t = XXLARGE;
  else if (num == 50000047)
    t = XXXLARGE;
  else
    t = INOMINUS;
  return t;
}

//// reimplementacao do arvore red black

void trocar_cor(Nodo *c)
{
  c->color = !c->color;
  if (c->ezq != NULL)
    c->ezq->color = !c->ezq->color;
  if (c->dir != NULL)
    c->dir->color = !c->dir->color;
}

int cor(Nodo *c)
{
  if (c->color == 0)
    return BLACK;
  else
    return c->color;
}

Nodo *rotacaoEzq(Nodo *nodo)
{
  Nodo *aux = nodo->dir;
  nodo->dir = aux->ezq;
  aux->ezq = nodo;
  aux->color = nodo->color;
  nodo->color = RED;
  return aux;
}

Nodo *rotacaoDir(Nodo *nodo)
{
  Nodo *aux = nodo->ezq;
  nodo->ezq = aux->dir;
  aux->dir = nodo;
  aux->color = nodo->color;
  nodo->color = RED;
  return aux;
}

Nodo *move2EzqR(Nodo *nodo)
{
  trocar_cor(nodo);
  if (cor(nodo->dir->ezq) == RED)
  {
    nodo->dir = rotacaoDir(nodo->dir);
    nodo = rotacaoEzq(nodo);
    trocar_cor(nodo);
  }
  return nodo;
}

Nodo *move2DirR(Nodo *nodo)
{
  trocar_cor(nodo);
  if (cor(nodo->ezq->ezq) == RED)
  {
    nodo = rotacaoDir(nodo);
    trocar_cor(nodo);
  }
  return nodo;
}

Nodo *balancear(Nodo *nodo)
{

  if (cor(nodo->dir) == RED)
    nodo = rotacaoEzq(nodo);
  if (nodo->ezq != NULL && cor(nodo->dir) == RED && cor(nodo->ezq->ezq) == RED)
    nodo = rotacaoDir(nodo);
  if (cor(nodo->ezq) == RED && cor(nodo->dir) == RED)
    trocar_cor(nodo);
  return nodo;
}

Table *insere_arv(Table *t, struct nodo *raiz, String valor)
{
  raiz = insereNO(t, raiz, valor);
  if ((raiz) != NULL)
    (raiz)->color = BLACK;
  int pos = hash(t, valor);
  t->filas[pos].arv->root = raiz;
  return t;
}

Nodo *insereNO(Table *t, Nodo *nodo, String valor)
{

  if (nodo == NULL)
  {
    struct nodo *novo;
    novo = (struct nodo *)malloc(sizeof(struct nodo));
    if (novo == NULL)
      return NULL;
    strcpy(novo->keyNodo, valor);
    novo->color = RED;
    novo->dir = NULL;
    novo->ezq = NULL;
    novo->qtdRepeticoes = 1;
    t->qtdLidas++;
    t->qtdOcupando++;
    return novo;
  }
  if (strcmp(valor, nodo->keyNodo) == 0)
  {
    t->qtdLidas++;
    nodo->qtdRepeticoes++;
  }
  else if (strcmp(valor, nodo->keyNodo) < 0)
  {
    nodo->ezq = insereNO(t, nodo->ezq, valor);
  }

  else if (strcmp(valor, nodo->keyNodo) > 0)
  {
    nodo->dir = insereNO(t, nodo->dir, valor);
  }

  if (nodo->ezq != NULL && nodo->dir != NULL)
  {

    if (cor(nodo->dir) == RED && cor(nodo->ezq) == BLACK)
    {
      nodo = rotacaoEzq(nodo);
    }

    if (cor(nodo->ezq) == RED && cor(nodo->dir) == RED)
    {
      trocar_cor(nodo);
    }
  }

  if (nodo->ezq != NULL && nodo->ezq->ezq != NULL)
  {
    if (cor(nodo->ezq) == RED && cor(nodo->ezq->ezq) == RED)
    {
      nodo = rotacaoDir(nodo);
    }
  }
  int pos = hash(t,valor);
  t->filas[pos].arv->root = nodo;
  return nodo;
}

void printArv(Nodo *nodo)
{
  if (!nodo)
    return;
  printArv(nodo->ezq);
  printf(" valor %s  repeticoes %d \n", nodo->keyNodo,nodo->qtdRepeticoes);
  printArv(nodo->dir);
}

int altura_arvore(struct nodo *a)
{
  if (a == NULL)
    return 0;
  int esq = altura_arvore(a->ezq);
  int dir = altura_arvore(a->dir);
  if (esq > dir)
    return (esq + 1);
  else
    return (dir + 1);
}

void printTable(Table *t)
{
  int ta = getTamanho(t);
  for (int m = 0; m < ta; m++)
  {
    printf("\nPosicao %d \t|Dado %s    | Repticoes %d\n", m, t->filas[m].keyFila, t->filas[m].qtdRepeticiones);
    if(t->filas[m].arv != NULL)
    printArv(t->filas[m].arv->root);
  }
}
void free_arv(Table *newTable, struct nodo *nodo)
{
  if (nodo == NULL)
    return;
  int pos,pos2,pos3,res;
  int tam = getTamanho(newTable);
  free_arv(newTable, nodo->ezq);
  free_arv(newTable, nodo->dir);
  insereFila(newTable, nodo->keyNodo);
  if(nodo->qtdRepeticoes>1){
    pos = hash(newTable,nodo->keyNodo);
    pos2 = hash_Alex(newTable,nodo->keyNodo);
    res = (pos * pos2) % tam;
    pos3 = (pos*pos2*res)%tam;
    if(strcmp(newTable->filas[pos].keyFila,nodo->keyNodo)==0){
      newTable->filas[pos].qtdRepeticiones = nodo->qtdRepeticoes;
    }else if(strcmp(newTable->filas[pos2].keyFila,nodo->keyNodo)==0){
      newTable->filas[pos2].qtdRepeticiones = nodo->qtdRepeticoes;
    }else if(strcmp(newTable->filas[pos3].keyFila,nodo->keyNodo)==0){
      newTable->filas[pos3].qtdRepeticiones = nodo->qtdRepeticoes;
    }else{
      Nodo* nod = getNodo(newTable->filas[pos].arv->root,nodo->keyNodo);
      nod->qtdRepeticoes = nodo->qtdRepeticoes;
    } 
  }
  free(nodo);
}

Nodo* getNodo(Nodo* nodo,String dado){
    if(nodo == NULL) return NULL;
    if(strcmp(nodo->keyNodo,dado)==0) return nodo;
    else if(strcmp(nodo->keyNodo,dado)>0) getNodo(nodo->ezq,dado);
    else if(strcmp(nodo->keyNodo,dado)<0) getNodo(nodo->dir,dado);
    return NULL;
}