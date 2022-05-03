#include "indexer.c"

int main(int argc, char** argv) {
   
   Indexer indexer;
   String arquivo,palavra;
   boolean next = true;
   int numArg;
   int resMenu;
   String opc ="";
   ARQ file = NULL;
   ARQ Arquivos[10];
   boolean sair;
   if(argc == 1) numArg =-1;
   else if(argc == 2) numArg =0;
   else if (argc < 5){
      strcpy(opc,argv[1]);
      if(strcmp(opc,"--freq") == 0) numArg = 1;
      else if(strcmp(opc,"--freq-word") == 0) numArg = 2;
      else numArg = -1;
   }else if (argc == 5){
       if((strcmp(opc,"--search") == 0)) numArg = 3;
       else numArg = -1;
   }else numArg = -1;
    
    imprimeHead();
   switch(numArg){
      case -1:
        while(next){
            printf("\nDigite a rota do arquivo junto com o nome Ex-> C:\\exemplo.txt seguido da tecla Enter \n Para Sair digite \"s\" e Enter\n");
            scanf("%s",arquivo);    
            if(strcmp(arquivo,"s") == 0) next = false;
            if(!next) break;
            file = getFile(arquivo);
            if(file != NULL){
                indexer = carregaTabela(file);
                sair = true;
            }   
         }
      break;
      case 0:
        strcpy(arquivo,argv[1]);
        next = true;
        while(next){
            file = getFile(arquivo);
            if(file != NULL){
                indexer = carregaTabela(file);
                next = false;
            }
            if(next){
                printf("| ERRO - Arquivo nulo ->\"%s\"<- ",arquivo);
                printf("\nDigite a rota do arquivo junto com o nome Ex-> C:\\exemplo.txt seguido da tecla Enter \n Para Sair digite \"s\" e Enter\n");
                scanf("%s",arquivo);    
            }                    
            if(strcmp(arquivo,"s") == 0 || getchar() == EOF) next = false;
            }
      break;
      case 1:

        strcpy(arquivo,argv[3]);
        int num = 0;
        num = atoi(argv[2]);
        file = getFile(arquivo);
         if(file ==NULL){
            printf("| ERRO - Arquivo nulo ->\"%s\"<- ",arquivo);
            printf("| Formato requerido .../>\"rota_e_nome_do_aquivo.txt\" \n");
            return 0;
         }
         indexer = carregaTabelaFreqN(file,num);
      break;
      case 2:
        strcpy(palavra,argv[2]);
        strcpy(arquivo,argv[3]);
        file = getFile(arquivo);
         if(file ==NULL){
            printf("| ERRO - Arquivo nulo ->\"%s\"<- ",arquivo);
            printf("| Formato requerido .../>\"rota_e_nome_do_aquivo.txt\" \n");
            return 0;
         }
         indexer = carregaTabelaFeqP(file,palavra);
      break;
      case 3: 
        strcpy(palavra,argv[2]);
        strcpy(arquivo,argv[3]);
        String arquivos;
        strcpy(arquivos,argv[4]);
        
        // verifica numero de arquivos na pasta

        //carrega um por um e armacena o resultado
        
        file = getFile(arquivo);
         if(file ==NULL){
            printf("| ERRO - Arquivo nulo ->\"%s\"<- ",arquivo);
            printf("| Formato requerido .../>\"rota_e_nome_do_aquivo.txt\" \n");
            return 0;
         }
         int tam = 1;
         Arquivos[0] = file;
         indexer = carregaTabelaSearch(Arquivos,palavra,tam);

      break;
      default:
      printf("Mensagem de Erro");
      break;
   }
    sair = false;
   while(!sair){
       showMenu();
        scanf("%d",&resMenu);

        switch(resMenu){
            case 1:
                printf("\n Digite o numero de palavras :");
                int qtd;
                scanf("%d",&qtd);
                printf("\n Digite o nome do arquivo, se estiver fora da pasta digite a rota ex:D:\\nome-arquivo.txt : \n");
                scanf("%s",arquivo);
                file = getFile(arquivo);
                if(file ==NULL){
                    printf("| ERRO - Arquivo nulo ->\"%s\"<- ",arquivo);
                    printf("| Formato requerido .../>\"rota_e_nome_do_aquivo.txt\" \n");
                    break;
                }
                indexer = carregaTabelaFreqN(file,qtd);
                sair = true;
            break;

            case 2:
                printf("\n Digite a palavra para perquisar a frequencia:");
                scanf("%s",palavra);
                printf("\n Digite o nome do arquivo, se estiver fora da pasta digite a rota ex:D:\\nome-arquivo.txt : \n");
                scanf("%s",arquivo);
                file = getFile(arquivo);
                if(file ==NULL){
                    printf("| ERRO - Arquivo nulo ->\"%s\"<- ",arquivo);
                    printf("| Formato requerido .../>\"rota_e_nome_do_aquivo.txt\" \n");
                    break;
                }
                indexer = carregaTabelaFeqP(file,palavra);
                sair = true;
            break;
            case 3:
                printf("\n Digite a palavra para perquisar a frequencia:");
                scanf("%s",palavra);
                int numArray=1;
                printf("\n Digite o nome do arquivo, se estiver fora da pasta digite a rota ex:D:\\nome-arquivo.txt : \n");
                scanf("%s",arquivo);
                ARQ file1;
                file1 = getFile(arquivo);
                Arquivos[0] = file1;
                indexer = carregaTabelaSearch(Arquivos,palavra,numArray);
                sair = true;
            break;
            case 4:
                printf("Saindo do Sistema... \n");    
                sair = true;
            break;
            default:
                printf("Opcao nao valida !!! \n");    
                sair = true;
            break;

        }// end do switch 

   }// end do while(!sair)
   
   

   return (EXIT_SUCCESS);
}
