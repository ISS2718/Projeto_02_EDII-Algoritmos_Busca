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

typedef struct el{
    string texto;
    struct el* prox;
} Elem;

typedef Elem* Lista;
//typedef struct Lista* Hash;

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

    for (int i = 0; i<n && !feof(f); i++) {
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

Lista* cria_lista() {
    Lista* li = (Lista*) malloc(sizeof(Lista));
    
    if(li != NULL) {
        *li = NULL;
    }
    return li;
}

void destroi_lista(Lista* li) {
    Elem *no;
    if(li != NULL) {
      while((*li) != NULL){
        no = *li;
        *li = no->prox;
        free(no->texto);
        free(no);
      }
      free(li);
    }
}

int insere_lista(Lista* li, string k) {
    Elem *no, *aux;
    int colisoes = 0;

    if(li == NULL) {
        return 0;
    }
        
    no =  malloc(sizeof(Elem));
    no->texto = malloc(sizeof(char)*25);
    strcpy(no->texto, k);
    no->prox = NULL;
    
    if((*li) == NULL) { //lista vazia
        *li = (Lista)no;
        return 0;
    }

    aux = *li;
    colisoes++;
    if(strcmp(aux->texto, no->texto) != 0) {
        while(aux->prox != NULL) {
            aux = aux->prox;
            if(strcmp(aux->texto, no->texto) == 0) {
              free(no->texto);
              free(no);
              return 0;
            }
            colisoes++;
        }
        aux->prox = no;
        return colisoes;
    }
    free(no->texto);
    free(no);
    return 0;    
}

int busca_lista(Lista* li, string k) {
    Lista no = *li;
    
    if(li == NULL) {
        return -1;
    }
        
    while(no != NULL && strcmp(no->texto, k) != 0){
        no = no->prox;
    }

    if(no == NULL) {
        return -1;
    }
   
    return 0;
}

unsigned h_div(unsigned x, unsigned B)
{
    return (x % B) % B;
}

unsigned h_mul(unsigned x, unsigned B)
{
    const double A = 0.6180;
    return  ((int)(fmod(x * A, 1) * B)) % B;
}

void criar_hash(Lista *** t, int B){
    (*t) = malloc(sizeof(Lista*)*B);
    for(int i = 0; i<B; i++){
        (*t)[i] = cria_lista();
    }
}

void destroi_hash(Lista *** t, unsigned B) {
    if(t != NULL) {
        for(unsigned i = 0; i<B; i++){
            destroi_lista((*t)[i]);
        }
        free(*t);
    }
}

int inserir_div(Lista ***t, int B, string k){
    int colisoes = 0;
    int x = converter(k);
    int pos = h_div(x, B);
    //printf("a");
    colisoes = insere_lista((*t)[pos], k);
    //printf("Armazenou %s\n", (*((*t)[pos]))->texto);

    return colisoes;
}

int buscar_div(Lista** t, int B, string k){
    int x = converter(k);
    int pos = h_div(x, B);
    if(t[pos] == NULL){//sao iguais -> posicao nunca ocupada ou palavra repetida
        //printf("Palavra %s nao existe na lista\n", k);
        return -1;
    }
    if(busca_lista((t)[pos], k) == 0 ){
        //printf("Palavra %s encontrada na posicao %d\n", t[pos], pos);
        return 0;
    }
    return -1;
}

int inserir_mul(Lista *** t, int B, string k){
    int colisoes = 0;
    int x = converter(k);
    int pos = h_mul(x, B);
    colisoes = insere_lista((*t)[pos], k);
    //printf("Armazenou %s\n", (**t)[pos]->texto);

    return colisoes;
}

int buscar_mul(Lista ** t, int B, string k){
    int x = converter(k);
    int pos = h_mul(x, B);
    if(t[pos] == NULL){//posicao nunca ocupada 
        //printf("Palavra %s nao existe na lista\n", k);
        return -1;
    }
    if(busca_lista((t)[pos], k) == 0 ){
        //printf("Palavra %s encontrada na posicao %d\n", t[pos], pos);
        return 0;
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
    Lista ** t;
    criar_hash(&t, B);

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
    destroi_hash(&t, B);
  
    // cria tabela hash com hash por divisão
    criar_hash(&t, B);

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
    destroi_hash(&t, B);
  
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

    for (int i = 0; i < N; i++) {
        free(insercoes[i]);
    }
    free(insercoes);
    for (int i = 0; i < M; i++) {
        free(consultas[i]);
    }
    free(consultas);

    return 0;
}