#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
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

/* -------- Funcoes do TAD -------- */

typedef struct no {
    string num;
    struct no* prox;
} no;

int cria_lista(no **lista){
    *lista = malloc(sizeof(no));
    if(lista != NULL){
        (*lista)->prox = NULL;
        (*lista)->num = "*";
        return 0;
    } else {
        return 1;
    }
}

int insere_final(no **lista, string ent){
    no *aux = *lista;
    no *new = malloc(sizeof(no));
    new->num = ent;
    new->prox = NULL;

    if (aux == NULL){
        (*aux) = new;
        return 0;
    }

    while (aux->prox != NULL){
        if(strcomp(aux->num, ent) == 0) return 0;
        aux = aux->prox;
    }

    if (strcomp(aux->num, ent) == 0) return 0;
    aux->prox = new;

    return 1;
}

int busca_lista(no **lista, string ent){
    no *aux = *lista;
  while(aux != NULL){
    if(strcmp(aux->num, ent) == 0)
      //Encontrado
      return 0;
    aux = aux->prox;
  }
  return -1;
}

void destroi_lista(no **lista){
    no *aux = *lista;
    no *next;
    while (*aux != NULL){
        next = aux->prox;
        free(aux);
        aux = next;
    }
}

/* ------- Fim das funções do TAD --------*/

typedef struct {
    no **lista;
} hash;

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

unsigned h_div(unsigned x, unsigned B)
{
    return x % B;
}

unsigned h_mul(unsigned x, unsigned B)
{
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
}

void cria_hash(int tamanho, hash *ha){
    ha->lista = malloc(sizeof(no*)*tamanho);
    for (int i = 0; i < tamanho; i++){
        ha->lista[i] = NULL;
    }
}    

void destroi_hash(hash *ha, const int tamanho){
    for (int i = 0; i < tamanho; i++) destroi_lista(&ha->lista[i]);
}

int inserir (hash *ha, string ent, unsigned (*f)(unsigned int)){
    unsigned int pos;
    unsigned int key = converter(ent);
    pos = (*f)(key);
    return insere_final(&(ha->lista[pos]), ent);
}

int busca_hash(hash *ha, string busca, unsigned (*f)(unsigned int)){
    unsigned int pos;
    unsigned int key = converter(busca);
    pos = (*f)(key);
    return(busca_lista(&(ha->lista[pos]), busca));
}

void printar_lista(no **node){
    no *aux= *node;
    printf("Lista: ");
    while(aux != NULL){
        printf("%s - ", aux->num);
        aux = aux->prox;
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);
    

    // cria tabela hash com hash por divisão

    
    // inserção dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // busca dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
    }
    double tempo_busca_h_div = finaliza_tempo();

    // destroi tabela hash com hash por divisão




    // cria tabela hash com hash por multiplicação

    // inserção dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // destroi tabela hash com hash por multiplicação



    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    return 0;
}
