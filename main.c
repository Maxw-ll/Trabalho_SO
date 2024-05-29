
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_FILE "D.png"
#define MAXQTD_FILES 2
#define TAMMAX_LINECOMAND 606
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
    FILE *disk = fopen(DISK_FILE, "wb");
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
    FILE *disk = fopen(DISK_FILE, "rb");
    if (!disk) {
        perror("Erro ao Abrir o Disco!");
        exit(1); //Finaliza o Programa indicando que houve um Erro
    }
    fread(&file_count, sizeof(file_count), 1, disk);
    fread(Diretorio, sizeof(Arquivo), MAXQTD_FILES, disk);
    fclose(disk);
}

void save_Diretorio() {
    FILE *disk = fopen(DISK_FILE, "r+b");
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

    FILE *disk = fopen(DISK_FILE, "r+b");
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
            FILE *disk = fopen(DISK_FILE, "rb");
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


char* mini_terminal(){
    printf("M:\\Max\\");
    char* linha_de_comando = (char*)(malloc(sizeof(char)*TAMMAX_LINECOMAND));
    fgets(linha_de_comando, TAMMAX_LINECOMAND, stdin);
    return linha_de_comando;
}

void split_entrada(char* line_comand, char* comando, char* arquivo, char* conteudo){
    int len = strlen(line_comand);
    
    int spaces = 0;
    int idx =0;
    for( int i=0; i<len-1; i++){
        if(line_comand[i] == ' '){
            switch(spaces){
                case 0: comando[idx] = '\0'; break;
                case 1: arquivo[idx] = '\0'; break;
                case 2: conteudo[idx] = '\0'; break;
            }
            spaces += 1;
            idx = 0;
        }
        else{
            switch(spaces){
                case 0:
                {
                    comando[idx] = line_comand[i];
                } break;
                case 1:
                {
                    arquivo[idx] = line_comand[i];
                } break;
                case 2:
                {   
                    conteudo[idx] = line_comand[i];
                } break;
            } 
        }
        idx += 1;
        printf("%d", idx);

    }
}

int main(int argc, char *argv[]) {
    // if (argc < 2) {
    //     fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
    //     return 1;
    // }

    // // Inicializar o disco se ele não existir
    // FILE *disk_check = fopen(DISK_FILE, "rb");
    // if (!disk_check) {
    //     init_disk();
    // } else {
    //     fclose(disk_check);
    // }

    // load_Diretorio();

    // const char *command = argv[1];
    #define TRUE 1
    char* comando  = (char*)malloc(sizeof(char)*TMAX_COMAND);
    char* arquivo  = (char*)malloc(sizeof(char)*TMAX_NAME);
    char* conteudo = (char*)malloc(sizeof(char)*TMAX_CONTENT);
    while (TRUE){
       
        char* ok = mini_terminal();

        printf("%s / %s  / %s", comando, arquivo, conteudo);
        split_entrada(ok, comando, arquivo, conteudo);
        printf("%s / %s  / %s", comando, arquivo, conteudo);

    }


    //MUDAR PARA SWITCH
    




    // if (strcmp(command, "cat") == 0 && argc == 4) {
    //     create_file(argv[2], argv[3]);
    // } else if (strcmp(command, "ls") == 0) {
    //     list_files();
    // } else if (strcmp(command, "more") == 0 && argc == 3) {
    //     show_file_content(argv[2]);
    // } else {
    //     fprintf(stderr, "Unknown command or incorrect arguments\n");
    // }

    return 0;
}


