#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5  // Tamanho fixo da fila

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único
} Peca;

// Estrutura que representa a fila circular
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente; // Índice da primeira peça
    int tras;   // Índice da última peça
    int qtd;    // Quantidade de peças atuais na fila
} Fila;

// ------------------- Funções de Fila -------------------

// Inicializa a fila vazia
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->qtd == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->qtd == 0;
}

// Gera automaticamente uma nova peça aleatória
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4]; // Escolhe aleatoriamente entre I, O, T ou L
    nova.id = id;
    return nova;
}

// Insere uma nova peça ao final da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n A fila está cheia! Não é possível inserir nova peça.\n");
        return;
    }
    f->tras = (f->tras + 1) % TAMANHO_FILA;
    f->pecas[f->tras] = p;
    f->qtd++;
}

// Remove uma peça da frente da fila (dequeue)
void desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("\nA fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }
    Peca removida = f->pecas[f->frente];
    printf("\nJogando peça: [%c %d]\n", removida.nome, removida.id);
    f->frente = (f->frente + 1) % TAMANHO_FILA;
    f->qtd--;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("\nFila de peças:\n");
    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }

    int i, idx;
    for (i = 0; i < f->qtd; i++) {
        idx = (f->frente + i) % TAMANHO_FILA;
        printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
    }
    printf("\n");
}

// ------------------- Função Principal -------------------

int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL));

    int idAtual = 0; // Contador de IDs únicos
    int opcao;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(idAtual++));
    }

    do {
        exibirFila(&fila);

        printf("\n--- MENU ---\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&fila);
                break;
            case 2:
                enfileirar(&fila, gerarPeca(idAtual++));
                break;
            case 0:
                printf("\nSaindo do jogo... \n");
                break;
            default:
                printf("\nOpção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
