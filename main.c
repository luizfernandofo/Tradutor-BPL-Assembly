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
    int var_indice;
    int vet_size;
    char p1_type;
    char p2_type;
    char p3_type;
    char var_type;
    int function_block = 1;
    int def_block = 1;
    int stack_block = 0;



    if((arquivo_S = fopen("Assembly.S", "w")) == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    generate_header(arquivo_S);

    while (read_line(line)){

        if(function_block){
            r = sscanf(line, "function f%d p%c1 p%c2 p%c3", &f_num, &p1_type, &p2_type, &p3_type);

            if (r == 4) {
                sprintf(assembly_line, "\t.globl f%d\nf%d:\n", f_num, f_num);
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    pushq %%rbp\n");
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    movq %%rsp, %%rbp\n\n");
                fputs(assembly_line, arquivo_S);

                function_block = 0;
                continue;
            }

            if (r == 3) {
                sprintf(assembly_line, "    .globl f%d\nf%d:\n", f_num, f_num);
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    pushq %%rbp\n");
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    movq %%rsp, %%rbp\n\n");
                fputs(assembly_line, arquivo_S);

                function_block = 0;
                continue;
            }

            if (r == 2) {
                sprintf(assembly_line, "    .globl f%d\nf%d:\n", f_num, f_num);
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    pushq %%rbp\n");
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    movq %%rsp, %%rbp\n\n");
                fputs(assembly_line, arquivo_S);

                function_block = 0;
                continue;
            }

            if (r == 1) {
                sprintf(assembly_line, "    .globl f%d\nf%d:\n", f_num, f_num);
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    pushq %%rbp\n");
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    movq %%rsp, %%rbp\n\n");
                fputs(assembly_line, arquivo_S);

                function_block = 0;
                continue;
            }
        }

        if (strncmp(line, "end", 3) == 0 && strncmp(line, "enddef", 6) != 0) {
            function_block = 1;

            sprintf(assembly_line, "    leave\n");
            fputs(assembly_line, arquivo_S);
            sprintf(assembly_line, "    ret\n\n");
            fputs(assembly_line, arquivo_S);

            continue;
        }

        if (strncmp(line, "def", 3) == 0) {
            def_block = 1;

            continue;
        }

        if (strncmp(line, "enddef", 6) == 0) {
            def_block = 0;

            continue;
        }

        if(def_block){
            r = sscanf(line, "v%c", &var_type);
            if(var_type == 'a'){
                r = sscanf(line, "v%cr vi%d", &var_type, &var_indice);

                sprintf(assembly_line, "    tipo var = %c, var%d\n\n", var_type, var_indice);
                fputs(assembly_line, arquivo_S);

                continue;
            }
            else{
                r = sscanf(line, "v%ct va%d size ci%d", &var_type, &var_indice, &vet_size);

                sprintf(assembly_line, "    tipo vet = %c, vet%d size %d\n\n", var_type, var_indice, vet_size);
                fputs(assembly_line, arquivo_S);

                continue;
            }

        }

        if (strncmp(line, "enddef", 3) == 0) {
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