#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5     // Tamanho fixo da fila circular
#define TAM_PILHA 3    // Capacidade máxima da pilha de reserva

// -------------------- Estrutura de Peça --------------------
typedef struct {
    char tipo;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// -------------------- Variáveis globais --------------------
int contadorID = 0; // Contador global para gerar IDs únicos

// -------------------- Função para gerar peças --------------------
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4]; // tipo aleatório
    nova.id = contadorID++;        // id único
    return nova;
}

// -------------------- Estrutura e funções da Fila Circular --------------------
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = p;
    f->qtd++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'?', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return removida;
}

// -------------------- Estrutura e funções da Pilha --------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->itens[++p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'?', -1};
    if (pilhaVazia(p)) return removida;
    removida = p->itens[p->topo--];
    return removida;
}

// -------------------- Exibir estado --------------------
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n==================== ESTADO ATUAL ====================\n");
    printf("Fila de peças\t: ");
    if (filaVazia(fila)) {
        printf("(vazia)");
    } else {
        int i, index = fila->frente;
        for (i = 0; i < fila->qtd; i++) {
            Peca p = fila->itens[index];
            printf("[%c %d] ", p.tipo, p.id);
            index = (index + 1) % TAM_FILA;
        }
    }
    printf("\n");

    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("(vazia)");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].tipo, pilha->itens[i].id);
        }
    }
    printf("\n======================================================\n");
}

// -------------------- Programa Principal --------------------
int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\nOpções de Ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1: {
                if (filaVazia(&fila)) {
                    printf("Fila vazia! Não há peça para jogar.\n");
                } else {
                    Peca jogada = desenfileirar(&fila);
                    printf("Você jogou a peça [%c %d].\n", jogada.tipo, jogada.id);
                    enfileirar(&fila, gerarPeca());
                    exibirEstado(&fila, &pilha);
                }
                break;
            }

            case 2: {
                if (filaVazia(&fila)) {
                    printf("Fila vazia! Não há peça para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("Pilha cheia! Não é possível reservar mais peças.\n");
                } else {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("Peça [%c %d] movida para a reserva.\n", reservada.tipo, reservada.id);
                    enfileirar(&fila, gerarPeca());
                    exibirEstado(&fila, &pilha);
                }
                break;
            }

            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("Nenhuma peça reservada para usar.\n");
                } else {
                    Peca usada = desempilhar(&pilha);
                    printf("Você usou a peça reservada [%c %d].\n", usada.tipo, usada.id);
                    exibirEstado(&fila, &pilha);
                }
                break;
            }

            case 0:
                printf("Encerrando o jogo...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
