#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256


// ===========================
// Funções auxiliares
// ===========================


void remove_newline(char *ptr);


// Lê uma linha do arquivo de entrada, verificando um caracter por vez.
int read_line(char *line);


void generate_header(FILE *file);


// ===========================
// Main
// ===========================


int main (){
    FILE *arquivo_S;
    char line[LINESZ];
    char assembly_line[LINESZ];
    int r;
    int f_num;
    char p1_type;
    char p2_type;
    char p3_type;
    int function_block = 1;


    if((arquivo_S = fopen("Assembly.S", "w")) == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    generate_header(arquivo_S);

    while (read_line(line)){

        if(function_block){
            r = sscanf(line, "function f%d p%c1 p%c2 p%c3", &f_num, &p1_type, &p2_type, &p3_type);

            if (r == 4) {
                sprintf(assembly_line, "\t.globl f%d\nf%d:\n   tipo 1 = %c\n   tipo 2 = %c\n   tipo 3 = %c\n\n", f_num, f_num, p1_type, p2_type, p3_type);
                fputs(assembly_line, arquivo_S);

                function_block = 0;
                continue;
            }

            if (r == 3) {
                sprintf(assembly_line, "    .globl f%d\nf%d:\n   tipo 1 = %c\n   tipo 2 = %c\n\n", f_num, f_num, p1_type, p2_type);
                fputs(assembly_line, arquivo_S);

                function_block = 0;
                continue;
            }

            if (r == 2) {
                sprintf(assembly_line, "    .globl f%d\nf%d:\n   tipo 1 = %c\n\n", f_num, f_num, p1_type);
                fputs(assembly_line, arquivo_S);

                function_block = 0;
                continue;
            }

            if (r == 1) {
                sprintf(assembly_line, "    .globl f%d\nf%d:\n\n", f_num, f_num);
                fputs(assembly_line, arquivo_S);

                function_block = 0;
                continue;
            }
        }

        if (strncmp(line, "end", 3) == 0) {
            function_block = 1;

            continue;
        }

    }

    fclose(arquivo_S);

    return 0;
}


// ===========================
// Funções auxiliares
// ===========================


void remove_newline(char *ptr){
  while (*ptr) {
    if (*ptr == '\n')
      *ptr = 0;
    else
      ptr++;
  }
}


int read_line(char *line){
    if(fgets(line, LINESZ, stdin) != NULL){
        remove_newline(line);

        return 1;
    }
    else return 0;
}


void generate_header(FILE *file){
    fputs(".section .rodata\n\n.data\n\n\t.text\n\n", file);
}