/*
Joao Pedro Goncalves Ferreira 12731314
Nicholas Estevao Pereira de Oliveira Rodrigues Braganca NUSP 12689616
Isaac Santos Soares 12751713
*/

//Implemente um algoritmo que realize uma busca sequencial simples
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

int* ler_inteiros(const char * arquivo, const int n){
    FILE *f = fopen(arquivo, "r");

    int *inteiros = (int*)malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++){
        fscanf(f, "%d\n", &inteiros[i]);
    }
    fclose(f);

    return inteiros;
}

void inicia_tempo(){
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo(){
    _fim = clock();
    return ((double)(_fim - _ini)) / CLOCKS_PER_SEC;
}

int busca_simples(int **entrada, int buscado, int N){ //ter ponteiro para ponteiro como parametro deixa o algoritmo mais rapido
    for (int j = 0; j < N; j++){
        if(buscado == (*entrada)[j]){
            return 1;
        }
    }
    return 0;
}

int main(int argc, char const *argv[]){
    const int N = 50000;
    unsigned encontrados = 0;

    int *entradas = ler_inteiros("dados/inteiros_entrada.txt", N);
    int *consultas = ler_inteiros("dados/inteiros_busca.txt", N);

    // realiza busca sequencial
    inicia_tempo();

    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        encontrados = encontrados + busca_simples(&entradas, consultas[i], N);
    }

    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}