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



void merging(int* array, int left, int mid, int right) {

    int l = left;
    int u = mid+1;
    int* buffer = malloc(sizeof(int) * (right+1));
    int cnt = 0;

    while (l <= mid || u <= right) {
        if (l > mid) {
            buffer[cnt] = array[u];
            u++;
            cnt++;
        } else if (u > right) {
            buffer[cnt] = array[l];
            l++;
            cnt++;
        } else {
            if (array[u] <= array[l]) {
                buffer[cnt] = array[u];
                u++;
                cnt++;
            } else {
                buffer[cnt] = array[l];
                l++;
                cnt++;
            }
        }
    }

    cnt = 0;
    for (int k = left; k <= right; k++) {
        array[k] = buffer[cnt];
        cnt++;
    }
    
    free(buffer);
}


void merge_sort(int* array, int left, int right) {

    if (left < right) {
        int mid = (left+right-1)/2;
        merge_sort(array, left, mid);
        merge_sort(array, mid+1, right);
        merging(array, left, mid, right);
    } else {
        return;
    }
}



int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int* inteiros = (int *) malloc(sizeof(int) * n);

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
    double tempos[3];
    int* entradas_original = entradas;
    for (int k = 0; k < 3; k++) {

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

    double desvio = desvio_padrao(tempos, 3);

    printf("\n\n--> Tempo medio de busca     :\t%fs\n", soma_clocks/3);
    printf("--> Desvio padrao dos tempos :\t%f\n", desvio);

    return 0;
}
