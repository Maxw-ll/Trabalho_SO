
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

//Máxima quantidade de Arquivos e Nome do Disco. Quantidade de Arquivos Alta
#define DISK_NAME "M.iso"
#define MAXQTD_FILES 1000

//Tamanho Máximo do Disco
#define TMAX_DISCO 1048576 // 1 MB

//Supoorta Arquivos com nome menores que 100 Caracteres e com Conteudo de até 10 mil Carcateres
#define TMAX_NAME 100
#define TMAX_COMAND 4
#define TMAX_CONTENT 10000 

typedef struct {
    char name [TMAX_NAME]; //Nome do Arquivo
    int  start;  //Byte onde o Arquivo começa 
    int  size;   //Total de Bytes do Arquivo
} Arquivo;

Arquivo Diretorio[MAXQTD_FILES]; //Vetor de Arquivos, com cada Arquivo do Disco

int file_count = 0; //Contador para a Quantidade de Arquivos

void init_disk() {
    FILE *disk = fopen(DISK_NAME, "wb");
    if (!disk) {
        perror("Erro ao tentar Criar o Disco!");
        exit(1);
    }
    // Preenche o disco com zeros, equivalente a iniciar ele sem nenhum arquivo
    char zero = 0;
    for (int i = 0; i < TMAX_DISCO; i++) {
        fwrite(&zero, sizeof(zero), 1, disk);
    }
    fclose(disk);
}

void load_Diretorio() {
    FILE *disk = fopen(DISK_NAME, "rb");
    if (!disk) {
        perror("Erro ao Abrir o Disco!");
        exit(1); //Finaliza o Programa indicando que houve um Erro
    }
    fread(&file_count, sizeof(file_count), 1, disk);
    fread(Diretorio, sizeof(Arquivo), MAXQTD_FILES, disk);
    fclose(disk);
}

void salvar_diretorio() {
    FILE *disk = fopen(DISK_NAME, "r+b");
    if (!disk) {
        perror("Erro ao Abrir o Disco!");
        exit(1); //Finaliza o Programa indicando que houve um Erro
    }
    fwrite(&file_count, sizeof(file_count), 1, disk);
    fwrite(Diretorio, sizeof(Arquivo), MAXQTD_FILES, disk);
    fclose(disk);
}

int verifica_existencia(char *name){
    for(int i=0; i<file_count; i++){
        if(strcmp(Diretorio[i].name, name) == 0){
            return TRUE;
        }
    }
    return FALSE;
}

void criar_arquivo(char *name, char *content) {
    if (verifica_existencia(name) == TRUE){
        printf("Nome de Arquivo ja Existe! Insira um Nome Diferente!\n");
        return;
    } 
    if (file_count >= MAXQTD_FILES) {
        printf("O Disco está Cheio! Máxima Quantidade de Arquivos Atingida: %d\n", MAXQTD_FILES);
        return;
    }

    int start;
    int content_len = strlen(content);
    if(file_count == 0){
        start = sizeof(int) + MAXQTD_FILES * sizeof(Arquivo); 
    } else{
        start = Diretorio[file_count - 1].start + Diretorio[file_count - 1].size;
    }

    if (start + content_len > TMAX_DISCO) {
        printf("O Disco está Cheio! Sem Espaço Suficiente!\n");
        return;
    }

    strcpy(Diretorio[file_count].name, name);
    Diretorio[file_count].start = start;
    Diretorio[file_count].size = content_len;
    file_count++;

    FILE *disk = fopen(DISK_NAME, "r+b");
    fseek(disk, start, SEEK_SET);
    fwrite(content, sizeof(char), content_len, disk);
    fclose(disk);

    salvar_diretorio();
}

void listar_arquivos() {
    for (int i = 0; i < file_count; i++) {
        printf("%s\n", Diretorio[i].name);
    }
}


void mostrar_arquivo(char *name) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(Diretorio[i].name, name) == 0) {
            //Iniciamos um conteúdo com tamanho do diretorio mais um, para acrecentarmos um \0 ao final
            char *content = (char *)malloc(Diretorio[i].size + 1);
            //Abrimos o Disco como Leitura
            FILE *disk = fopen(DISK_NAME, "rb");
            fseek(disk, Diretorio[i].start, SEEK_SET);
            fread(content, sizeof(char), Diretorio[i].size, disk);
            content[Diretorio[i].size] = '\0';
            fclose(disk);

            printf("%s", content);
            free(content);
            return;
        }
    }
    printf("Arquivo nao Encontrado!\n");
}

//Definir a função do Comando quando temos mais de duas entradas no terminal
void tratar_comando(char* comando, char* arquivo, char* conteudo){
    if (strcmp(comando, "cat") == 0) {
        criar_arquivo(arquivo, conteudo);
    } else if (strcmp(comando, "more") == 0) {
        mostrar_arquivo(arquivo);
    } 
}

//Imprimir os Comandos Disponíveis
void imprimir_help(){
    printf("Comandos disponiveis:\n");
    printf("[Criar Arquivo]   : cat  <nome> <conteudo>\n");
    printf("[Ler Arquivo]     : more <nome>\n");
    printf("[Listar Arquivos] : ls\n");
    printf("[Limpar Terminal] : clear\n");
    printf("[Encerrar]        : exit\n");
}


// Impressão para Simular um Terminal identificando o comando digitado e realizando sua função
int mini_terminal(char* comando, char* arquivo, char* conteudo){
    
    //"Apagar" o conteúdo das entradas para reutilizálas de novo
    //Não realmente apagamos, apenas como o '\0' no inicio, pois '\0' demarca o fim da string
    comando [0] = '\0';
    arquivo [0] = '\0';
    conteudo[0] = '\0';

    printf("M:\\Max\\");
    scanf("%s", comando);
    int get_next_entry = FALSE;

    //Como são strings não conseguimos usar Switch, else if mesmo. Percorrendo todos os possíveis comandos. 
    //Existem comandos de uma linha só, como ls, /help, clear, exit. Esses já sao executados aqui
    //Mas ainda temos o cat e more que precisam de mais argumentos. Nesse caso, pegamos as próximas entradas
    //Caso nenhum dos comandos 
    if (strcmp(comando, "ls") == 0){
        listar_arquivos();
    } else if (strcmp(comando, "/help") == 0){
        imprimir_help();
    } else if (strcmp(comando, "exit") == 0){
        return FALSE;
    } else if (strcmp(comando, "clear") == 0){
        system("clear");
    } else if (strcmp(comando, "cat") == 0) {
        get_next_entry = TRUE;
    } else if (strcmp(comando, "more") == 0) {
        get_next_entry = TRUE;
    } else {
        printf("Comando Desconhecido!: Digite /help para visualizar os comandos disponiveis\n");
        //Aqui o fgets pega todo o restante digitado para Apaga-lo depois, impedindo que o comando pegar cada entrada separada. 
        //Caso contrario tinhamos um bug de imprimir essa mensagem varias vezes, a quantidade de quantas entradas separadas por espaço tivermos
        fgets(conteudo, TMAX_CONTENT, stdin);
        return TRUE;
    }
    
    if (get_next_entry == TRUE){
        scanf("%s", arquivo);
        if (strcmp(comando, "cat") == 0){
            getchar(); // Pega o próximo Espaço na Entrada. Para no conteudo ficar sem esse espaço Inicial
        }
        fgets(conteudo, TMAX_CONTENT, stdin);
        tratar_comando(comando, arquivo, conteudo);
    } else{
        //Limpar e entrada quando digitado um comando que já existe como ls e o usuario digita algo a mais. Ex: ls arquivo conteudo
        //Caso contrario tinhamos um bug de imprimir essa mensagem varias vezes, a quantidade de quantas entradas separadas por espaço tivermos
        fgets(conteudo, TMAX_CONTENT, stdin);
    }
    
    return TRUE; 
}


int main(int argc, char *argv[]) {
  
    //Inicializar o disco se ele não existir

    FILE *disk_check = fopen(DISK_NAME, "rb");
    if (!disk_check) {
        init_disk();
    } else {
        fclose(disk_check);
    }

    //Carregando Sempre o Diretorio ao iniciar
    load_Diretorio();

    //Variáveis para armazenar as entradas da linha de comando
    char* comando  = (char*)malloc(sizeof(char)*TMAX_COMAND);
    char* arquivo  = (char*)malloc(sizeof(char)*TMAX_NAME);
    char* conteudo = (char*)malloc(sizeof(char)*TMAX_CONTENT);
    
    //Variável de Controle para Encerrar o Programa
    int run = TRUE;
    while (run){
        run = mini_terminal(comando, arquivo, conteudo);
    }

    printf("Obrigado por usar meu Programa de Gerenciamento de Arquivos em Disco!");
    return 0;
}


