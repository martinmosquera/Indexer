#define EXIT_SUCCESS 0
#define TAM_TABLE 97
#define TAM_STRING 60
#define SEED 0x12345678

#define RED 1
#define BLACK 0

typedef enum Logico {false, true}boolean;
typedef enum tam_table {XSMALL,SMALL,MEDIUM,LARGE,XLARGE}TAM;
typedef char String[60];
typedef FILE* ARQ;

typedef struct fila{
    struct arvore* arv;
    String keyFila;
    int qtdRepeticiones;
}Fila;

typedef struct tabela{
    Fila filas[TAM_TABLE];
    int qtdLidas;
    int qtdOcupando;
    TAM tamanho;
    Fila* taux;
}Table;

typedef Table* Indexer;

typedef struct nodo{
    String keyNodo;
    struct nodo* dir;
    struct nodo* ezq;
    int qtdRepeticoes;
    int color;
}Nodo;

typedef struct arvore{
    String keyRoot;
    int profundidade;
    struct nodo* root;
    int maxRep;
    struct nodo* n;
}Arvore;

typedef struct nodo* Root;

typedef struct taux{
    String valor;
    Fila* taux;
    int qtd;
}Taux;

ARQ getFile(char* nome);

void imprimeHead();
void showMenu();

Indexer carregaTabela(ARQ file);

Indexer carregaTabelaFreqN(ARQ arquivo,int num);

Indexer carregaTabelaFeqP(ARQ arquivo,String palavra);

Indexer carregaTabelaSearch(ARQ Arquivos[],String palavra,int tam);

char* getStringClean(String dado);

Table* initTable();

void freeTable(Table* t);

Table* insereFila(Table* t, String dado);

int hash(Table *t,String dado);

void freeTable(Table* t);

int hash(Table *t,String dado);

void insereInNull(Table *t,String entrando,int oldPos);

int getTamanho(Table* t);

Table* aumentaTamnaho(Table* t,String dado);

TAM getSize(int num);

void printTable(Table* t);

int hash_Alex(Table *t,String dado);

void trocar_cor(Nodo* c);

Nodo* rotacaoEzq(Nodo* nodo);

Nodo* rotacaoDir(Nodo* nodo);

Nodo* move2EzqR(Nodo* nodo);

Nodo* move2DirR(Nodo* nodo);

Nodo* balancear(Nodo* nodo);

Table* insere_arv(Table* t,struct nodo* raiz,String valor);

Nodo* insereNO(Table* t,Nodo* nodo,String valor);

void printArv(Nodo* nodo);

int cor(Nodo* c);



