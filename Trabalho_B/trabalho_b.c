#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 50

typedef struct {
    int protocolo;
    char local[50];
    char tipo[30];
    char horario[20]; 
} Chamada;

void menu();

int push(Chamada pilha[], int *topo, Chamada nova);
int pop(Chamada pilha[], int *topo, Chamada *removida);
int peek(Chamada pilha[], int topo);
int protocoloExiste(Chamada pilha[], int topo, int protocolo);
void listar(Chamada pilha[], int topo);
void salvarCSV(Chamada chamada);
void criarCSV();

int main() {

    criarCSV();
    Chamada pilha[MAX];
    Chamada aux;

    int topo = -1;
    int opcao;

    do {

        menu();
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {

            case 1:

                printf("\n=== REGISTRAR CHAMADA ===\n");

                printf("Protocolo: ");
                scanf("%d", &aux.protocolo);
                getchar();

                if(protocoloExiste(pilha, topo, aux.protocolo)) {
                    printf("ERRO: Protocolo ja cadastrado!\n");
                    break;
                }

                printf("Local: ");
                fgets(aux.local, 50, stdin);
                aux.local[strcspn(aux.local, "\n")] = '\0';

                printf("Tipo da ocorrencia: ");
                fgets(aux.tipo, 30, stdin);
                aux.tipo[strcspn(aux.tipo, "\n")] = '\0';

                printf("Horario: ");
                fgets(aux.horario, 20, stdin);
                aux.horario[strcspn(aux.horario, "\n")] = '\0';

                push(pilha, &topo, aux);

                break;

            case 2:

                if(pop(pilha, &topo, &aux) == 0) {

                    printf("\n=== CHAMADA ATENDIDA ===\n");
                    printf("Protocolo: %d\n", aux.protocolo);
                    printf("Local: %s\n", aux.local);
                    printf("Tipo: %s\n", aux.tipo);
                    printf("Horario: %s\n", aux.horario);
                }

                break;

            case 3:

                peek(pilha, topo);

                break;

            case 4:

                listar(pilha, topo);

                break;

            case 0:

                printf("Encerrando sistema...\n");
                break;

            default:

                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);

    return 0;
}

void menu() {

    system("clear");
    printf("\n=====================================\n");
    printf(" PILHA DE CHAMADAS DE EMERGENCIA\n");
    printf("=====================================\n");
    printf("1 - Registrar chamada (Push)\n");
    printf("2 - Atender chamada (Pop)\n");
    printf("3 - Consultar ultima chamada (Peek)\n");
    printf("4 - Listar pilha\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
}

int protocoloExiste(Chamada pilha[], int topo, int protocolo) {

    int i;

    for(i = 0; i <= topo; i++) {

        if(pilha[i].protocolo == protocolo) {
            return 1;
        }
    }

    return 0;
}

int push(Chamada pilha[], int *topo, Chamada nova) {
    system("clear");
    if(*topo >= MAX - 1) {

        printf("ERRO: Pilha cheia!\n");
        sleep(5);
        return 1;
    }

    *topo = *topo + 1;

    pilha[*topo] = nova;
    
    salvarCSV(nova);

    printf("Chamada registrada com sucesso!\n");
    sleep(5);

    return 0;
}

int pop(Chamada pilha[], int *topo, Chamada *removida) {
    system("clear");
    if(*topo < 0) {

        printf("ERRO: Pilha vazia!\n");
        sleep(5);
        return 1;
    }

    *removida = pilha[*topo];

    *topo = *topo - 1;

    return 0;
}

int peek(Chamada pilha[], int topo) {
    system("clear");
    if(topo < 0) {

        printf("ERRO: Pilha vazia!\n");
        sleep(5);
        return 1;
    }

    printf("\n=== ULTIMA CHAMADA REGISTRADA ===\n");
    printf("Protocolo: %d\n", pilha[topo].protocolo);
    printf("Local: %s\n", pilha[topo].local);
    printf("Tipo: %s\n", pilha[topo].tipo);
    printf("Horario: %s\n", pilha[topo].horario);
    sleep(5);
    return 0;
}

void listar(Chamada pilha[], int topo) {

    int i;
    system("clear");
    if(topo < 0) {

        printf("ERRO: Pilha vazia!\n");
        sleep(5);
        return;
    }

    printf("\n=== PILHA DE CHAMADAS ===\n");

    for(i = topo; i >= 0; i--) {

        printf("\nProtocolo: %d\n", pilha[i].protocolo);
        printf("Local: %s\n", pilha[i].local);
        printf("Tipo: %s\n", pilha[i].tipo);
        printf("Horario: %s\n", pilha[i].horario);
    }
    sleep(5);
}


void salvarCSV(Chamada chamada) {

    FILE *arquivo;

    arquivo = fopen("chamadas.csv", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(arquivo, "%d,%s,%s,%s\n",
            chamada.protocolo,
            chamada.local,
            chamada.tipo,
            chamada.horario);

    fclose(arquivo);
}

void criarCSV() {

    FILE *arquivo;

    arquivo = fopen("chamadas.csv", "r");

    if (arquivo == NULL) {

        arquivo = fopen("chamadas.csv", "w");

        fprintf(arquivo,
                "Protocolo,Local,Tipo,Horario\n");

        fclose(arquivo);
    }
    else {
        fclose(arquivo);
    }
}
