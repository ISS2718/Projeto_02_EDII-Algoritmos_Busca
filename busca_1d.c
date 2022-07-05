#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int* inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

int buscar_ind(int *indices, int *entradas, int buscado, int n, int s){
    int t = n/s;
    int pos = 0;
    for(int i=0; i<t-1; i++){
        if(buscado >= indices[i] && buscado < indices[i+1]){
            pos = i;
        }
    }
    if(buscado >= indices[t-1]){
        pos = t-1;
    }
    //printf("%d no indices %d entre %d e %d\n", buscado, pos, indices[pos], indices[pos+1]);
    int inicio = pos*s;
    int fim = (pos+1)*s;
    for(int i = inicio; i<fim; i++){
        if(buscado == entradas[i]){
            //printf("%d encontrado em %d\n", buscado, i);
            return i;
        }
    }
    //printf("%d nao encontrado\n", buscado);
    return -1;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int S  = 10000; //numero de inteiros por indice
    const int T = N/S;
    const int index_size = 10000;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("dados/inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("dados/inteiros_busca.txt", N);
    
    // ordenar entrada
    for(int i = 0; i<N; i++){
        int menor = entradas[i];
        int j, pos;
        for(j = i; j<N; j++){
            if(entradas[j]< menor){
                menor = entradas[j];
                pos = j;
                if(menor == 49874){
                    printf("%d %d\n", i, menor);
                }
                if(i == 40726){
                    printf("%d troca com %d - menor: %d - i: %d\n",  entradas[i],  entradas[pos], menor, i);
                }
            }   
        }
        
        entradas[pos] = entradas[i];
        entradas[i] = menor;
        int flag = 0;
        for(int j = 0; j<N; j++){
            if(entradas[j] == 49874){
                flag = 1;
            }
        }
        if(i == N-1 && flag)
            printf("achou");
    }
    for(int j = 0; j<N; j++){
            if(entradas[j] == 49874){
                printf("encontrou o num");
            }
    }
    // criar tabela de indice
    FILE * entrada_ord = fopen("dados/entradas_ord.txt","w");
    FILE * encontrados_ind = fopen("dados/encontrados_ind.txt","w");
    for(int i = 0; i<N; i++){
        fprintf(entrada_ord, "%d\n", entradas[i]);
    }
    
    int * indices = calloc(T, sizeof(int));
    for(int i=0; i<T; i++){
        indices[i] = entradas[S*i];
    }
    // realizar consultas na tabela de indices 
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        int res = buscar_ind(indices, entradas, consultas[i], N, S);
        if(res != -1){
            encontrados++;
            fprintf(encontrados_ind, "%d\n", consultas[i]);
        }
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
