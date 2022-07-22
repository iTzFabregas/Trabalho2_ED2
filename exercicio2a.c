/* Parte II a) hash fechado
Fabrıcio Sampaio - 12547423
Pedro Arthur Francoso - 12547301
Pedro Lucas Castro de Andrade - 11212289
*/

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

// Numero de repeticoes necessarias
#define num_rep 3

unsigned converter(string s) {
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++) 
      h = h * 256 + s[i];
   return h;
}

string* ler_strings(const char * arquivo, const int n) {
    FILE* f = fopen(arquivo, "r");
    
    string* strings = (string *) malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
}

void inicia_tempo() {
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo() {
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

unsigned h_div(unsigned x, unsigned i, unsigned B) {
    return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i, unsigned B) {
    const double A = 0.6180;
    return  ((int) ((fmod(x * A, 1) * B) + i)) % B;
}

double desvio_padrao(double* tempos)
{
    double sum = 0;
    for (int i = 0; i < num_rep; i++) {
        sum += tempos[i];
    }
    double media = sum / num_rep;

    double dp = 0;
    for (int i = 0; i < num_rep; i++) {
        dp += (pow((tempos[i] - media),2));
    }
    dp = dp / num_rep;
    dp = sqrt(dp);

    return dp;
}


hash* criar_tabela(int B) {
    hash* tabela = malloc(sizeof(hash) * B);
    if (tabela == NULL) { //problema na alocacao de memoria
        printf("Erro alocacao tabela\n");
        exit(-1);
    }
    for (int i = 0; i < B; i++) {
        strcpy(tabela[i].dados, "VAZIO\0"); // string para indicar que o bucket esta livre
    }
    return tabela;
}

void liberar_tabela(hash* tabela, int B) {
    free(tabela);
}

int inserir_tabela_div(hash* tabela_hash, string inserir, int B) {
    unsigned pos;
    unsigned string_num = converter(inserir);
    for (int i = 0; i < B;  i++) {
        pos = h_div(string_num, i, B);

        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) {
            strcpy(tabela_hash[pos].dados, inserir); // elemento inserido na tabela
            return (i == 0) ? 0 : 1; // se precisou procurar mais de uma vez, houve colisao
        }
        if (!(strcmp(tabela_hash[pos].dados, inserir))) {
            return 0; // elemento repetido
        }
    }
    return 1; // lista cheia
}

int busca_tabela_div(hash* tabela_hash, string consultar, int B) {
    unsigned pos;
    unsigned string_num = converter(consultar);
    for (int i = 0; i < B;  i++) {
        pos = h_div(string_num, i, B);
        if (!(strcmp(tabela_hash[pos].dados, consultar))) {       
            return 1; // elemento encontrado
        }
        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) {
            return 0; // elemento nao esta na tabela
        }
    }
    return 0; // elemento nao esta na tabela
}

int inserir_tabela_mul(hash* tabela_hash, string inserir, int B) {
    unsigned pos;
    unsigned string_num = converter(inserir);
    for (int i = 0; i < B;  i++) {
        pos = h_mul(string_num, i, B);

        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) {
            strcpy(tabela_hash[pos].dados, inserir); // elemento inserido na tabela
            return (i == 0) ? 0 : 1; // se preciou procurar mais de uma vez, houve colisao
        }
        if (!(strcmp(tabela_hash[pos].dados, inserir))) {
            return 0; // elemento repetido
        }
    }
    return 1; // lista cheia
}

int busca_tabela_mul(hash* tabela_hash, string consultar, int B) {
    unsigned pos;
    unsigned string_num = converter(consultar);
    for (int i = 0; i < B;  i++) {
        pos = h_mul(string_num, i, B);
        if (!(strcmp(tabela_hash[pos].dados, consultar))) {       
            return 1; // elemento encontrado
        }
        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) {
            return 0; // elemento nao esta na tabela
        }
    }
    return 0; // elemento nao esta na tabela
}


int main(int argc, char const *argv[]) {
    unsigned N = 50000;
    unsigned M = 70000;
    unsigned B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);

    double soma_clocks_insercao_div = 0;
    double soma_clocks_busca_div = 0;
    double tempos_insercao_div[num_rep];
    double tempos_busca_div[num_rep];
    
    for (int k = 0; k < num_rep; k++) {

        // cria tabela hash com hash por divisão
        hash* tabela_hash_div = criar_tabela(B);

        // inserção dos dados na tabela hash usando hash por divisão
        inicia_tempo();

        for (int i = 0; i < N; i++) {
            // inserir insercoes[i] na tabela hash
            colisoes_h_div += inserir_tabela_div(tabela_hash_div, insercoes[i], B);
        }

        tempos_insercao_div[k] = finaliza_tempo();
        soma_clocks_insercao_div += tempos_insercao_div[k];
        printf("Insercao numero : %d -> Tempo de insecao:  %fs\n", k+1, tempos_insercao_div[k]);


        // consulta dos dados na tabela hash usando hash por divisão
        inicia_tempo();

        for (int i = 0; i < M; i++) {
            // buscar consultas[i] na tabela hash
            encontrados_h_div += busca_tabela_div(tabela_hash_div, consultas[i], B);
        }

        tempos_busca_div[k] = finaliza_tempo();
        soma_clocks_busca_div += tempos_busca_div[k];
        printf("Busca numero    : %d -> Tempo de busca:  %fs\n", k+1, tempos_busca_div[k]);

        // limpa a tabela hash com hash por divisão
        liberar_tabela(tabela_hash_div, B);
        printf("\n");

    }


    double soma_clocks_insercao_mul = 0;
    double soma_clocks_busca_mul = 0;
    double tempos_insercao_mul[num_rep];
    double tempos_busca_mul[num_rep];

    for (int k = 0; k < num_rep; k++) {

        // cria tabela hash com hash por multiplicação
        hash* tabela_hash_mul = criar_tabela(B);
        // inserção dos dados na tabela hash usando hash por multiplicação
        inicia_tempo();
        for (int i = 0; i < N; i++) {
            // inserir insercoes[i] na tabela hash
            colisoes_h_mul += inserir_tabela_mul(tabela_hash_mul, insercoes[i], B);
        }
        tempos_insercao_mul[k] = finaliza_tempo();
        soma_clocks_insercao_mul += tempos_insercao_mul[k];
        printf("Insercao numero : %d -> Tempo de busca:  %fs\n", k+1, tempos_insercao_mul[k]);

        // busca dos dados na tabela hash com hash por multiplicação
        inicia_tempo();
        for (int i = 0; i < M; i++) {
            // buscar consultas[i] na tabela hash
            encontrados_h_mul += busca_tabela_mul(tabela_hash_mul, consultas[i], B);
        }
        
        tempos_busca_mul[k] = finaliza_tempo();
        soma_clocks_busca_mul += tempos_busca_mul[k];
        printf("Busca numero    : %d -> Tempo de busca:  %fs\n", k+1, tempos_busca_mul[k]);

        // limpa a tabela hash com hash por multiplicação
        liberar_tabela(tabela_hash_mul, B);
        printf("\n");

    }

    float desvio_insercao_div = desvio_padrao(tempos_insercao_div);
    float desvio_busca_div = desvio_padrao(tempos_busca_div);
    printf("\n---> Hash por Divisao <---\n");
    printf("--> Tempo medio de insercao   :\t%fs\n", soma_clocks_insercao_div/num_rep);
    printf("--> Desvio padrao da insercao :\t%f\n", desvio_insercao_div);
    printf("--> Tempo medio de busca      :\t%fs\n", soma_clocks_busca_div/num_rep);
    printf("--> Desvio padrao da busca    :\t%f\n", desvio_busca_div);
    printf("--> Encontrados   :\t%d\n", encontrados_h_div/3);
    printf("--> Colisoes   :\t%d\n", colisoes_h_div/3);

    printf("\n");

    float desvio_insercao_mul = desvio_padrao(tempos_insercao_mul);
    float desvio_busca_mul = desvio_padrao(tempos_busca_mul);
    printf("---> Hash por Multiplicacao <---\n");
    printf("--> Tempo medio de insercao   :\t%fs\n", soma_clocks_insercao_mul/num_rep);
    printf("--> Desvio padrao da insercao :\t%f\n", desvio_insercao_mul);
    printf("--> Tempo medio de busca      :\t%fs\n", soma_clocks_busca_mul/num_rep);
    printf("--> Desvio padrao da busca    :\t%f\n", desvio_busca_mul);
    printf("--> Encontrados   :\t%d\n", encontrados_h_mul/3);
    printf("--> Colisoes   :\t%d\n", colisoes_h_mul/3);

    return 0;
}
