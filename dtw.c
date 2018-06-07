
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

typedef struct dupla {
    int tipo;
    float dtw;
} DUPLA;

void swap (DUPLA vetor[], int i, int j)
{
    DUPLA aux;
    aux = vetor[i];
    vetor[i] = vetor[j];
    vetor[j] = aux;
}

void heapify(DUPLA arr[], int n, int i)
{
    int smaller = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l].dtw > arr[smaller].dtw)
        smaller = l;
    if (r < n && arr[r].dtw > arr[smaller].dtw)
        smaller = r;
    if (smaller != i)
    {
        swap(arr, i, smaller);
        heapify(arr, n, smaller);
    }
}

void heapSort(DUPLA arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i >= 0; i--)
    {
        swap(arr, 0, i);
        heapify(arr, i, 0);
    }
}


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
        char * scanI, *scanEsp, espaco[3], aux[1500];
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


int define_tipo (DUPLA vet[], int tam)
{
    int knn, aux, i_maior = 0, i_2maior = 0, a;
    int *n_prox = (int *) calloc(12, sizeof(int));
    //ordena o vetor
    heapSort(vet, tam);
    for (a = 0; a < tam; a++)
        //printf("tipo e dtw: %d  %f\n", vet[a].tipo, vet[a].dtw);
    //printf("tamanho do vetor: %d\n", a);
    //pega quantas vezez cada tipo apareceu nos k mais proximos
    knn = 1;
    for(a = 0; a < knn; a++)
    {
        aux = vet[a].tipo - 1;
        n_prox[aux] += 1;
        aux = 0;
    }
    //decide o tipo
    for (int b = 0; b < 12; b++)
    {
        //printf(" %d ", n_prox[b]);

        if(n_prox[b] >= n_prox[i_maior])
        {
            int auxiliar = 0;
            auxiliar = i_2maior;
            i_2maior = i_maior;
            i_maior = b;
        }
    }
    return (i_maior + 1);
}

int get_dtw (LISTA *cabeca_treino, LISTA *cabeca_teste)
{
    int a;
    NO *teste, *treino_atual, *aux1, *aux2;

    teste = cabeca_teste->inicio;
    treino_atual = cabeca_treino->inicio;

    if ((treino_atual == NULL) || (teste == NULL))
    {
        printf("Erro: get_dtw lista vazia\n");
        return (-1);
    }

    while (teste != NULL)
    {
        DUPLA *dtws = (DUPLA *) calloc(cabeca_treino->tam, sizeof(DUPLA));
        treino_atual = cabeca_treino->inicio;
        for (int t = 0; t < cabeca_treino->tam; t++)
        {
            //printf("Chegamos em get_dtw para %d\n", t);
            dtws[t].dtw = dtw_f (treino_atual->item->val, teste->item->val, treino_atual->item->tam, teste->item->tam);
            dtws[t].tipo = treino_atual->item->tipo;
            treino_atual = treino_atual->prox;
            if (treino_atual == NULL)
                break;
        }
        //define o tipo
        teste->item->atrib = define_tipo(dtws, cabeca_treino->tam);
        printf("Tipo real: %d   Tipo atribuido: %d\n", teste->item->tipo, teste->item->atrib);
        if(teste->prox == NULL)
            break;
        free(dtws);
        teste = teste->prox;
    }
    return 1;
}



int main()
{
    FILE *arq_treino = NULL, *arq_teste = NULL;
    char arq[30];
    DADO *teste = NULL;
    //printf("Diga o nome do arquivo de treino: ");
    //gets(arq);
    arq_treino = fopen("treino.txt", "r");
    arq_teste = fopen("teste.txt", "r");
    //printf("Abriu o arq\n");
    //printf("%x\n", arq_treino);
    int a;

    LISTA *cabeca_teste, *cabeca_treino, *cabeca;
    cabeca_treino = criar_lista (arq_treino);
    cabeca_teste = criar_lista (arq_teste);

    cabeca = cabeca_treino;
    if (cabeca != NULL)
    {
        int k = 0;
        NO *agora = NULL;
        agora = cabeca->inicio;
        while (agora!=NULL)
        {
            k++;
            printf("%d %d",agora->item->tipo, agora->item->tam);
            a = 0;
            while (agora->item->val[a])
            {
                //printf("%g ",agora->item->val[a]);
                a++;
            }
            agora = agora->prox;
            printf("\n");
        }
        //printf("Tamanho da lista: %d\n", k);
    }

    cabeca = cabeca_teste;
    if (cabeca != NULL)
    {
        int k = 0;
        NO *agora = NULL;
        agora = cabeca->inicio;
        while (agora!=NULL)
        {
            k++;
            printf("%d %d",agora->item->tipo, agora->item->tam );
            a = 0;
            while (agora->item->val[a])
            {
                //printf("%g ",agora->item->val[a]);
                a++;
            }
            agora = agora->prox;
            printf("\n");
        }
        //printf("Tamanho da lista: %d\n", k);
    }

    get_dtw (cabeca_treino, cabeca_teste);
    printf("\ntamanho das listas: %d  %d\n", cabeca_treino->tam, cabeca_teste->tam);
    cabeca = cabeca_teste;
    if (cabeca != NULL)
    {
        int k = 0, acerto = 0;
        NO *agora = NULL;
        agora = cabeca->inicio;
        while (agora != NULL)
        {
            k++;
            if (agora->item->tipo == agora->item->atrib)
                acerto++;
            a = 0;
            agora = agora->prox;
        }
        printf("Porcentagem de acerto: %f\n\n\n", (float)acerto/k);
    }

    cabeca = cabeca_teste;
    if (cabeca != NULL)
    {
        int k[13], acerto[13];
        for (int c = 0; c <= 13; c++)
        {
            k[c]=0;
            acerto[c]=0;
        }
        NO *agora = NULL;
        agora = cabeca->inicio;
        while (agora != NULL)
        {
            if (agora->item->tipo == agora->item->atrib)
                acerto[agora->item->tipo] += 1;

            k[agora->item->tipo] += 1;
            a = 0;
            agora = agora->prox;
        }
        for (int p = 0; p < 12; p++)
        {
            printf("Porcentagem de acerto em tipo %d: %f\n", p+1,(float)acerto[p + 1]/(float) k[p + 1]);
        }
    }


    fclose (arq_treino);
    fclose(arq_teste);
    //printf("%x\n", arq_treino);

    return 0;
}
