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

// Definindo  structs necessarias para o hash
typedef char elem;

typedef struct {
    elem dados[20];
}hash;


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

hash* criar_tabela(int B) {
    hash* tabela = malloc(sizeof(hash) * B);
    if (tabela == NULL) {
        printf("Erro alocacao tabela\n");
        exit(-1);
    }
    for (int i = 0; i < B; i++) {
        strcpy(tabela[i].dados, "VAZIO\0");
    }
    return tabela;
}

void liberar_tabela(hash* tabela, int B) {
    free(tabela);
}

int inserir_tabela_div(hash* tabela_hash, string inserir, int B) {
    int colisoes = 0;
    unsigned pos;
    unsigned string_num = converter(inserir);
    for (int i = 0; i < B;  i++) {
        pos = h_div(string_num, i, B);

        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) {
            strcpy(tabela_hash[pos].dados, inserir);
            return colisoes; // inserida
        }
        if (!(strcmp(tabela_hash[pos].dados, inserir))) {
            //colisoes = 1; // ISSO SERIA CONSIDERADO UMA COLISÃO ???
            return colisoes; // ja foi inserida
        }
        colisoes = 1;
    }
    return colisoes; // lista cheia
}

int busca_tabela_div(hash* tabela_hash, string consultar, int B) {
    unsigned pos;
    unsigned string_num = converter(consultar);
    for (int i = 0; i < B;  i++) {
        pos = h_div(string_num, i, B);
        if (!(strcmp(tabela_hash[pos].dados, consultar))) {       
            return 1; // encontrado
        }
        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) {
            return 0; // nao foi inserido
        }
    }
    return 0; // lsita cheia00
}

int inserir_tabela_mul(hash* tabela_hash, string inserir, int B) {
    int colisoes = 0;
    unsigned pos;
    unsigned string_num = converter(inserir);
    for (int i = 0; i < B;  i++) {
        pos = h_mul(string_num, i, B);

        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) {
            strcpy(tabela_hash[pos].dados, inserir);
            return colisoes; // inserida
        }
        if (!(strcmp(tabela_hash[pos].dados, inserir))) {
            //colisoes = 1; // ISSO SERIA CONSIDERADO UMA COLISÃO ???
            return colisoes; // ja foi inserida
        }
        colisoes = 1;
    }
    return colisoes; // lista cheia
}

int busca_tabela_mul(hash* tabela_hash, string consultar, int B) {
    unsigned pos;
    unsigned string_num = converter(consultar);
    for (int i = 0; i < B;  i++) {
        pos = h_mul(string_num, i, B);
        if (!(strcmp(tabela_hash[pos].dados, consultar))) {       
            return 1; // encontrado
        }
        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) {
            return 0; // nao foi inserido
        }
    }
    return 0; // lsita cheia00
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

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);


    // cria tabela hash com hash por divisão
    hash* tabela_hash_div = criar_tabela(B);

    // inserção dos dados na tabela hash usando hash por divisão
    inicia_tempo();

    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        colisoes_h_div += inserir_tabela_div(tabela_hash_div, insercoes[i], B);
    }

    double tempo_insercao_h_div = finaliza_tempo();


    // consulta dos dados na tabela hash usando hash por divisão
    inicia_tempo();

    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        encontrados_h_div += busca_tabela_div(tabela_hash_div, consultas[i], B);
    }

    double tempo_busca_h_div = finaliza_tempo();

    // limpa a tabela hash com hash por divisão
    liberar_tabela(tabela_hash_div, B);



    // cria tabela hash com hash por divisão
    hash* tabela_hash_mul = criar_tabela(B);

    // inserção dos dados na tabela hash usando hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        colisoes_h_mul += inserir_tabela_mul(tabela_hash_mul, insercoes[i], B);
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        encontrados_h_mul += busca_tabela_mul(tabela_hash_mul, consultas[i], B);
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // limpa a tabela hash com hash por multiplicação



    printf("Hash por Divisao\n");
    printf("Colisoes na insercao : %d\n", colisoes_h_div);
    printf("Tempo de insercao    : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca       : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados    : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicacao\n");
    printf("Colisoes na insercao: %d\n", colisoes_h_mul);
    printf("Tempo de insercao   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);
    return 0;
}
