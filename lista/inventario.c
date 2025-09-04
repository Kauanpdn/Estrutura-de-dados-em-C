#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMP 20

// -------------------------------
// Struct principal
// -------------------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int  prioridade; // 1 (alta) ... 10 (baixa) — ou conforme seu jogo
} Componente;

// -------------------------------
// Estado global (vetor de trabalho)
// -------------------------------
Componente comps[MAX_COMP];
int total = 0;

// Flag para habilitar busca binária (só funciona após ordenar por nome)
int ultimoOrdenadoPorNome = 0;

// -------------------------------
// Utilidades de I/O
// -------------------------------
void limpaNewline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n && s[n-1] == '\n') s[n-1] = '\0';
}

void lerLinha(const char *prompt, char *buf, size_t tam) {
    printf("%s", prompt);
    if (fgets(buf, (int)tam, stdin) == NULL) {
        // Em caso de EOF, garantir string vazia
        buf[0] = '\0';
        return;
    }
    limpaNewline(buf);
}

int lerIntFaixa(const char *prompt, int min, int max) {
    char linha[64];
    int v, ok = 0;
    do {
        printf("%s", prompt);
        if (!fgets(linha, sizeof(linha), stdin)) {
            clearerr(stdin);
            continue;
        }
        if (sscanf(linha, "%d", &v) == 1 && v >= min && v <= max) {
            ok = 1;
        } else {
            printf("Valor inválido. Digite um número entre %d e %d.\n", min, max);
        }
    } while (!ok);
    return v;
}

// -------------------------------
/* Exibição */
void mostrarComponentes(Componente v[], int n) {
    if (n == 0) {
        printf("\n📦 Lista vazia.\n");
        return;
    }
    printf("\n--- Componentes ---\n");
    for (int i = 0; i < n; i++) {
        printf("%2d) Nome: %-20s | Tipo: %-12s | Prioridade: %d\n",
               i+1, v[i].nome, v[i].tipo, v[i].prioridade);
    }
}

// -------------------------------
// Swapper
void trocar(Componente *a, Componente *b) {
    Componente t = *a; *a = *b; *b = t;
}

// -------------------------------
// Algoritmos de ordenação (contam comparações)
// -------------------------------

// 1) Bubble Sort por NOME (string)
long long bubbleSortNome(Componente v[], int n) {
    long long comps = 0;
    for (int i = 0; i < n-1; i++) {
        int trocou = 0;
        for (int j = 0; j < n-1-i; j++) {
            comps++;
            if (strcmp(v[j].nome, v[j+1].nome) > 0) {
                trocar(&v[j], &v[j+1]);
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    return comps;
}

// 2) Insertion Sort por TIPO (string)
long long insertionSortTipo(Componente v[], int n) {
    long long comps = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;

        // Contagem: comparamos enquanto mantemos o loop
        while (j >= 0) {
            comps++; // compara v[j].tipo com chave.tipo
            if (strcmp(v[j].tipo, chave.tipo) > 0) {
                v[j+1] = v[j];
                j--;
            } else {
                break;
            }
        }
        v[j+1] = chave;
    }
    return comps;
}

// 3) Selection Sort por PRIORIDADE (int)
long long selectionSortPrioridade(Componente v[], int n) {
    long long comps = 0;
    for (int i = 0; i < n-1; i++) {
        int minIdx = i;
        for (int j = i+1; j < n; j++) {
            comps++;
            if (v[j].prioridade < v[minIdx].prioridade) {
                minIdx = j;
            }
        }
        if (minIdx != i) trocar(&v[i], &v[minIdx]);
    }
    return comps;
}

// -------------------------------
// Busca Binária por NOME (vetor precisa estar ordenado por no
