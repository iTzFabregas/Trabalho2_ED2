/* Parte II c) hash aberto com lista encadeada
Fabrıcio Sampaio - 12547423
Pedro Arthur Francoso - 12547301
Pedro Lucas Castro de Andrade - 11212289
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

// Numero de repeticoes necessarias
#define num_rep 3

/* -------- Funcoes do TAD -------- */

typedef struct no {
    string info;
    struct no* prox;
} no;

int insere_final(no **lista, string ent){
    no *aux = *lista;
    no *new = malloc(sizeof(no));
    new->info = ent;
    new->prox = NULL;

    if (aux == NULL){ //bucket estava vazio
        (*lista) = new;
        return 0;
    }

    while (aux->prox != NULL){ //percorre a lista até achar um no vazio
        if(strcmp(aux->info, ent) == 0){
            return 0; //elemento repetido
        }
        aux = aux->prox;
    }
    
    if (strcmp(aux->info, ent) == 0){
        return 0; //elemento repetido
    }
    aux->prox = new;

    return 1; //insercao com ocorrencia de colisao
}

int busca_lista(no **lista, string ent){
    no *aux = *lista;
    while(aux != NULL){ //percorre a lista
        if(strcmp(aux->info, ent) == 0){
            return 0; //elemento encontrado
        }
        aux = aux->prox;
    }
  return -1; //elemento não encontrado
}

void destroi_lista(no **lista){
    no *aux;
    while ((*lista) != NULL){ //percorre a lista
        aux = *lista;
        *lista = (*lista)->prox;
        free(aux); //libera a memoria alocada para o no
    }
    free(*lista); 
}

/* ------- Fim das funções do TAD --------*/

typedef struct {
    no **lista;
} hash;

unsigned converter(string s){
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++) 
      h = h * 256 + s[i];
   return h;
}

string* ler_strings(const char * arquivo, const int n){
    FILE* f = fopen(arquivo, "r");
    
    string* strings = (string *) malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
}

void inicia_tempo(){
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo(){
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

unsigned h_div(unsigned x, unsigned B){
    return x % B;
}

unsigned h_mul(unsigned x, unsigned B){
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
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

void cria_hash(hash *ha, int B){
    ha->lista = malloc(sizeof(no*) * B); //aloca memoria necessaria para o hash
    for (int i = 0; i < B; i++){
        ha->lista[i] = NULL; //cada bucket comeca com uma lista vazia
    }
}    

void destroi_hash(hash *ha, int B){
    for (int i = 0; i < B; i++){ 
        destroi_lista(&(ha->lista[i]));
    }
    free(ha->lista);
}

int inserir (hash *ha, string ent, int B, unsigned (*f)(unsigned, unsigned)){
    unsigned int pos;
    unsigned int key = converter(ent);
    pos = (*f)(key, B);
    return insere_final(&(ha->lista[pos]), ent);
}

int busca_hash(hash *ha, string busca, int B, unsigned (*f)(unsigned, unsigned)){
    unsigned int pos;
    unsigned int key = converter(busca);
    pos = (*f)(key, B);
    return(busca_lista(&(ha->lista[pos]), busca) != -1) ? pos : -1;
}

int main(int argc, char const *argv[]){
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

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
        hash tabela;
        cria_hash(&tabela, B);

        // inserção dos dados na tabela hash com hash por divisão
        inicia_tempo();
        for (int i = 0; i < N; i++) {
            string leitura = insercoes[i];
            colisoes_h_div += inserir(&tabela, leitura, B, &h_div);
        }
        tempos_insercao_div[k] = finaliza_tempo();
        soma_clocks_insercao_div += tempos_insercao_div[k];
        printf("Insercao numero : %d -> Tempo de busca:  %fs\n", k+1, tempos_insercao_div[k]);

        // busca dos dados na tabela hash com hash por divisão
        inicia_tempo();
        for (int i = 0; i < M; i++) {  
            string consulta = consultas[i];
            if(busca_hash(&tabela, consulta, B, &h_div) != -1){
                encontrados_h_div++;
            }
        }
        tempos_busca_div[k] = finaliza_tempo();
        soma_clocks_busca_div += tempos_busca_div[k];
        printf("Busca numero    : %d -> Tempo de busca:  %fs\n", k+1, tempos_busca_div[k]);
        printf("Elementos encontrados : %d\n\n", encontrados_h_div);

        // destroi tabela hash com hash por divisão
        encontrados_h_div = 0;
        destroi_hash(&tabela, B);

    }

    double soma_clocks_insercao_mul = 0;
    double soma_clocks_busca_mul = 0;
    double tempos_insercao_mul[num_rep];
    double tempos_busca_mul[num_rep];

    for (int k = 0; k < num_rep; k++) {

        // cria tabela hash com hash por multiplicação
        hash tabela;
        cria_hash(&tabela, B);
        // inserção dos dados na tabela hash com hash por multiplicação
        inicia_tempo();
        for (int i = 0; i < N; i++) {
            string leitura = insercoes[i];
            // inserir insercoes[i] na tabela hash
            colisoes_h_mul += inserir(&tabela, leitura, B, &h_mul);
        }
        tempos_insercao_mul[k] = finaliza_tempo();
        soma_clocks_insercao_mul += tempos_insercao_mul[k];
        printf("Insercao numero : %d -> Tempo de busca:  %fs\n", k+1, tempos_insercao_mul[k]);

        // busca dos dados na tabela hash com hash por multiplicação
        inicia_tempo();
        for (int i = 0; i < M; i++) {
            string consulta = consultas[i];
            if(busca_hash(&tabela, consulta, B, &h_mul) != -1){
                encontrados_h_mul++;
            }
        }
        tempos_busca_mul[k] = finaliza_tempo();
        soma_clocks_busca_mul += tempos_busca_mul[k];
        printf("Busca numero    : %d -> Tempo de busca:  %fs\n", k+1, tempos_busca_mul[k]);
        printf("Elementos encontrados : %d\n\n", encontrados_h_mul);

        // destroi tabela hash com hash por multiplicação
        encontrados_h_mul = 0;
        destroi_hash(&tabela, B);

    }

    float desvio_insercao_div = desvio_padrao(tempos_insercao_div);
    float desvio_busca_div = desvio_padrao(tempos_busca_div);
    printf("\n---> Hash por Divisao <---\n");
    printf("--> Tempo medio de insercao   :\t%fs\n", soma_clocks_insercao_div/num_rep);
    printf("--> Desvio padrao da insercao :\t%f\n", desvio_insercao_div);
    printf("--> Tempo medio de busca      :\t%fs\n", soma_clocks_busca_div/num_rep);
    printf("--> Desvio padrao da busca    :\t%f\n", desvio_busca_div);

    printf("\n");

    float desvio_insercao_mul = desvio_padrao(tempos_insercao_mul);
    float desvio_busca_mul = desvio_padrao(tempos_busca_mul);
    printf("---> Hash por Multiplicacao <---\n");
    printf("--> Tempo medio de insercao   :\t%fs\n", soma_clocks_insercao_mul/num_rep);
    printf("--> Desvio padrao da insercao :\t%f\n", desvio_insercao_mul);
    printf("--> Tempo medio de busca      :\t%fs\n", soma_clocks_busca_mul/num_rep);
    printf("--> Desvio padrao da busca    :\t%f\n", desvio_busca_mul);

    return 0;
}
