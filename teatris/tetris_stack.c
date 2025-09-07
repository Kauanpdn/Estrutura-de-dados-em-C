#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ==========================
// Definição das constantes
// ==========================
#define MAX_FILA 5   // capacidade da fila
#define MAX_PILHA 3  // capacidade da pilha

// ==========================""
// Estrutura da peça
// ==========================
typedef struct {
    char nome;  // tipo da peça ('I', 'O', 'T', 'L')
    int id;     // identificador único
} Peca;

// ==========================
// Estrutura da fila circular
// ==========================
typedef struct {
    Peca elementos[MAX_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

// ==========================
// Estrutura da pilha
// ==========================
typedef struct {
    Peca elementos[MAX_PILHA];
    int topo;
} Pilha;

// ==========================
// Variável global de ID
// ==========================
int contadorId = 0;

// ==========================
// Função: gerar nova peça
// ==========================
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorId++;
    return nova;
}

// ==========================
// Funções da fila
// ==========================
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

void enfileirar(Fila *fila, Peca p) {
    if (fila->tamanho == MAX_FILA) return;
    fila->tras = (fila->tras + 1) % MAX_FILA;
    fila->elementos[fila->tras] = p;
    fila->tamanho++;
}

Peca desenfileirar(Fila *fila) {
    Peca removida = {'-', -1};
    if (fila->tamanho == 0) return removida;
    removida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->tamanho--;
    return removida;
}

// ==========================
// Funções da pilha
// ==========================
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int empilhar(Pilha *pilha, Peca p) {
    if (pilha->topo == MAX_PILHA - 1) {
        printf("⚠️ Pilha cheia!\n");
        return 0;
    }
    pilha->elementos[++pilha->topo] = p;
    return 1;
}

Peca desempilhar(Pilha *pilha) {
    Peca removida = {'-', -1};
    if (pilha->topo == -1) {
        printf("⚠️ Pilha vazia!\n");
        return removida;
    }
    return pilha->elementos[pilha->topo--];
}

// =======
