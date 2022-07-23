//João Pedro Gonçalves Ferreira - Nº USP: 12731314
//Isaac Santos Soares - Nº USP: 12751713
//Nicholas Estevão Pereira de Oliveira Rodrigues Bragança - Nº USP: 1268961

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

    int * inteiros = (int *) malloc(sizeof(int) * n);

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

int busca_seq_trans (int ** entrada, int buscado, int n){
    for(int i=0; i<n; i++){
        if(buscado == (*entrada)[i]){
            if(i != 0){
                (*entrada)[i] = (*entrada)[i-1];
                (*entrada)[i-1] = buscado;
            }
            return 1;
        }
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("dados/inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("dados/inteiros_busca.txt", N);
    // realiza busca sequencia com realocação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        encontrados += busca_seq_trans(&entradas, consultas[i], N);
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    free(entradas);
    free(consultas);
  
    return 0;
}
