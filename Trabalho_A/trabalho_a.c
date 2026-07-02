// João Pedro Fernandes Rodrigues, Matheus Barbosa de Jesus Santos , Victor Hugo Ferreira Costa
// Turma C
// Trabalho A
#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    int id;
    char titulo[50];
    char artista[50];
    int duracao;
    char genero[30];
} Musica;

Musica lista[MAX];
int quantidade = 0;

void inserir();
void buscar();
void editar();
void excluir();
void listar();
void salvarCSV();
void carregarCSV();

int main() {
    int opcao;

    carregarCSV();

    do {
        printf("\n|====== PLAYLIST =======|\n");
        printf("|1. Inserir musica      |\n");
        printf("|2. Buscar por ID       |\n");
        printf("|3. Editar informacoes  |\n");
        printf("|4. Excluir musica      |\n");
        printf("|5. Listar playlist     |\n");
        printf("|6. Sair                |\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1: inserir(); break;
            case 2: buscar(); break;
            case 3: editar(); break;
            case 4: excluir(); break;
            case 5: listar(); break;
            case 6:
                printf("Salvando dados e saindo...\n");
                salvarCSV();
                break;
            default: printf("Opcao invalida!\n");
        }
    } while(opcao != 6);

    return 0;
}

void inserir() {
    if (quantidade >= MAX) {
        printf("Erro: Playlist cheia!\n");
        return;
    }

    int novoId;
    printf("\nID: ");
    scanf("%d", &novoId);
    getchar();

    for (int i = 0; i < quantidade; i++) {
        if (lista[i].id == novoId) {
            printf("Erro: O ID ja existe!\n");
            return;
        }
    }

    lista[quantidade].id = novoId;

    printf("Titulo: ");
    fgets(lista[quantidade].titulo, 50, stdin);
    lista[quantidade].titulo[strcspn(lista[quantidade].titulo, "\n")] = '\0';

    printf("Artista: ");
    fgets(lista[quantidade].artista, 50, stdin);
    lista[quantidade].artista[strcspn(lista[quantidade].artista, "\n")] = '\0';

    printf("Duracao (segundos): ");
    scanf("%d", &lista[quantidade].duracao);
    getchar();

    printf("Genero: ");
    fgets(lista[quantidade].genero, 30, stdin);
    lista[quantidade].genero[strcspn(lista[quantidade].genero, "\n")] = '\0';

    quantidade++;
    printf("Musica cadastrada!\n");
}

void buscar() {
    if (quantidade == 0) {
        printf("Playlist vazia!\n");
        return;
    }

    int idProcurado;
    printf("\nDigite o ID para buscar: ");
    scanf("%d", &idProcurado);
    getchar();

    for (int i = 0; i < quantidade; i++) {
        if (lista[i].id == idProcurado) {
            printf("ID: %d | %s - %s (%ds) | %s\n",
                   lista[i].id, lista[i].titulo, lista[i].artista, lista[i].duracao, lista[i].genero);
            return;
        }
    }
    printf("Musica nao encontrada!\n");
}

void editar() {
    if (quantidade == 0) {
        printf("Playlist vazia!\n");
        return;
    }

    int idProcurado;
    printf("\nDigite o ID da musica que deseja editar: ");
    scanf("%d", &idProcurado);
    getchar();

    for (int i = 0; i < quantidade; i++) {
        if (lista[i].id == idProcurado) {
            printf("Novo Titulo: ");
            fgets(lista[i].titulo, 50, stdin);
            lista[i].titulo[strcspn(lista[i].titulo, "\n")] = '\0';

            printf("Novo Artista: ");
            fgets(lista[i].artista, 50, stdin);
            lista[i].artista[strcspn(lista[i].artista, "\n")] = '\0';

            printf("Nova Duracao (segundos): ");
            scanf("%d", &lista[i].duracao);
            getchar();

            printf("Novo Genero: ");
            fgets(lista[i].genero, 30, stdin);
            lista[i].genero[strcspn(lista[i].genero, "\n")] = '\0';

            printf("Musica atualizada!\n");
            return;
        }
    }
    printf("Musica nao encontrada!\n");
}

void excluir() {
    if (quantidade == 0) {
        printf("Playlist vazia!\n");
        return;
    }

    int idProcurado;
    printf("\nDigite o ID da musica para excluir: ");
    scanf("%d", &idProcurado);
    getchar();

    for (int i = 0; i < quantidade; i++) {
        if (lista[i].id == idProcurado) {
            for (int j = i; j < quantidade - 1; j++) {
                lista[j] = lista[j + 1];
            }
            quantidade--;
            printf("Musica excluida e playlist reorganizada!\n");
            return;
        }
    }
    printf("Musica nao encontrada!\n");
}

void listar() {
    if (quantidade == 0) {
        printf("Playlist vazia!\n");
        return;
    }

    printf("\n--- MÚSICAS ARMAZENADAS ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("ID: %d | %s - %s (%ds) | Genero: %s\n",
               lista[i].id, lista[i].titulo, lista[i].artista, lista[i].duracao, lista[i].genero);
    }
}


void salvarCSV() {
    FILE *arquivo = fopen("playlist.csv", "w");
    if (arquivo == NULL) return;

    for (int i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%d;%s;%s;%d;%s\n",
                lista[i].id, lista[i].titulo, lista[i].artista, lista[i].duracao, lista[i].genero);
    }

    fclose(arquivo);
}

void carregarCSV() {
    FILE *arquivo = fopen("playlist.csv", "r");
    if (arquivo == NULL) return;

    quantidade = 0;
    while (quantidade < MAX &&
           fscanf(arquivo, "%d;%[^;];%[^;];%d;%[^\n]\n",
                  &lista[quantidade].id,
                  lista[quantidade].titulo,
                  lista[quantidade].artista,
                  &lista[quantidade].duracao,
                  lista[quantidade].genero) == 5) {
        quantidade++;
    }

    fclose(arquivo);
}
