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
    int inicio = pos*s;
    int fim = (pos+1)*s;
    for(int i = inicio; i<fim; i++){
        if(buscado == entradas[i]){
            return i;
        }
    }
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
            if(entradas[j]<= menor){
                menor = entradas[j];
                pos = j;
            }   
        }
        entradas[pos] = entradas[i];
        entradas[i] = menor;
    }
    // criar tabela de indice
    
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
        }
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    free(entradas);
    free(consultas);
    free(indices);
  
    return 0;
}
