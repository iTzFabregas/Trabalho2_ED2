/* Parte I b) busca sequencial mover-para-frente
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

//funcao que le inteiros de um arquivo
int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int * inteiros = (int *) malloc(sizeof(int) * n);

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
    for (int i = 0; i < num_rep; i++) {
        dp += (pow((tempos[i] - media),2));
    }
    dp = dp / num_rep;
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
    double tempos[num_rep];
    double desvio;
    int* entradas_original = entradas;
    for (int k = 0; k < num_rep; k++) {

        entradas = entradas_original;
        inicia_tempo();
        
        int buffer;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if(consultas[i] == entradas[j]) {
                    encontrados++;
                    if (j != 0) {
                        buffer = entradas[j];
                        entradas[j] = entradas[0];
                        entradas[0] = buffer;
                    }
                }
            }
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
