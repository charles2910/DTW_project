
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct particula { //estrutura contendo os dados do arquivo (teste ou treino)
    int tipo; //diz qual o tipo de movimento foi executado
    int tam; //diz o num de dados q temos no vetor
    int atrib; //mostra o tipo atribuido no teste para esse dado dpois do dtw e do k-nn
    float val[200]; //cria vetor estatico de valores do acelerom
} DADO;

typedef struct no { //elemento da lista q contem os dados de treino
    DADO *item; //aponta para o DADO desta posição
    struct no *prox; //aponta para o prox elemento da lista
} NO;

typedef struct lista { //cabeça da lista
    NO *inicio;  //aponta para o primeiro elemento da lista
    int tam;  //guarda o tamanho da lista
} LISTA;



LISTA * criar_cabeca () //cria a cabeça da lista
{
    LISTA *cabeca = (LISTA *) malloc (sizeof(LISTA)); //aloca memoria para cabeça
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


LISTA * criar_lista (FILE *arquivo) //cria lista de treino com dados do arquivo fornecido pelo usuario
{  //OBS: não funciona quando há " 0 " no meio dos dados
    LISTA * cabeca = criar_cabeca();
    NO * anterior = NULL;

    while (!feof(arquivo)) //enquanto nao chegar o fim do arquivo do usuario, continuar adicionando a lista os valores do arquivo.
    {
        int a = 0, pos = 0;
        char * scanI, *scanEsp, espaco[3], end[3], aux[1500];
        float t = 0.0;
        espaco[0] = ' ';
        espaco[1] = ' ';

        fgets(aux, 1500, arquivo); //lê a linha do arquivo q contem os dados
        scanI = aux; //aponta para o inicio da string com os dados
        if (feof(arquivo))//testa se chegamos ao EOF depois da ultimo fgets
            break;


        NO *novo = criar_no(); //cria um novo nó para inserir o dado
        if (cabeca->inicio == NULL)
        {
            cabeca->inicio = novo; //nó anterior da lista passa a apontar para o novo
            cabeca->tam += 1;
        }
        else
            anterior->prox = novo;
        novo->item = criar_dado();  //insere o dado no nó



        for (int b = 0; b < 200; b++)
        {
            scanEsp = strpbrk (scanI, espaco);
            sscanf (scanI, "%f", &t);

            if (b == 0)
            {
                novo->item->tipo = (int) t;
                if ((novo->item->tipo < 1) || (novo->item->tipo >12)) //checa se estamos lendo um dado errado ou corrompido, se sim, printa msg de erro com a linha dos dados
                {
                    printf("Erro na leitura da linha:\n%s\n", aux);
                    free(novo);
                    novo = anterior;
                    break;
                }
            }
            else //atribui o valor do dado ao array
            {
                novo->item->val[b - 1] = t;
                novo->item->tam += 1;
            }
            if (scanEsp == NULL) //testa se chegamos ao fim da linha
                break;
            scanI = scanEsp + 1;
            t = 0.0;
       }
       cabeca->tam += 1;
       anterior = novo;
    }

    return cabeca;  //retorna a cabeça da lista de treino
}


float min (float a, float b, float c) //funçao auxiliar ao dtw
{
    float minimo;
    if (a <= b)
        minimo = a;
    else if (b <= c)
        minimo = b;
    else
        minimo = c;
    if (c <= a)
        minimo = c;
    return minimo;
}


float dtw_f (float *v1, float *v2, int tam1, int tam2)
{
    int a = 0,
        b = 0;
    float matriz_dtw[tam1][tam2];
    for (a = 0; a < tam1; a++)  //monta a matriz com as distancias entre as series
    {
        for (b = 0; b < tam2; b++)
        {
            matriz_dtw[a][b] = (float) fabs((v1[a] - v2[b]));
        }
    }

    for (a = 1; a < tam1; a++)  //inicializa a primeira coluna
        matriz_dtw[a][0] = matriz_dtw[a][0] + matriz_dtw[a - 1][0];
    for (b = 1; b < tam2; b++) //inicializa a primeira linha
        matriz_dtw[0][b] = matriz_dtw[0][b] + matriz_dtw[0][b - 1];

    for (a = 1; a < tam1; a++) //faz o dtw ----> ultima posição é o valor do DTW
    {
        for ( b = 1; b < tam2; b++)
        {
            matriz_dtw[a][b] = matriz_dtw[a][b] + min (matriz_dtw[a - 1][b],matriz_dtw[a][b - 1],matriz_dtw[a - 1][b - 1]);
        }
    }

    return matriz_dtw[tam1 - 1][tam2 - 1]; //retorna o valor do dtw
}








int main()
{
    FILE *arq_treino = NULL;
    char arq[30];
    DADO *teste = NULL;
    //printf("Diga o nome do arquivo de treino: ");
    //gets(arq);
    arq_treino = fopen("treino.txt", "r");
    //printf("Abriu o arq\n");
    //printf("%x\n", arq_treino);
    int a;

    LISTA *cabeca;
    cabeca = criar_lista (arq_treino);

    if (cabeca != NULL)
    {
        int k = 0;
        NO *agora = NULL;
        agora = cabeca->inicio;
        while (agora!=NULL)
        {
            k++;
            printf("%d ",agora->item->tipo/*, agora->item->tam */);
            a = 0;
            while (agora->item->val[a])
            {
                printf("%g ",agora->item->val[a]);
                a++;
            }
            agora = agora->prox;
            printf("\n");
        }
        //printf("Tamanho da lista: %d\n", k);
    }


    printf("\ntamanho da lista: %d\n", cabeca->tam);




    fclose (arq_treino);
    //printf("%x\n", arq_treino);

    return 0;
}
