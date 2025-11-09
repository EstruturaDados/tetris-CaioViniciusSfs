#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ---------------------- ESTRUTURAS ----------------------
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} FilaCircular;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// ---------------------- VARIÁVEIS GLOBAIS ----------------------
int contadorID = 0;

// ---------------------- FUNÇÕES AUXILIARES ----------------------
char gerarTipoAleatorio() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    return tipos[rand() % 4];
}

Peca gerarPeca() {
    Peca nova;
    nova.nome = gerarTipoAleatorio();
    nova.id = contadorID++;
    return nova;
}

// ---------------------- FUNÇÕES FILA ----------------------
void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

int filaCheia(FilaCircular *f) {
    return f->qtd == TAM_FILA;
}

int filaVazia(FilaCircular *f) {
    return f->qtd == 0;
}

void enfileirar(FilaCircular *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->pecas[f->tras] = p;
    f->qtd++;
}

Peca desenfileirar(FilaCircular *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return removida;
}

// ---------------------- FUNÇÕES PILHA ----------------------
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
    p->pecas[++p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) return removida;
    return p->pecas[p->topo--];
}

Peca topoPilha(Pilha *p) {
    Peca vazia = {'-', -1};
    if (pilhaVazia(p)) return vazia;
    return p->pecas[p->topo];
}

// ---------------------- FUNÇÕES DE EXIBIÇÃO ----------------------
void exibirEstado(FilaCircular *fila, Pilha *pilha) {
    printf("\n================= ESTADO ATUAL =================\n");

    printf("Fila de peças\t");
    int i, idx;
    for (i = 0; i < fila->qtd; i++) {
        idx = (fila->frente + i) % TAM_FILA;
        printf("[%c %d] ", fila->pecas[idx].nome, fila->pecas[idx].id);
    }

    printf("\nPilha de reserva\t(Topo -> base): ");
    for (i = pilha->topo; i >= 0; i--) {
        printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
    }

    printf("\n================================================\n");
}

// ---------------------- FUNÇÕES DE AÇÃO ----------------------
void jogarPeca(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("\nFila vazia! Não há peça para jogar.\n");
        return;
    }
    Peca jogada = desenfileirar(fila);
    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
    // Mantém fila cheia
    if (!filaCheia(fila)) {
        enfileirar(fila, gerarPeca());
    }
}

void reservarPeca(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\nFila vazia! Nenhuma peça para reservar.\n");
        return;
    }
    if (pilhaCheia(pilha)) {
        printf("\nPilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    Peca reservada = desenfileirar(fila);
    empilhar(pilha, reservada);
    printf("\nPeça [%c %d] movida da fila para a pilha.\n", reservada.nome, reservada.id);
    // Mantém fila cheia
    if (!filaCheia(fila)) {
        enfileirar(fila, gerarPeca());
    }
}

void usarPecaReservada(Pilha *pilha) {
    if (pilhaVazia(pilha)) {
        printf("\nPilha vazia! Nenhuma peça reservada para usar.\n");
        return;
    }
    Peca usada = desempilhar(pilha);
    printf("\nPeça reservada usada: [%c %d]\n", usada.nome, usada.id);
}

void trocarTopo(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("\nImpossível trocar — uma das estruturas está vazia.\n");
        return;
    }
    int frente = fila->frente;
    Peca temp = fila->pecas[frente];
    fila->pecas[frente] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;
    printf("\nTroca entre peça da fila e topo da pilha realizada com sucesso!\n");
}

void trocarMultiplas(FilaCircular *fila, Pilha *pilha) {
    if (fila->qtd < 3 || pilha->topo < 2) {
        printf("\nNão há peças suficientes para troca múltipla!\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int idxFila = (fila->frente + i) % TAM_FILA;
        Peca temp = fila->pecas[idxFila];
        fila->pecas[idxFila] = pilha->pecas[pilha->topo - i];
        pilha->pecas[pilha->topo - i] = temp;
    }
    printf("\nTroca múltipla realizada entre as 3 primeiras peças da fila e as 3 da pilha!\n");
}

// ---------------------- PROGRAMA PRINCIPAL ----------------------
int main() {
    srand(time(NULL));
    FilaCircular fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: jogarPeca(&fila); break;
            case 2: reservarPeca(&fila, &pilha); break;
            case 3: usarPecaReservada(&pilha); break;
            case 4: trocarTopo(&fila, &pilha); break;
            case 5: trocarMultiplas(&fila, &pilha); break;
            case 0: printf("\nEncerrando programa...\n"); break;
            default: printf("\nOpção inválida!\n"); break;
        }
    } while (opcao != 0);

    return 0;
}
