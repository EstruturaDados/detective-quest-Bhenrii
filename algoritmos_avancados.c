#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==================== DEFINIÇÕES DE ESTRUTURAS ====================

// Estrutura para uma sala na árvore binária
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para um nó na BST de pistas
typedef struct PistaBST {
    char pista[100];
    struct PistaBST *esquerda;
    struct PistaBST *direita;
} PistaBST;

// Estrutura para um nó na tabela hash
typedef struct NoHash {
    char pista[100];
    char suspeito[50];
    struct NoHash *proximo;
} NoHash;

// ==================== DEFINIÇÕES GLOBAIS ====================

#define TAMANHO_HASH 10
NoHash* tabelaHash[TAMANHO_HASH];
PistaBST* arvorePistas = NULL;

// Lista de suspeitos
char* suspeitos[] = {"Carlos", "Ana", "Pedro", "Mariana", "João"};
int totalSuspeitos = 5;

// ==================== FUNÇÕES DA ÁRVORE BINÁRIA (MANSÃO) ====================

/**
 * Função: criarSala
 * Cria dinamicamente um novo cômodo da mansão
 * Parâmetros: nome da sala e pista associada
 * Retorno: ponteiro para a sala criada
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * Função: construirMansao
 * Constrói o mapa fixo da mansão com salas e pistas
 * Retorno: ponteiro para a raiz da árvore
 */
Sala* construirMansao() {
    // Criar salas com pistas pré-definidas
    Sala* entrada = criarSala("Entrada", "Uma carta rasgada com a letra 'C'");
    Sala* salaEstar = criarSala("Sala de Estar", "Um copo com batom vermelho");
    Sala* cozinha = criarSala("Cozinha", "Uma faca manchada");
    Sala* biblioteca = criarSala("Biblioteca", "Um livro sobre venenos aberto");
    Sala* quarto = criarSala("Quarto Principal", "Um relógio parado às 21:30");
    Sala* escritorio = criarSala("Escritorio", "Um computador com email aberto");
    Sala* jardim = criarSala("Jardim", "Uma pegada de barro");
    
    // Montar a estrutura da mansão
    entrada->esquerda = salaEstar;
    entrada->direita = cozinha;
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = quarto;
    
    cozinha->esquerda = escritorio;
    cozinha->direita = jardim;
    
    return entrada;
}

// ==================== FUNÇÕES DA BST (PISTAS) ====================

/**
 * Função: inserirPista
 * Insere uma nova pista na árvore BST de forma ordenada
 * Parâmetros: raiz da BST e pista a ser inserida
 * Retorno: ponteiro para a raiz atualizada
 */
PistaBST* inserirPista(PistaBST* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaBST* novaPista = (PistaBST*)malloc(sizeof(PistaBST));
        if (novaPista == NULL) {
            printf("Erro de alocação de memória!\n");
            exit(1);
        }
        strcpy(novaPista->pista, pista);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        return novaPista;
    }
    
    int comparacao = strcmp(pista, raiz->pista);
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    
    return raiz;
}

/**
 * Função: exibirPistasOrdenadas
 * Exibe todas as pistas coletadas em ordem (percurso in-order)
 * Parâmetros: raiz da BST
 */
void exibirPistasOrdenadas(PistaBST* raiz) {
    if (raiz != NULL) {
        exibirPistasOrdenadas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistasOrdenadas(raiz->direita);
    }
}

// ==================== FUNÇÕES DA TABELA HASH ====================

/**
 * Função: funcaoHash
 * Calcula o índice hash para uma pista
 * Parâmetros: pista a ser hasheada
 * Retorno: índice na tabela hash
 */
int funcaoHash(const char* pista) {
    int hash = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        hash = (hash * 31 + pista[i]) % TAMANHO_HASH;
    }
    return hash;
}

/**
 * Função: inserirNaHash
 * Insere uma associação pista-suspeito na tabela hash
 * Parâmetros: pista e suspeito correspondente
 */
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = funcaoHash(pista);
    
    NoHash* novoNo = (NoHash*)malloc(sizeof(NoHash));
    if (novoNo == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = NULL;
    
    // Inserir no início da lista encadeada
    if (tabelaHash[indice] == NULL) {
        tabelaHash[indice] = novoNo;
    } else {
        novoNo->proximo = tabelaHash[indice];
        tabelaHash[indice] = novoNo;
    }
}

/**
 * Função: encontrarSuspeito
 * Consulta o suspeito correspondente a uma pista na tabela hash
 * Parâmetros: pista a ser consultada
 * Retorno: nome do suspeito ou NULL se não encontrado
 */
char* encontrarSuspeito(const char* pista) {
    int indice = funcaoHash(pista);
    NoHash* atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

/**
 * Função: inicializarTabelaHash
 * Inicializa a tabela hash com associações pista-suspeito pré-definidas
 */
void inicializarTabelaHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
    
    // Definir associações fixas pista -> suspeito
    inserirNaHash("Uma carta rasgada com a letra 'C'", "Carlos");
    inserirNaHash("Um copo com batom vermelho", "Ana");
    inserirNaHash("Uma faca manchada", "Pedro");
    inserirNaHash("Um livro sobre venenos aberto", "Mariana");
    inserirNaHash("Um relógio parado às 21:30", "João");
    inserirNaHash("Um computador com email aberto", "Carlos");
    inserirNaHash("Uma pegada de barro", "Pedro");
}

// ==================== FUNÇÕES DO JOGO ====================

/**
 * Função: explorarSalas
 * Controla a navegação do jogador pela mansão e coleta de pistas
 * Parâmetros: sala atual onde o jogador está
 */
void explorarSalas(Sala* salaAtual) {
    if (salaAtual == NULL) {
        printf("Fim da exploração!\n");
        return;
    }
    
    printf("\n=== VOCÊ ESTÁ NA: %s ===\n", salaAtual->nome);
    
    // Verificar se há pista na sala e coletá-la
    if (strlen(salaAtual->pista) > 0) {
        printf("🔍 Você encontrou uma pista: %s\n", salaAtual->pista);
        
        // Inserir pista na BST
        arvorePistas = inserirPista(arvorePistas, salaAtual->pista);
        printf("✅ Pista adicionada à sua coleção!\n");
    }
    
    // Opções de navegação
    printf("\nOpções de navegação:\n");
    if (salaAtual->esquerda != NULL) {
        printf("(e) - Ir para %s (esquerda)\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("(d) - Ir para %s (direita)\n", salaAtual->direita->nome);
    }
    printf("(s) - Sair da exploração e fazer acusação\n");
    
    char escolha;
    printf("\nSua escolha: ");
    scanf(" %c", &escolha);
    
    switch (escolha) {
        case 'e':
            explorarSalas(salaAtual->esquerda);
            break;
        case 'd':
            explorarSalas(salaAtual->direita);
            break;
        case 's':
            printf("\n🏃 Saindo da exploração...\n");
            break;
        default:
            printf("❌ Opção inválida! Tente novamente.\n");
            explorarSalas(salaAtual);
            break;
    }
}

/**
 * Função: contarPistasPorSuspeito
 * Conta quantas pistas apontam para um suspeito específico
 * Parâmetros: suspeito a ser verificado
 * Retorno: número de pistas que apontam para o suspeito
 */
int contarPistasPorSuspeito(const char* suspeito) {
    int contador = 0;
    
    // Função auxiliar para percorrer a BST e contar pistas do suspeito
    void contarRecursivo(PistaBST* raiz) {
        if (raiz != NULL) {
            contarRecursivo(raiz->esquerda);
            char* suspeitoDaPista = encontrarSuspeito(raiz->pista);
            if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, suspeito) == 0) {
                contador++;
            }
            contarRecursivo(raiz->direita);
        }
    }
    
    contarRecursivo(arvorePistas);
    return contador;
}

/**
 * Função: verificarSuspeitoFinal
 * Conduz a fase final de julgamento e verifica a acusação
 */
void verificarSuspeitoFinal() {
    printf("\n" "🎭" "========== FASE DE ACUSAÇÃO ==========\n");
    
    // Exibir pistas coletadas
    printf("\n📋 Pistas coletadas (em ordem):\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada!\n");
    } else {
        exibirPistasOrdenadas(arvorePistas);
    }
    
    // Exibir suspeitos disponíveis
    printf("\n🔎 Lista de Suspeitos:\n");
    for (int i = 0; i < totalSuspeitos; i++) {
        printf("%d. %s\n", i + 1, suspeitos[i]);
    }
    
    // Receber acusação do jogador
    int acusacao;
    printf("\n👮 Quem é o culpado? (Digite o número): ");
    scanf("%d", &acusacao);
    
    if (acusacao < 1 || acusacao > totalSuspeitos) {
        printf("❌ Acusação inválida!\n");
        return;
    }
    
    char* suspeitoAcusado = suspeitos[acusacao - 1];
    printf("\n" "⚖️" " Você acusou: %s\n", suspeitoAcusado);
    
    // Verificar se há pistas suficientes
    int pistasDoSuspeito = contarPistasPorSuspeito(suspeitoAcusado);
    printf("🔍 Pistas que apontam para %s: %d\n", suspeitoAcusado, pistasDoSuspeito);
    
    // Determinar o resultado final
    if (pistasDoSuspeito >= 2) {
        printf("\n" "🎉" " PARABÉNS! A acusação está correta!\n");
        printf("Há evidências suficientes para prender %s!\n", suspeitoAcusado);
    } else {
        printf("\n" "💔" " ACUSAÇÃO INCORRETA!\n");
        printf("Não há pistas suficientes para sustentar a acusação contra %s.\n", suspeitoAcusado);
        printf("O verdadeiro culpado escapou...\n");
    }
}

/**
 * Função: liberarMemoria
 * Libera toda a memória alocada dinamicamente
 * Parâmetros: raiz da árvore da mansão
 */
void liberarMemoria(Sala* raiz) {
    if (raiz != NULL) {
        liberarMemoria(raiz->esquerda);
        liberarMemoria(raiz->direita);
        free(raiz);
    }
}

void liberarBST(PistaBST* raiz) {
    if (raiz != NULL) {
        liberarBST(raiz->esquerda);
        liberarBST(raiz->direita);
        free(raiz);
    }
}

void liberarTabelaHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash* atual = tabelaHash[i];
        while (atual != NULL) {
            NoHash* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

// ==================== FUNÇÃO PRINCIPAL ====================

int main() {
    printf("🕵️‍♂️" " BEM-VINDO AO DETECTIVE QUEST! 🕵️‍♂️\n");
    printf("========================================\n");
    printf("Explore a mansão, colete pistas e descubra o culpado!\n\n");
    
    // Inicializar sistemas
    inicializarTabelaHash();
    Sala* mansao = construirMansao();
    
    // Iniciar exploração
    explorarSalas(mansao);
    
    // Fase final de acusação
    verificarSuspeitoFinal();
    
    // Liberar memória
    liberarMemoria(mansao);
    liberarBST(arvorePistas);
    liberarTabelaHash();
    
    printf("\n" "👋" " Obrigado por jogar Detective Quest!\n");
    
    return 0;
}