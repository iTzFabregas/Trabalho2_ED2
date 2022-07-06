/* Parte I c) busca sequencial transposicao
Fabrıcio Sampaio - 12547423
Pedro Arthur Francoso - 12547301
Pedro Lucas Castro de Andrade - 11212289
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

// Numero de repeticoes necessarias
#define num_rep 3

//recursividade para ordenacao merge
void merge(int* array, int esquerda, int meio, int direita) {

    int menor = esquerda;
    int maior = meio + 1;
    int* buffer = malloc(sizeof(int) * (direita + 1));
    int cnt = 0;

    while (menor <= meio || maior <= direita) {
        if (menor > meio) {
            buffer[cnt] = array[u];
            maior++;
            cnt++;
        } else if (maior > direita) {
            buffer[cnt] = array[menor];
            menor++;
            cnt++;
        } else {
            if (array[maior] <= array[menor]) {
                buffer[cnt] = array[maior];
                maior++;
                cnt++;
            } else {
                buffer[cnt] = array[menor];
                menor++;
                cnt++;
            }
        }
    }

    cnt = 0;
    for (int k = esquerda; k <= direita; k++) {
        array[k] = buffer[cnt];
        cnt++;
    }
    
    free(buffer);
}

//chamada geral do merge sort
void merge_sort(int* array, int esquerda, int direita) {

    if (esquerda < direita) {
        int meio = (esquerda + direita - 1)/2;
        merge_sort(array, esquerda, meio);
        merge_sort(array, meio + 1, direita);
        merge(array, esquerda, meio, direita);
    } else {
        return;
    }
}

//funcao que le inteiros de um arquivo
int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int* inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

//inicializar a contagem de tempos
void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

//finalizar a contagem de tempo e retornar a diferenca do tempo inicial e final
double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

double desvio_padrao(double* tempos)
{
    double sum = 0;
    for (int i = 0; i < num_rep; i++) {
        sum += tempos[i];
    }
    double media = sum / num_rep;

    double dp = 0;
    for (int i = 0; i < k; i++) {
        dp += (pow((tempos[i] - media),2));
    }
    dp = dp / num_rep;
    dp = sqrt(dp);

    return dp;
}



int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int index_size = 10000;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);
    
    // ordenar entrada
    merge_sort(entradas, 0, N-1);

    // criar tabela de indice
    int T = N/index_size;
    int tabela_index[T];
    for (int i = 0; i < T; i++) {
        tabela_index[i] = index_size*i;
    }

    // realizar consultas na tabela de indices 
    double soma_clocks = 0;
    double tempos[num_rep];
    int* entradas_original = entradas;
    double desvio;
    for (int k = 0; k < num_rep; k++) {

        entradas = entradas_original;
        inicia_tempo();
        
        for (int i = 0; i < N; i++) {
            int j = 0;
            while (entradas[tabela_index[j]] <= consultas[i]) {
                j++;
                if (j >= T) {
                    break;
                }
            }

            
            int cnt = tabela_index[j];
            while (entradas[cnt] >= consultas[i]) {

                if (entradas[cnt] == consultas[i]) {
                    encontrados++;
                    break;
                }
                cnt--;
            }
            continue;
        }

        tempos[k] = finaliza_tempo();
        soma_clocks += tempos[k];
        printf("Execucao numero: %d -> Itens encontrados:  %d -> Tempo de busca:  %fs\n", k+1, encontrados, tempos[k]);
        encontrados = 0;
    }

    desvio = desvio_padrao(tempos);

    printf("\n\n--> Tempo medio de busca     :\t%fs\n", soma_clocks/num_rep);
    printf("--> Desvio padrao dos tempos :\t%f\n", desvio);

    return 0;
}
