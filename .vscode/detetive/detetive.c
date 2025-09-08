#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================
   Estrutura de dados
   ========================= */

// Nó da árvore da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];          // Pista opcional no cômodo
    struct Sala *esquerda;    // Caminho à esquerda
    struct Sala *direita;     // Caminho à direita
} Sala;

// Nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/* =========================
   Funções de criação
   ========================= */

/*
 * criarSala()
 * Cria dinamicamente uma sala com nome e pista opcional
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, ""); // sala sem pista
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/*
 * inserirPista()
 * Insere uma pista na árvore BST de forma ordenada
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    // pistas iguais não são duplicadas

    return raiz;
}

/*
 * exibirPistas()
 * Imprime as pistas em ordem alfabética (in-order traversal)
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/* =========================
   Função de exploração
   ========================= */

/*
 * explorarSalasComPistas()
 * Permite que o jogador explore a mansão e colete pistas
 */
void explorarSalasComPistas(Sala *atual, PistaNode **pistasColetadas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // coleta de pista, se houver
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
        }

        printf("Para onde deseja ir? (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há sala à esquerda!\n");
            }
        } 
        else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há sala à direita!\n");
            }
        } 
        else if (escolha == 's' || escolha == 'S') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        } 
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

/* =========================
   Função principal
   ========================= */

int main() {
    // Criação do mapa da mansão
    Sala *hall = criarSala("Hall de Entrada", "Chave misteriosa");
    hall->esquerda = criarSala("Sala de Estar", "Bilhete antigo");
    hall->direita = criarSala("Cozinha", "Pegada de sapato");
    
    hall->esquerda->esquerda = criarSala("Biblioteca", "Mapa da mansão");
    hall->esquerda->direita = criarSala("Jardim", NULL);
    
    hall->direita->esquerda = criarSala("Despensa", "Frasco suspeito");
    hall->direita->direita = criarSala("Sala de Jantar", "Carta rasgada");

    PistaNode *pistasColetadas = NULL;

    printf("=== Detective Quest: Coletando Pistas na Mansão ===\n");
    explorarSalasComPistas(hall, &pistasColetadas);

    printf("\n=== Pistas coletadas em ordem alfabética ===\n");
    if (pistasColetadas != NULL)
        exibirPistas(pistasColetadas);
    else
        printf("Nenhuma pista foi coletada.\n");

    return 0;
}

