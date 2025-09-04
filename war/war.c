#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =========================
   ESTRUTURAS DE DADOS
   ========================= */

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

typedef struct {
    char nome[20];
    char cor[10];
    char *missao;   // armazenada dinamicamente
} Jogador;

/* =========================
   PROTÓTIPOS
   ========================= */

// Cadastro e exibição
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirMapa(Territorio* mapa, int n);

// Combate
void atacar(Territorio* atacante, Territorio* defensor);

// Missões
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(const char* nomeJogador, const char* missao);

// Verificação de missões
// OBS: usamos uma variável global com a cor do jogador atualmente verificado
// para cumprir o protótipo exigido na especificação.
int verificarMissao(char* missao, Territorio* mapa, int tamanho);

// Liberação de memória
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int qtdJogadores);

/* =========================
   VARIÁVEIS GLOBAIS AUXILIARES
   ========================= */

// Cor do jogador atualmente sendo checado em verificarMissao()
static const char* g_corAtual = NULL;

/* =========================
   IMPLEMENTAÇÕES
   ========================= */

void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);

        printf("Cor do exército: ");
        scanf(" %9s", mapa[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

void exibirMapa(Territorio* mapa, int n) {
    printf("\n=== Estado Atual do Mapa ===\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] %-28s | Cor: %-9s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("⚠️  Não é possível atacar um território da mesma cor!\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("⚠️  O atacante precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n🎲 %s (ATACANTE, cor %s) vs %s (DEFENSOR, cor %s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("✅ Ataque bem-sucedido! %s foi conquistado.\n", defensor->nome);
        // defensor muda de dono; repartição de tropas
        strcpy(defensor->cor, atacante->cor);
        int metade = atacante->tropas / 2;
        defensor->tropas = metade;
        atacante->tropas -= metade; // mantém a soma consistente
    } else {
        printf("❌ Defesa bem-sucedida! O atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    // Sorteia uma missão e copia para 'destino' (que deve ter espaço suficiente)
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

void exibirMissao(const char* nomeJogador, const char* missao) {
    printf("\n🎯 Missão do jogador %s:\n   %s\n", nomeJogador, missao);
}

/*
 Regras de verificação (lógicas simples baseadas no texto da missão):
 - "Conquistar pelo menos 3 territorios"
        => contar territórios com cor == g_corAtual >= 3
 - "Acumular pelo menos 15 tropas no total"
        => somar tropas de territórios com cor == g_corAtual >= 15
 - "Eliminar todas as tropas da cor vermelha"
        => nenhum território com cor "vermelha" e tropas > 0
 - "Controlar um territorio com pelo menos 8 tropas"
        => existe território da cor g_corAtual com tropas >= 8
 - "Reduzir tropas inimigas totais para menos de 10"
        => soma de tropas cuja cor != g_corAtual < 10
*/
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if (g_corAtual == NULL || missao == NULL || mapa == NULL) return 0;

    // Helpers
    int meusTerr = 0, minhasTropas = 0, inimigasTropas = 0;
    int tenhoUmCom8 = 0;
    int existeVermelhoComTropa = 0;

    for (int i = 0; i < tamanho; i++) {
        int souMeu = (strcmp(mapa[i].cor, g_corAtual) == 0);
        if (souMeu) {
            meusTerr++;
            minhasTropas += mapa[i].tropas;
            if (mapa[i].tropas >= 8) tenhoUmCom8 = 1;
        } else {
            inimigasTropas += mapa[i].tropas;
        }
        if (strcmp(mapa[i].cor, "vermelha") == 0 && mapa[i].tropas > 0) {
            existeVermelhoComTropa = 1;
        }
    }

    if (strstr(missao, "Conquistar pelo menos 3 territorios") != NULL) {
        return meusTerr >= 3;
    }
    if (strstr(missao, "Acumular pelo menos 15 tropas no total") != NULL) {
        return minhasTropas >= 15;
    }
    if (strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        return !existeVermelhoComTropa;
    }
    if (strstr(missao, "Controlar um territorio com pelo menos 8 tropas") != NULL) {
        return tenhoUmCom8;
    }
    if (strstr(missao, "Reduzir tropas inimigas totais para menos de 10") != NULL) {
        return inimigasTropas < 10;
    }

    // Caso padrão: missão desconhecida
    return 0;
}

void liberarMemoria(Territorio* mapa, Jogador* jogadores, int qtdJogadores) {
    if (mapa) free(mapa);
    if (jogadores) {
        for (int i = 0; i < qtdJogadores; i++) {
            free(jogadores[i].missao);
        }
        free(jogadores);
    }
}

/* =========================
   FUNÇÃO PRINCIPAL (main)
   ========================= */

int main(void) {
    srand((unsigned)time(NULL));

    // --- Vetor de missões pré-definidas ---
    char* missoes[] = {
        "Conquistar pelo menos 3 territorios",
        "Acumular pelo menos 15 tropas no total",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar um territorio com pelo menos 8 tropas",
        "Reduzir tropas inimigas totais para menos de 10"
    };
    const int TOTAL_MISSOES = (int)(sizeof(missoes)/sizeof(missoes[0]));

    // --- Jogadores (2 para simplificar o fluxo) ---
    int qtdJogadores = 2;
    Jogador* jogadores = (Jogador*) calloc(qtdJogadores, sizeof(Jogador));
    if (!jogadores) {
        printf("Erro ao alocar memoria para jogadores.\n");
        return 1;
    }

    for (int i = 0; i < qtdJogadores; i++) {
        printf("\n=== Cadastro do Jogador %d ===\n", i + 1);
        printf("Nome: ");
        scanf(" %19[^\n]", jogadores[i].nome);
        printf("Cor do exército (ex: vermelha, azul, verde...): ");
        scanf(" %9s", jogadores[i].cor);

        // Aloca a missão dinamicamente (tamanho fixo seguro para este exemplo)
        jogadores[i].missao = (char*) malloc(128 * sizeof(char));
        if (!jogadores[i].missao) {
            printf("Erro ao alocar memoria para missao do jogador.\n");
            liberarMemoria(NULL, jogadores, qtdJogadores);
            return 1;
        }
        atribuirMissao(jogadores[i].missao, missoes, TOTAL_MISSOES);
    }

    // Exibir missão apenas uma vez por jogador
    for (int i = 0; i < qtdJogadores; i++) {
        exibirMissao(jogadores[i].nome, jogadores[i].missao);
    }

    // --- Territórios (alocação dinâmica) ---
    int n;
    printf("\nQuantos territorios deseja cadastrar? ");
    scanf("%d", &n);

    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memoria para territorios.\n");
        liberarMemoria(NULL, jogadores, qtdJogadores);
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirMapa(mapa, n);

    // --- Loop de turnos ---
    int vencedor = -1;
    int turno = 0;

    while (vencedor == -1) {
        int iJog = turno % qtdJogadores;
        Jogador* j = &jogadores[iJog];

        printf("\n===== Turno de %s (cor %s) =====\n", j->nome, j->cor);
        printf("1 - Exibir mapa\n");
        printf("2 - Atacar\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        int opcao;
        if (scanf("%d", &opcao) != 1) opcao = 0;

        if (opcao == 1) {
            exibirMapa(mapa, n);
        } else if (opcao == 2) {
            exibirMapa(mapa, n);

            int iAtacante, iDefensor;
            printf("Índice do território ATACANTE: ");
            scanf("%d", &iAtacante);
            printf("Índice do território DEFENSOR: ");
            scanf("%d", &iDefensor);

            if (iAtacante < 0 || iAtacante >= n || iDefensor < 0 || iDefensor >= n) {
                printf("⚠️  Índices inválidos.\n");
            } else if (strcmp(mapa[iAtacante].cor, j->cor) != 0) {
                printf("⚠️  Você só pode atacar a partir de um território da sua cor (%s).\n", j->cor);
            } else if (strcmp(mapa[iDefensor].cor, j->cor) == 0) {
                printf("⚠️  Você não pode atacar um território aliado.\n");
            } else {
                atacar(&mapa[iAtacante], &mapa[iDefensor]);
                exibirMapa(mapa, n);
            }
        } else if (opcao == 0) {
            printf("Saindo do jogo...\n");
            break;
        } else {
            printf("Opção inválida.\n");
        }

        // --- Verificação silenciosa de vitória após o turno ---
        for (int i = 0; i < qtdJogadores; i++) {
            g_corAtual = jogadores[i].cor; // define a cor do jogador em verificação
            if (verificarMissao(jogadores[i].missao, mapa, n)) {
                vencedor = i;
                break;
            }
        }
        g_corAtual = NULL;

        if (vencedor != -1) {
            printf("\n🏆 Vitória! O jogador %s (cor %s) cumpriu a missão:\n   \"%s\"\n",
                   jogadores[vencedor].nome, jogadores[vencedor].cor, jogadores[vencedor].missao);
            break;
        }

        turno++;
    }

    // Libera toda memória dinâmica
    liberarMemoria(mapa, jogadores, qtdJogadores);
    return 0;
}
