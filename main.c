#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256


// ===========================
// Funções auxiliares
// ===========================


void remove_newline(char *ptr);

/// @brief Lê uma linha do arquivo de entrada, verificando um caracter por vez.
/// @param line 
/// @return 
int read_line(char *line);


void generate_header(FILE *file);

/// @brief Calcula a posição da pilha que deve ser salvo
/// @param align 
/// @param saving_size 
/// @param stack 
/// @return 
int calculate_stack(int align, int saving_size, int *stack);


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
    int stack_counter = 0;
    int stack = 0;
    char p1_type;
    char p2_type;
    char p3_type;
    char var_type;
    int function_block = 1;
    int def_block = 0;
    int body_function_block = 0;

    char _1_operand_type;
    char _2_operand_type;
    char operand;
    int _1_operand_index;
    int _2_operand_index;
    char control_str[4] = {0,0,0,0};


    if((arquivo_S = fopen("Assembly.S", "w")) == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    generate_header(arquivo_S);

    while (read_line(line)){

        /// @brief bloco da que gera codigo do começo da função
        /// @return 
        if(function_block){
            r = sscanf(line, "function f%d p%c1 p%c2 p%c3", &f_num, &p1_type, &p2_type, &p3_type);

            if (r == 4) {
                sprintf(assembly_line, "\t.globl f%d\nf%d:\n", f_num, f_num);
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    pushq %%rbp\n");
                fputs(assembly_line, arquivo_S);
                sprintf(assembly_line, "    movq %%rsp, %%rbp\n\n");
                fputs(assembly_line, arquivo_S);

                stack = 0;

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

                stack = 0;

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

                stack = 0;

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

                stack = 0;

                function_block = 0;
                continue;
            }
        }
 
        /// @brief bloco que faz a verificação do final da função, e do bloco de definição
        /// @return 
        {/*****************************************************/
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
                body_function_block = 1; 

                continue;
            }
        }


        /// @brief bloco que gera o codigo relativo a definição de variaveis locais
        /// @return 
        if(def_block){
            r = sscanf(line, "v%c", &var_type);
            if(var_type == 'a'){
                r = sscanf(line, "v%cr vi%d", &var_type, &var_indice);

                stack_counter = calculate_stack(4, 4, &stack);
                
                sprintf(assembly_line, "    # vi%d: -%d\n", var_indice, stack_counter);
                fputs(assembly_line, arquivo_S);

                continue;
            }
            else{
                r = sscanf(line, "v%ct va%d size ci%d", &var_type, &var_indice, &vet_size);

                stack_counter = calculate_stack(8, 8*vet_size, &stack);
                
                sprintf(assembly_line, "    # va%d: -%d\n", var_indice, stack_counter);
                fputs(assembly_line, arquivo_S);

                continue;
            }

        }


        if(body_function_block){
            /// @brief bloco de atribuição
            /// @return 
            {/*****************************************************/

                sscanf(line, "vi%d = %c%c%c%c", &var_indice, 
                &control_str[0],
                &control_str[1],
                &control_str[2],
                &control_str[3]);

                if(strncmp(control_str, "call", 4) == 0){
                    sprintf(assembly_line, "    funcao de call\n");
                    fputs(assembly_line, arquivo_S);

                    control_str[0] = 0;
                    continue;
                }

                sscanf(line, "vi%d = %ci%d", &var_indice, &_1_operand_type, &_1_operand_index);
            
                sscanf(line, "vi%d = %ci%d %c %ci%d",
                    &var_indice, 
                    &_1_operand_type,
                    &_1_operand_index,
                    &operand,
                    &_2_operand_type,
                    &_2_operand_index);

                
                




            }
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

int calculate_stack(int align, int saving_size, int *stack){
    if(*stack % align == 0 || stack == 0){
        *stack += saving_size;

        return *stack;
    }
    else{
        *stack += (align - (*stack % align));
        
        *stack += saving_size;

        return *stack;
    }
}
