#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // funções strcmp e strcpy
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

// Definição do tipo string
typedef char * string;

#define MAX_STRING_LEN 20


unsigned converter(string s) {
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++) 
      h = h * 256 + s[i];
   return h;
}

string* ler_strings(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");
    
    string* strings = (string *) malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
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

unsigned h_div(unsigned x, unsigned i, unsigned B)
{
    return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i, unsigned B)
{
    const double A = 0.6180;
    return  ((int) ((fmod(x * A, 1) * B) + i)) % B;
}

void criar(string ** t, int B){
    *t = malloc(B*sizeof(string));
    for(int i = 0; i<B; i++){
        (*t)[i] = malloc(25*sizeof(char));
        strcpy((*t)[i], "-1");
    }
}

int inserir_div(string ** t, int B, string k){
    int colisoes = 0;
    for(int i=0; i<B; i++){
        int x = converter(k);
        int pos = h_div(x, i, B);
        if(strcmp((*t)[pos], "-1") == 0){// posicao nunca ocupada
            strcpy((*t)[pos], k);
            //printf("Armazenou %s\n", (*t)[pos]);
            return colisoes;
        }
        if(strcmp((*t)[pos], k) == 0 ){
            //printf("Palavra %s ja cadastrada\n", (*t)[pos]);
            return colisoes;
        }
        //Se chegou aqui a posicao nao estava vazia e palavra nao era repetida => houve colisao
        colisoes++;
    }
    return colisoes;
}

int buscar_div(string * t, int B, string k){
    for(int i=0; i<B; i++){
        int x = converter(k);
        int pos = h_div(x, i, B);
        if(strcmp(t[pos], k) == 0 ){
            //printf("Palavra %s encontrada na posicao %d\n", t[pos], pos);
            return 0;
        }
        if(strcmp(t[pos], "-1") == 0){//sao iguais -> posicao nunca ocupada ou palavra repetida
            //printf("Palavra %s nao existe na lista\n", k);
            return -1;
        }
    }
    return -1;
}

int inserir_mul(string ** t, int B, string k){
    int colisoes = 0;
    for(int i=0; i<B; i++){
        int x = converter(k);
        int pos = h_mul(x, i, B);
        if(strcmp((*t)[pos], "-1") == 0){// posicao nunca ocupada
            strcpy((*t)[pos], k);
            //printf("Armazenou %s\n", (*t)[pos]);
            return colisoes;
        }
        if(strcmp((*t)[pos], k) == 0 ){
            //printf("Palavra %s ja cadastrada\n", (*t)[pos]);
            return colisoes;
        }
        //Se chegou aqui a posicao nao estava vazia e palavra nao era repetida => houve colisao
        colisoes++;
    }
    return colisoes;
}

int buscar_mul(string * t, int B, string k){
    for(int i=0; i<B; i++){
        int x = converter(k);
        int pos = h_mul(x, i, B);
        if(strcmp(t[pos], k) == 0 ){
            //printf("Palavra %s encontrada na posicao %d\n", t[pos], pos);
            return 0;
        }
        if(strcmp(t[pos], "-1") == 0){//sao iguais -> posicao nunca ocupada ou palavra repetida
            //printf("Palavra %s nao existe na lista\n", k);
            return -1;
        }
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    unsigned N = 50000;
    unsigned M = 70000;
    unsigned B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("dados/strings_entrada.txt", N);
    string* consultas = ler_strings("dados/strings_busca.txt", M);


    // cria tabela hash com hash por divisão
    string * t;
    criar(&t, B);

    // inserção dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        //printf("%s\n", insercoes[i]);
        colisoes_h_div += inserir_div(&t, B, insercoes[i]);
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // consulta dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        //printf("%s\n", consultas[i]);
        if(buscar_div(t, B, consultas[i]) == 0){
            encontrados_h_div++;
        }
    }
    double tempo_busca_h_div = finaliza_tempo();

    // limpa a tabela hash com hash por divisão




    // cria tabela hash com hash por divisão

    // inserção dos dados na tabela hash usando hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        colisoes_h_mul += inserir_mul(&t, B, insercoes[i]);
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        if(buscar_mul(t, B, consultas[i]) == 0){
            encontrados_h_mul++;
        }
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // limpa a tabela hash com hash por multiplicação



    printf("Hash por Divisao\n");
    printf("Colisoes na insercao: %d\n", colisoes_h_div);
    printf("Tempo de insercao   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicacao\n");
    printf("Colisoes na insercao: %d\n", colisoes_h_mul);
    printf("Tempo de insercao   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    return 0;
}
