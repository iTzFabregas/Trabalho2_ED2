#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void insere_lista_inicio(Lista* lista, int dt)
{
    if(lista == NULL) {
        return;
    }
    Elem* no;
    no = (Elem*) malloc(sizeof(Elem));
    if(no == NULL) {
        return;
    }
    no->dado = dt;
    no->prox = (*lista);
    *lista = no;
    return;
}

ler_inteiros(const char * arquivo, Lista* lista)
{
    FILE* f = fopen(arquivo, "r");

    for (int i = 0; !feof(f); i++) {
        int num;
        fscanf(f, "%d\n", &num);
        insere_lista_inicio(lista, num);
    }  
    fclose(f);
}

void mover_para_frente(Lista* entradas, int index) {

    int buffer = entradas[index];

    for(int i = 0; i < index; i++) {
        entradas[i+1] = entradas[i];
    }
    entradas[0] = buffer;
    return entradas;
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


int main(int argc, char const *argv[])
{
    const int N = 50000;
    unsigned encontrados = 0;
    Lista* entradas;
    entradas = cria_lista();

    Lista* consultas;
    consultas = cria_lista();

    entradas = ler_inteiros("inteiros_entrada.txt", N);
    consultas = ler_inteiros("inteiros_busca.txt", N);

    // realiza busca sequencia com realocação
    inicia_tempo();
    
    Elem* cons = *consultas;
    Elem* prev_entry;
    Elem* entry = *entradas;
    while(cons != NULL) {
        while(entry != NULL) {
            if (entry->dado = cons->dado) {
                if (entry == entradas*)
                Elem *buffer = entry;

            }
        }
    }

    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    libera_lista(entradas);
    libera_lista(consultas);
    return 0;
}
