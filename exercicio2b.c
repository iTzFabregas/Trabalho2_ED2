/* Parte II b) hash fechado com rehash duplo
Fabrıcio Sampaio - 12547423
Pedro Arthur Francoso - 12547301
Pedro Lucas Castro de Andrade - 11212289
*/

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
        strcpy(tabela[i].dados, "VAZIO\0"); // indica que o bucket esta disponivel
    }
    return tabela;
}

void liberar_tabela(hash* tabela, int B) {
    free(tabela);
}

int inserir_tabela(hash* tabela_hash, string inserir, int B) {
    unsigned pos;
    unsigned string_num = converter(inserir);
    for (int i = 0; i < B;  i++) {
        pos = (h_mul(string_num, i, B) + i * h_div(string_num, i, B)) % B;

        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) { 
            strcpy(tabela_hash[pos].dados, inserir);
             return (i == 0) ? 0 : 1; // se precisou procurar mais de uma vez, houve colisao
        }
        if (!(strcmp(tabela_hash[pos].dados, inserir))) {
            //colisoes = 1; // ISSO SERIA CONSIDERADO UMA COLISÃO ???
            return 0; // elemento repetido
        }
    }
    return 1; // lista cheia
}

int busca_tabela(hash* tabela_hash, string consultar, int B) {
    unsigned pos;
    unsigned string_num = converter(consultar);
    for (int i = 0; i < B;  i++) {
       pos = (h_mul(string_num, i, B) + i * h_div(string_num, i, B)) % B;
        if (!(strcmp(tabela_hash[pos].dados, consultar))) {       
            return 1; // encontrado
        }
        if (!(strcmp(tabela_hash[pos].dados, "VAZIO"))) {
            return 0; // nao foi inserido
        }
    }
    return 0; // nao foi encontrado
}

int main(int argc, char const *argv[]) {
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes = 0;
    unsigned encontrados = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);


    double soma_clocks_insercao = 0;
    double soma_clocks_busca = 0;
    double tempos_insercao[num_rep];
    double tempos_busca[num_rep];

    for(int k = 0; k < num_rep; k++) {

        // cria tabela hash com hash por hash duplo
        hash* tabela_hash = criar_tabela(B);

        // inserção dos dados na tabela hash
        inicia_tempo();
        for (int i = 0; i < N; i++) {
            // inserir insercoes[i] na tabela hash
            colisoes += inserir_tabela(tabela_hash, insercoes[i], B);
        }
        tempos_insercao[k] = finaliza_tempo();
        soma_clocks_insercao += tempos_insercao[k];
        printf("Insercao numero : %d -> Tempo de busca:  %fs\n", k+1, tempos_insercao[k]);


        // busca dos dados na tabela hash
        inicia_tempo();
        for (int i = 0; i < M; i++) {
            // buscar consultas[i] na tabela hash
            encontrados += busca_tabela(tabela_hash, consultas[i], B);
        }
        tempos_busca[k] = finaliza_tempo();
        soma_clocks_busca += tempos_busca[k];
        printf("Busca numero    : %d -> Tempo de busca:  %fs\n", k+1, tempos_busca[k]);
        printf("Elementos encontrados : %d\n\n", encontrados);

        // limpa a tabela hash
        encontrados = 0;
        liberar_tabela(tabela_hash, B);

    }


    float desvio_insercao = desvio_padrao(tempos_insercao);
    float desvio_busca = desvio_padrao(tempos_busca);
    printf("\n--> Tempo medio de insercao   :\t%fs\n", soma_clocks_insercao/num_rep);
    printf("--> Desvio padrao da insercao :\t%f\n", desvio_insercao);
    printf("--> Tempo medio de busca      :\t%fs\n", soma_clocks_busca/num_rep);
    printf("--> Desvio padrao da busca    :\t%f\n", desvio_busca);

    return 0;
}
