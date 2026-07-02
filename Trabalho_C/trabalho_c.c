// João Pedro Fernandes Rodrigues, Matheus Barbosa de Jesus Santos , Victor Hugo Ferreira Costa
// Turma C
// Trabalho C
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#define MAX 10

struct Cliente { // struct do cliente do banco que está na fila
    int senha; // senha para chamar o cliente
    char tipo; // tipo normal (N) ou preferencial (P), usará tolower na hora de pegar entrada do usuário
    char horario[20]; // horário em que o cliente chegou no banco
};

struct FilaNormal { // struct para representar a fila dos clientes que nao sao preferenciais
    struct Cliente vetor[MAX]; // lista que possui os clientes que estão na fila
    int primeiro; // variavel para indicar o primeiro cliente da fila,
    int ultimo; // variavel para indicar o ultimo cliente da fila
};

void initNormal(struct FilaNormal *s) { // função para inicializar as variaveis da fila normal
    s->primeiro = 0;
    s->ultimo = -1;
}

struct FilaPreferencial { // fila para clientes preferenciais. !Fila Circular!
    struct Cliente vetor[MAX]; // mesmas variaveis da outra fila
    int primeiro;
    int quantidadeClientes; // quantidade de clientes na fila
};

void initPrefencial(struct FilaPreferencial *s) { // função para inicializar variaveis da fila preferencial
    s->primeiro = 0;
    s->quantidadeClientes = 0;
}

void salvarCSV(struct FilaNormal filaN, struct FilaPreferencial filaP) { // função para salvar em arquivo separado
    FILE *arquivo = fopen("filas_banco.csv", "w"); // abre (ou cria) arquivo para salvamento
    if (arquivo == NULL) { // erro para se o arquivo estiver vazio
        printf("Erro ao abrir o arquivo para salvar os dados!\n");
        return;
    }

    // Salva os clientes da Fila Normal
    if (!(filaN.primeiro > filaN.ultimo || filaN.primeiro >= MAX)) { // se a fila não estiver vazia...
        for (int i = filaN.primeiro; i <= filaN.ultimo; i++) {
            fprintf(arquivo, "%d;%c;%s\n", filaN.vetor[i].senha, filaN.vetor[i].tipo, filaN.vetor[i].horario); // atravez de um fprintf
        }
    }

    // Salva os clientes da Fila Preferencial
    if (filaP.quantidadeClientes > 0) { // se a fila não estiver vazia...
        int j = filaP.primeiro; // variavel para ser o indice
        for (int i = 0; i < filaP.quantidadeClientes; i++) {
            fprintf(arquivo, "%d;%c;%s\n", filaP.vetor[j].senha, filaP.vetor[j].tipo, filaP.vetor[j].horario); // atravez de um fprintf
            j = (j + 1) % MAX; // calculo para fazer com que o indice funcione na fila circular
        }
    }

    fclose(arquivo); // fecha o salvamento do arquivo
    printf("Dados salvos com sucesso em 'filas_banco.csv'!\n");
}

void enfileirarNormal(struct FilaNormal *fila, struct Cliente cliente);
void enfileirarPreferencial(struct FilaPreferencial *fila, struct Cliente cliente);

void carregarCSV(struct FilaNormal *filaN, struct FilaPreferencial *filaP) { // função para carregar de arquivo separado
    FILE *arquivo = fopen("filas_banco.csv", "r"); // abre arquivo para carregar
    if (arquivo == NULL) {
        // Se o arquivo não existir, apenas ignora (primeira execução)
        return;
    }

    struct Cliente c; // cliente temporario para carregar
    // Lê o arquivo linha por linha usando o ponto e vírgula (;) como separador
    while (fscanf(arquivo, "%d;%c;%19[^\n]\n", &c.senha, &c.tipo, c.horario) == 3) {
        if (c.tipo == 'N') {
            enfileirarNormal(filaN, c);
        } else if (c.tipo == 'P') {
            enfileirarPreferencial(filaP, c);
        }
    }

    fclose(arquivo); // fecha arquivo
    printf("Dados carregados com sucesso do arquivo CSV!\n");
}

void menuInicial() { // funcao de mensagem que aparece inicialmente ao executar o codigo
    printf("Bem vindo ao banco! Insira um número para selecionar uma opção: \n");
    printf("1 - Inserir um cliente na fila \n");
    printf("2 - Mostrar as filas \n");
    printf("3 - Buscar por senha \n");
    printf("4 - Atender um cliente \n"); // atender 2 normais para cada preferencial
    printf("5 - Cancelar senha \n");
    printf("6 - Sair\n");
}

void menuInsercao() { // funcao de mensagem que aparece ao inserir um cliente na fila
    printf("Você é um cliente preferencial?: \n");
    printf("1 - Sim \n");
    printf("2 - Não \n");
    printf("3 - Sair \n");
}

void enfileirarNormal(struct FilaNormal *fila, struct Cliente cliente) { // função de enfileirar da fila normal
    if(fila->ultimo >= MAX - 1) { // se a quantidade de clientes exceder o maximo a fila está cheia
        printf("Fila está cheia... \n");
        return; // então retorna a função main
    }

    (fila->ultimo)++; // adiciona 1 ao contador do final da fila para indicar que entrou um cliente na fila
    fila->vetor[fila->ultimo] = cliente; // se insere o cliente na nova posição
}

void enfileirarPreferencial(struct FilaPreferencial *fila, struct Cliente cliente) { // função de enfileirar da fila preferencial
    if(fila->quantidadeClientes == MAX) { // se a quantidade de clientes for igual a quantidade maxima, a fila está cheia
        printf("Fila está cheia... \n");
        return; // e então volta a função main
    }

    int i; // é criada uma variável temporaria para calcular o indice do vetor
    i = (fila->primeiro + fila->quantidadeClientes) % MAX; // o indice é calculado somando o indicador do inicio da fila com a quantidade total e ...
    // tirando o resto da divisão com a quantidade maxima, assim se for inserido um cliente que excederia o tamanho maximo, ele é inserido no inicio do vetor
    fila->vetor[i] = cliente; // cliente é inserido no indice calculado
    (fila->quantidadeClientes)++; // é somado um na quantidade de clientes
}

int desenfileirarNormal(struct FilaNormal *fila, struct Cliente *cliente) { // função para desenfileirar da fila normal
    if(fila->primeiro > fila->ultimo || fila->primeiro >= MAX) { // a fila está vazia quando o indicador do inicio está maior que o fim ou quando o inicio excede o tamanho
        printf("Fila está vazia... \n");
        return 0; // e então volta a função main com o retorno falso
    }

    *cliente = fila->vetor[fila->primeiro]; // a variavel cliente recebe o cliente que foi removido da fila
    (fila->primeiro)++; // e é somado um no indicador do primeiro da fila
    return 1; // retorna com o verdadeiro
}

int desenfileirarPreferencial(struct FilaPreferencial *fila, struct Cliente *cliente) { // função para desenfileirar da fila preferencial
    if(fila->quantidadeClientes == 0) { // se não há clientes na fila, então ela está vazia...
        printf("Fila está vazia... \n");
        return 0; // e entrão retorna com falso
    }

    *cliente = fila->vetor[fila->primeiro]; // a variavel cliente recebe o cliente a ser removido
    fila->primeiro = (fila->primeiro + 1) % MAX; // o indicador do primeiro da fila recebe o resto da divisão entre ele mesmo mais um com o valor maximo de...
    // clientes que certifica que, se o indicador fosse ultrapassar o valor maximo, ele retorna ao inicio
    (fila->quantidadeClientes)--; // e é removido um da quantidade de clientes
    return 1; // retorna com verdadeiro
}

void mostrarFilaNormal(struct FilaNormal fila) { // função para mostrar fila normal
    if(fila.primeiro > fila.ultimo || fila.primeiro >= MAX) { // se a fila estiver vazia ela não é mostrada
        printf("A fila está vazia... \n");
        return;
    }else {
        printf("Fila Normal: \n");
        for(int i = fila.primeiro; i <= fila.ultimo; i++) { // é mostrado do primeiro até o ultimo cliente da fila
            printf("Cliente senha: %d / horário de adimissão: %s \n", fila.vetor[i].senha, fila.vetor[i].horario); // é mostrado a senha e horario
        }
    }
}

void mostrarFilaPreferencial(struct FilaPreferencial fila) { // função para mostrar fila preferencial
    int j = fila.primeiro; // variavel de indice auxiliar que começa com o indice do primeiro da fila

    if(fila.quantidadeClientes == 0) { // se a fila estiver vazia ela não é mostrada
        printf("A fila está vazia... \n");
        return;
    }else {
        printf("Fila Preferencial: \n");
        for(int i = 0; i < fila.quantidadeClientes; i++) { // é mostrada apenas a quantidade de clientes
            printf("Cliente senha: %d / horário de adimissão: %s \n", fila.vetor[j].senha, fila.vetor[j].horario); // é mostrado a senha e horario
            j = (j + 1) % MAX; // o indice é incrementado um e tirado o resto com o valor maximo para manter a circularidade da fila
        }
    }
}

int cancelarSenhaNormal(struct FilaNormal *fila, int senhaAlvo) { // função para cancelar senha normal
    if(fila->primeiro > fila->ultimo || fila->primeiro >= MAX) { // verifica se a fila estiver vazia
        printf("Fila vazia... \n");
        return 0;
    }

    int i = -1; // variavel temporaria para achar o indice da senha a ser removida
    for(int j = fila->primeiro; j <= fila->ultimo; j++) { // percorre o vetor para achar o indice da senha
        if(fila->vetor[j].senha == senhaAlvo) {
            i = j;
            break;
        }
    }

    if(i != -1) { // se a senha foi encontrada...
        for(int j = i; j < fila->ultimo; j++) { // se percorre o vetor "empurrando" os elementos ápos o cancelado para cobrir
            fila->vetor[j] = fila->vetor[j + 1];
        }

        (fila->ultimo)--; // e se remove 1 da quantidade
        return 1;
    }

    return 0;
}

int cancelarSenhaPreferencial(struct FilaPreferencial *fila, int senhaAlvo) { // função para cancelar senha preferencial
    if(fila->quantidadeClientes == 0) { // verifica se a fila estiver vazia
        printf("Fila vazia... \n");
        return 0;
    }

    int i = -1; // variavel temporaria para achar o indice da senha a ser removida
    int atual = fila->primeiro; // auxiliar para o indice da fila circular

    for(int j = 0; j < fila->quantidadeClientes; j++) { // percorre a fila procurando a senha indicada
        if(fila->vetor[atual].senha == senhaAlvo) {
            i = atual;
            break;
        }
        atual = (atual + 1) % MAX;
    }

    if(i != -1) { // se foi achada...
        int proximo = 0; // auxiliar para guardar o proximo indice

        for(int j = 0; j < fila->quantidadeClientes - 1; j++) { // se percorre a fila para "mover" os clientes e apagar o selecionado
            proximo = (i + 1) %  MAX;
            fila->vetor[i] = fila->vetor[proximo];
            i = proximo;
        }

        (fila->quantidadeClientes)--; // diminui um da quantidade
        return 1;
    }

    return 0;
}

int buscarNormal(struct FilaNormal fila, int senhaAlvo) { // função para buscar senha na fila normal
    int busca = -1; // variavel que receberá o indice do cliente para testes

    if(fila.primeiro > fila.ultimo || fila.primeiro >= MAX) { // verifica se fila está vazia
        printf("Fila vazia... \n");
        return 0;
    }

    for(int i = fila.primeiro; i <= fila.ultimo; i++) { // percorre o vetor para achar o indice da senha
        if(fila.vetor[i].senha == senhaAlvo) {
            busca = i;
            return 1;
        }
    }

    return 0;
}

int buscarPreferencial(struct FilaPreferencial fila, int senhaAlvo) { // função para buscar senha na fila preferencial
    int busca = -1; // variavel que receberá o indice do cliente para testes
    int atual = fila.primeiro; // variavel de onde começará a busca

    if(fila.quantidadeClientes == 0) { // verifica se fila está vazia
        printf("Fila vazia... \n");
        return 0;
    }

    for(int i = 0; i < fila.quantidadeClientes; i++) { // percorre a fila procurando a senha indicada
        if(fila.vetor[atual].senha == senhaAlvo) {
            busca = atual;
            return 1;
        }
        atual = (atual + 1) % MAX;
    }

    return 0;
}

int main() {
    /* Simulador de fila do banco */
    setlocale(LC_ALL, "");

    struct Cliente C; // variavel de struct cliente, para criar novos clientes do banco
    struct FilaNormal fN; // variavel da fila normal
    initNormal(&fN); // inicializando fila normal...
    struct FilaPreferencial fP; // variavel da fila prefencial
    initPrefencial(&fP); // inicializando fila prefencial...

    int entradaNum = 0, temp = 0; // variaveis temporarias para auxilio
    char entradaChar; // variaveis para entradas temporarias
    int op = 0; // variavel de selecao de opcao do menu inicial
    int op2 = 0; // variavel de selecao de opcao do submenu de insercao
    int contNormal = 0; // variavel de controle para atendimento, conta quantos clientes normais foram atendidos
    int vezPreferencial = 1; // variavel de controle para atendimento, verifica se é a vez do preferencial

    carregarCSV(&fN, &fP);

    do {
        menuInicial();
        scanf("%d", &op);

        switch(op) {
            case 1:
                // inserir cliente na fila: aplicar submenu de insercao
                do {
                    menuInsercao();
                    scanf("%d", &op2);

                    switch(op2) {
                        case 1:
                            // inserção do cliente preferencial
                            C.tipo = 'P'; // tipo do cliente
                            printf("Insira o número da senha desejada: \n");
                            scanf("%d", &C.senha); // digito da senha escolhida pelo cliente
                            printf("Qual é o horário agora?: (hs:mn) \n");
                            scanf("%s", C.horario); // horario que o cliente tirou a senha
                            enfileirarPreferencial(&fP, C); // enfileiramento do cliente
                            printf("Cliente prefencial inserido! \n");
                            break;
                        case 2:
                            // inserção do cliente normal
                            C.tipo = 'N'; // tipo do cliente
                            printf("Insira o número da senha desejada: \n");
                            scanf("%d", &C.senha); // digito da senha escolhida pelo cliente
                            printf("Qual é o horário agora?: (hs:mn) \n");
                            scanf("%s", C.horario); // horario que o cliente tirou a senha
                            enfileirarNormal(&fN, C); // enfileiramento do cliente
                            printf("Cliente normal inserido! \n");
                            break;
                        case 3:
                            printf("Voltando... \n");
                            break;
                        default:
                            printf("Entrada não aceita, tente novamente... \n");
                            break;
                    }
                }while(op2 != 3);

                break;
            case 2:
                // mostrar filas
                mostrarFilaNormal(fN);
                mostrarFilaPreferencial(fP);
                break;
            case 3:
                // buscar senha
                printf("Insira senha que deseja buscar: \n");
                scanf("%d", &entradaNum);

                temp = buscarNormal(fN, entradaNum); // tenta buscar na fila normal

                if(temp == 1) {
                    printf("Senha existe e é de um cliente normal \n");
                }else {
                    temp = buscarPreferencial(fP, entradaNum); // se não estiver na normal, tenta na preferencial

                    if(temp == 1) {
                        printf("Senha existe e é de um cliente preferencial \n");
                    }else {
                        printf("Cliente não existe... \n"); // se não estiver em ambas, ele não existe
                    }
                }
                break;
            case 4:
                // atender (remover) clientes
                if((fN.primeiro > fN.ultimo || fN.primeiro >= MAX) && fP.quantidadeClientes == 0) { // verifica se as filas estão vazias
                    printf("Nenhum cliente a ser atendido... \n");
                    break;
                }

                if(vezPreferencial == 1 && fP.quantidadeClientes > 0) { // se for a vez do preferencial e houver clientes para atender
                    temp = desenfileirarPreferencial(&fP, &C); // temp segura o resultado 0 se for vazia e 1 se houve cliente

                    if(temp == 1) {
                        printf("Atendendo cliente preferencial, senha: %d \n", C.senha);
                        vezPreferencial = 0; // passa a ser a vez dos normais
                        contNormal = 0;
                    }
                }else { // se não for a vez preferencial ou não houver clientes preferenciais, atende os normais
                    temp = desenfileirarNormal(&fN, &C);

                    if(temp == 1) {
                        printf("Atendendo cliente normal, senha: %d \n", C.senha);
                        contNormal++;

                        if(contNormal >= 2) { // se 2 normais foram atendidos, passa a ser a vez dos preferencias
                            vezPreferencial = 1;
                        }
                    }else if(fP.quantidadeClientes > 0) { // se tentou atender clientes normais mas a fila está vazia, se atende os preferenciais
                        temp = desenfileirarPreferencial(&fP, &C);

                        if(temp == 1) {
                            printf("Atendendo cliente preferencial (fila normal vazia), senha: %d \n", C.senha);
                            vezPreferencial = 0;
                            contNormal = 0;
                        }
                    }
                }

                if(vezPreferencial == 0 && (fN.primeiro > fN.ultimo || fN.primeiro >= MAX)) { // se era a vez dos normais e a fila está vazia, passa a ser a vez dos preferenciais
                    vezPreferencial = 1;
                }
                break;
            case 5:
                // cancelar senha
                printf("Insira a senha que deseja cancelar: \n");
                scanf("%d", &entradaNum);

                temp = cancelarSenhaNormal(&fN, entradaNum); // tenta cancelar a senha normal

                if(temp == 1) {
                    printf("Senha %d normal cancelada \n", entradaNum);
                }else {
                    temp = cancelarSenhaPreferencial(&fP, entradaNum); // se não foi achada na normal, tenta na fila preferencial

                    if(temp == 1) {
                        printf("Senha %d preferencial cancelada \n", entradaNum);
                    }else {
                        printf("Senha não encontrada em nenhuma das filas... \n"); // se não foi encontrada em nenhuma das duas, mostra uma mensagem de erro
                    }
                }
                break;
            case 6:
                salvarCSV(fN, fP);
                printf("Obrigado por usar o sistema! \n");
                break;
            default:
                printf("Entrada não aceita, tente novamente...\n");
                break;
        }
    }while(op != 6); // enquanto a opcao de saida nao foi selecionada...

    return 0;
}
