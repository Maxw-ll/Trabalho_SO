
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1

#define DISK_NAME "D.png"
#define MAXQTD_FILES 100

#define DISK_SIZE 1048576 // 1 MB

#define TMAX_NAME 100
#define TMAX_COMAND 4
#define TMAX_CONTENT 500

typedef struct {
    char name[TMAX_NAME]; //Nome do Arquivo
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
    for (int i = 0; i < DISK_SIZE; i++) {
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

void save_Diretorio() {
    FILE *disk = fopen(DISK_NAME, "r+b");
    if (!disk) {
        perror("Erro ao Abrir o Disco!");
        exit(1); //Finaliza o Programa indicando que houve um Erro
    }
    fwrite(&file_count, sizeof(file_count), 1, disk);
    fwrite(Diretorio, sizeof(Arquivo), MAXQTD_FILES, disk);
    fclose(disk);
}


void create_file(const char *name, const char *content) {
    if (file_count >= MAXQTD_FILES) {
        fprintf(stderr, "O Disco está Cheio! Máxima Quantidade de Arquivos Atingida\n");
        return;
    }

    int content_len = strlen(content);
    int start = (file_count == 0) ? sizeof(int) + MAXQTD_FILES * sizeof(Arquivo) : Diretorio[file_count - 1].start + Diretorio[file_count - 1].size;

    if (start + content_len > DISK_SIZE) {
        fprintf(stderr, "O Disco está Cheio! Sem Espaço Suficiente\n");
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

    save_Diretorio();
}

void list_files() {
    for (int i = 0; i < file_count; i++) {
        printf("%s\n", Diretorio[i].name);
    }
}


void show_file_content(const char *name) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(Diretorio[i].name, name) == 0) {
            char *content = (char *)malloc(Diretorio[i].size + 1);
            FILE *disk = fopen(DISK_NAME, "rb");
            fseek(disk, Diretorio[i].start, SEEK_SET);
            fread(content, sizeof(char), Diretorio[i].size, disk);
            content[Diretorio[i].size] = '\0';
            fclose(disk);

            printf("%s\n", content);
            free(content);
            return;
        }
    }
    fprintf(stderr, "File not found\n");
}

void tratar_comando(char* comando, char* arquivo, char* conteudo){

    
    if (strcmp(comando, "cat") == 0) {
        create_file(arquivo, conteudo);
    } else if (strcmp(comando, "ls") == 0) {
        list_files();
    } else if (strcmp(comando, "more") == 0) {
        show_file_content(arquivo);
    } else {
        fprintf(stderr, "Unknown command or incorrect arguments\n");
    }
}

char* mini_terminal(char* comando, char* arquivo, char* conteudo){
    printf("M:\\Max\\");
    scanf("%s %s ", comando, arquivo);
    fgets(conteudo, TMAX_CONTENT, stdin);
    tratar_comando(comando, arquivo, conteudo);
}


int main(int argc, char *argv[]) {
  
    //Inicializar o disco se ele não existir

    FILE *disk_check = fopen(DISK_NAME, "rb");
    if (!disk_check) {
        init_disk();
    } else {
        fclose(disk_check);
    }

    load_Diretorio();



    char* comando  = (char*)malloc(sizeof(char)*TMAX_COMAND);
    char* arquivo  = (char*)malloc(sizeof(char)*TMAX_NAME);
    char* conteudo = (char*)malloc(sizeof(char)*TMAX_CONTENT);

    while (TRUE){
       
        char* ok = mini_terminal(comando, arquivo, conteudo);

       

    }


  
    




    return 0;
}


