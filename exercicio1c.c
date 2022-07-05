#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;

// Definição da lista
typedef struct elemento{
    int dado;
    struct elemento *prox;
}Elem;

typedef struct elemento* Lista;

Lista* cria_lista()
{
    Lista* li = (Lista*) malloc(sizeof(Lista));
    if(li != NULL)
        *li = NULL;
    return li;
}

void libera_lista(Lista* li)
{
    if(li != NULL){
        Elem* no;
        while((*li) != NULL){
            no = *li;
            *li = (*li)->prox;
            free(no);
        }
        free(li);
    }
}


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

double desvio_padrao(double* tempos, int k)
{
    double sum = 0;
    for (int i = 0; i < k; i++) {
        sum += tempos[i];
    }
    double media = sum / 3;

    double dp = 0;
    for (int i = 0; i < k; i++) {
        dp += (pow((tempos[i] - media),2));
    }
    dp = dp / 3;
    dp = sqrt(dp);

    return dp;
}


int main(int argc, char const *argv[])
{
    const int N = 50000;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);

    // realiza busca sequencia com realocação
    double soma_clocks = 0;
    double tempos[3];
    int* entradas_original = entradas;
    for (int k = 0; k < 3; k++) {

        entradas = entradas_original;
        inicia_tempo();

        int buffer;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if(consultas[i] == entradas[j]) {
                    encontrados++;
                    if (j != 0) {
                        buffer = entradas[j];
                        entradas[j] = entradas[j-1];
                        entradas[j-1] = buffer;
                    }
                }
            }
        }
        
        tempos[k] = finaliza_tempo();
        soma_clocks += tempos[k];
        printf("Execucao numero: %d -> Itens encontrados:  %d -> Tempo de busca:  %fs\n", k+1, encontrados, tempos[k]);
        encontrados = 0;
    }

    double desvio = desvio_padrao(tempos, 3);

    printf("\n\n--> Tempo medio de busca     :\t%fs\n", soma_clocks/3);
    printf("--> Desvio padrao dos tempos :\t%f\n", desvio);

    return 0;
}
