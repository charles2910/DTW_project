
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct particula { //estrutura contendo os dados do arquivo (teste ou treino)
    int tipo; //diz qual o tipo de movimento foi executado
    float val[40]; //cria vetor estatico de valores do acelerom
} DADO;

typedef struct no { //elemento da lista q contem os dados de treino
    DADO *item; //aponta para o DADO desta posição
    struct no *prox; //aponta para o prox elemento da lista
} NO;

typedef struct lista_treino { //cabeça da lista
    NO *inicio;  //aponta para o primeiro elemento da lista
    int tam;  //guarda o tamanho da lista
} LISTA_TREINO;



LISTA_TREINO * criar_cabeca () //cria a cabeça da lista
{
    LISTA_TREINO *cabeca = (LISTA_TREINO *) malloc (sizeof(LISTA_TREINO)); //aloca memoria para cabeça
    if (cabeca != NULL)
    {
        cabeca->inicio = NULL; //aterra a cabeça
        cabeca->tam = -1; //indica lista vazia
        return cabeca;
    }
    else{
        printf("ERRO na criação da cabeça da lista\n");
        exit(-1);
    }
}

NO * criar_no () //cria um nó vazio
{
    NO *aux = (NO *) malloc (sizeof(NO)); //aloca memoria para o nó
    if (aux != NULL) //checa se o malloc funcionou
    {
        aux->item = NULL;
        aux->prox = NULL; //aterra ponteiros do nó inicializado
        return aux;
    }
    else
    {
        printf("ERRO na criação do nó\n");
        exit(-1);
    }
}

DADO * criar_dado ()
{
    DADO *aux = (DADO *) malloc (sizeof(DADO)); //aloca memoria para o dado
    if (aux != NULL) //checa se o malloc funcionou
        return aux; //retorna ponteiro para o dado
    else
    {
        printf("ERRO na criação do dado\n");
        exit(-1);
    }
}

LISTA_TREINO * criar_treino (FILE *arquivo) //cria lista de treino com dados do arquivo fornecido pelo usuario
{
    LISTA_TREINO * cabeca = criar_cabeca();
    NO * anterior = NULL;
    //anterior = cabeca; //anterior aponta para inicio da lista

    while (!feof(arquivo)) //enquanto nao chegar o fim do arquivo do usuario, continuar adicionando a lista os valores de treino.
    {
        NO *novo = criar_no(); //cria um novo nó para inserir o dado
        if (cabeca->inicio == NULL)
            cabeca->inicio = novo; //nó anterior da lista passa a apontar para o novo
        else
            anterior->prox = novo;
        novo->item = criar_dado();  //insere o dado no nó
        int a = 0, pos = 0;
        char * scanIni, *scanEsp, espaco[3];
        espaco[0] = ' ';
        espaco[1] = ' ';

/*
        while (!feof(arquivo))
        {
            char aux[500];

            fgets(aux, 490, arquivo);
            str = aux;

            scanIni = strpbrk (str, espaco);
            printf("caracter encontrado: %c", ScanIni[0]);

        }

*/
        /*
        do {
            char val[10], conv[10];
            do {
                a = 0;
                val[a] = fgetc(arquivo);
                if (a > 0)
                    conv[a - 1] = val[a - 1];
            }
            while((val[a] != ' ') && (!feof(arquivo)) && (val[a] != '\n'));
            novo->item->val[pos] = atof (conv);
            pos++;
            if (val[a] == '\n')
                break;
        }
        while(1);

        fread (novo->item->tipo, sizeof(int), 1, arquivo)

        fread*/



        anterior = novo;
    }

    return cabeca;  //retorna a cabeça da lista de treino
}







int main()
{
    FILE *arq_teste = NULL;
    char arq[30];
    DADO *teste = NULL;
    printf("Diga o nome do arquivo de treino: ");
    gets(arq);
    arq_teste = fopen("treino.txt", "r");
    printf("Abriu o arq\n");
    printf("%x\n", arq_teste);
    teste = criar_dado();  //insere o dado no nó
    printf ("endereco do dado: %x\n", teste);
    int a = 0, pos = 0;
    char *scanI;
    char *scanEsp;
    char *scanEnd;
    char espaco[3], end[3];
    espaco[0] = ' ';
    espaco[1] = ' ';

    end[0] = '\n';
    end[1] = '\n';



    printf ("tipo do dado: %d  Val[0]: %f", teste->tipo, teste->val[0]);
    char aux[500];



    while(!feof(arq_teste))
    {
        fgets(aux, 520, arq_teste);
        float t = 0.0;a
        //printf ("string:\n%s\n\n", aux);
        scanI = aux;
        scanEnd = strpbrk (aux, end);

	    for (int b = 0; b < 30; b++)
        {
	        scanEsp = strpbrk (scanI, espaco);
            if (scanEsp == NULL)
                break;
            if ((scanI == scanEnd) || (scanI == scanEnd - 1))
                break;
	        sscanf (scanI, "%f", &t);
	        scanI = scanEsp + 1;
            printf("val[%d]: %f\n", b, t);
            if (b == 0)
                novo->item->tipo = (int) t;
            else
                novo->item->val[b] = t;
	    }

    }





    fclose (arq_teste);
    printf("%x\n", arq_teste);
    //lista_treino_1 = criar_treino(teste);
    //printf("criar treino retornou algo\n");
    //printf("Primeiro valor do arquivo: %lf\n", lista_treino_1->inicio->item->val[0]);
    return 0;
}