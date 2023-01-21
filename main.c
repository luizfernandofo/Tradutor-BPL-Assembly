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
    int var_index;
    int vet_size;
    int stack_counter = 0;
    int stack = 0;
    int func_number_of_para;
    char p1_type;
    char p2_type;
    char p3_type;
    char var_type;
    int function_block = 1;
    int def_block = 0;
    int body_function_block = 0;

    char _1_operand_type;
    char _2_operand_type;
    char _3_operand_type;
    char _1_op_sub_type;
    char _2_op_sub_type;
    char _3_op_sub_type;
    char operand;
    int _1_operand_index;
    int _2_operand_index;
    int _3_operand_index;
    int func_index;
    char control_str[4] = {0,0,0,0};

    int var_stack[5];
    int para_stack[3];


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

                stack_counter = calculate_stack(8, 8, &stack);
                sprintf(assembly_line, "    # %%rdi ->   -%d   # 1o para\n",  stack_counter);
                fputs(assembly_line, arquivo_S);
                para_stack[0] = stack_counter;

                stack_counter = calculate_stack(8, 8, &stack);
                sprintf(assembly_line, "    # %%rsi ->   -%d   # 2o para\n",  stack_counter);
                fputs(assembly_line, arquivo_S);
                para_stack[1] = stack_counter;

                stack_counter = calculate_stack(8, 8, &stack);
                sprintf(assembly_line, "    # %%rdx ->   -%d   # 3o para\n",  stack_counter);
                fputs(assembly_line, arquivo_S);
                para_stack[2] = stack_counter;

                func_number_of_para = 3;

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

                stack_counter = calculate_stack(8, 8, &stack);
                sprintf(assembly_line, "    # %%rdi ->   -%d   # 1o para\n",  stack_counter);
                fputs(assembly_line, arquivo_S);
                para_stack[0] = stack_counter;

                stack_counter = calculate_stack(8, 8, &stack);
                sprintf(assembly_line, "    # %%rsi ->   -%d   # 2o para\n",  stack_counter);
                fputs(assembly_line, arquivo_S);
                para_stack[1] = stack_counter;

                func_number_of_para = 2;

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

                stack_counter = calculate_stack(8, 8, &stack);
                sprintf(assembly_line, "    # %%rdi ->   -%d   # 1o para\n",  stack_counter);
                fputs(assembly_line, arquivo_S);
                para_stack[0] = stack_counter;

                func_number_of_para = 1;
                
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

                func_number_of_para = 0;

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
                r = sscanf(line, "v%cr vi%d", &var_type, &var_index);

                stack_counter = calculate_stack(4, 4, &stack);
                
                sprintf(assembly_line, "    # vi%d: -%d\n", var_index, stack_counter);
                fputs(assembly_line, arquivo_S);

                var_stack[var_index-1] = stack_counter;

                continue;
            }
            else{
                r = sscanf(line, "v%ct va%d size ci%d", &var_type, &var_index, &vet_size);

                stack_counter = calculate_stack(4, 4*vet_size, &stack);
                
                sprintf(assembly_line, "    # va%d: -%d\n", var_index, stack_counter);
                fputs(assembly_line, arquivo_S);

                var_stack[var_index-1] = stack_counter;

                continue;
            }

        }


        if(body_function_block){
            /*****************************************************/
            /// @brief Cabeçalho do corpo da função, imprime subq e salva os parametros na pilha
            /// @return 
            if(func_number_of_para != -1){
                sprintf(assembly_line, "\n    subq  $===, %%rsp\n\n");
                fputs(assembly_line, arquivo_S);

                if(func_number_of_para == 1){
                    sprintf(assembly_line, "    movq    %%rdi, -%d(%%rbp)    # salvando 1o para\n\n", para_stack[0]);
                    fputs(assembly_line, arquivo_S);

                    func_number_of_para = -1;
                }
                if(func_number_of_para == 2){
                    sprintf(assembly_line, "    movq    %%rdi, -%d(%%rbp)    # salvando 1o para\n", para_stack[0]);
                    fputs(assembly_line, arquivo_S);

                    sprintf(assembly_line, "    movq    %%rsi, -%d(%%rbp)    # salvando 2o para\n\n", para_stack[1]);
                    fputs(assembly_line, arquivo_S);

                    func_number_of_para = -1;
                }
                if(func_number_of_para == 3){
                    sprintf(assembly_line, "    movq    %%rdi, -%d(%%rbp)    # salvando 1o para\n", para_stack[0]);
                    fputs(assembly_line, arquivo_S);
                
                    sprintf(assembly_line, "    movq    %%rsi, -%d(%%rbp)    # salvando 2o para\n", para_stack[1]);
                    fputs(assembly_line, arquivo_S);

                    sprintf(assembly_line, "    movq    %%rdx, -%d(%%rbp)    # salvando 3o para\n\n", para_stack[2]);
                    fputs(assembly_line, arquivo_S);

                    func_number_of_para = -1;
                }
            }
            /*****************************************************/

            /// @brief bloco de atribuição
            /// @return 
            {/*****************************************************/
                /*************************************************/
                /// @brief Bloco para caso ocorra chamada de função
                /// @return 
                sscanf(line, "vi%d = %c%c%c%c", &var_index, 
                &control_str[0],
                &control_str[1],
                &control_str[2],
                &control_str[3]);

                if(strncmp(control_str, "call", 4) == 0){
                    r = sscanf(line, "vi%d = call f%d %c%c%d %c%c%d %c%c%d", 
                    &var_index, 
                    &func_index, 
                    &_1_operand_type,
                    &_1_op_sub_type,
                    &_1_operand_index,
                    &_2_operand_type,
                    &_2_op_sub_type,
                    &_2_operand_index,
                    &_3_operand_type,
                    &_3_op_sub_type,
                    &_3_operand_index);
                    
                    if(r == 11){
                        
                        if(_1_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%edi\n", _1_operand_index);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'v'){
                            if(_1_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%edi\n", var_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_1_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    leaq    -%d(%%rbp), %%rdi\n", var_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        if(_1_operand_type == 'p'){
                            if(_1_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10d, %%edi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_1_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10, %%rdi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        if(_2_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%esi\n", _1_operand_index);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_2_operand_type == 'v'){
                            if(_2_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%esi\n", var_stack[_2_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_2_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    leaq    -%d(%%rbp), %%rsi\n", var_stack[_2_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        if(_2_operand_type == 'p'){
                            if(_2_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_2_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10d, %%esi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_2_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_2_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10, %%rsi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        if(_3_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%edx\n", _3_operand_index);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_3_operand_type == 'v'){
                            if(_3_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%edx\n", var_stack[_3_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_3_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    leaq    -%d(%%rbp), %%rdx\n", var_stack[_3_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        if(_3_operand_type == 'p'){
                            if(_3_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_3_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10d, %%edx\n");
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_3_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_3_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10, %%rdx\n");
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        sprintf(assembly_line, "\n    call    f%d\n", func_index);
                        fputs(assembly_line, arquivo_S);

                        sprintf(assembly_line, "\n    movl    %%eax, -%d(%%rbp)\n", var_stack[var_index-1]);
                        fputs(assembly_line, arquivo_S);

                        control_str[0] = 0;
                        continue;
                    }

                    if(r == 8){

                        if(_1_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%edi\n", _1_operand_index);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'v'){
                            if(_1_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%edi\n", var_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_1_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    leaq    -%d(%%rbp), %%rdi\n", var_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        if(_1_operand_type == 'p'){
                            if(_1_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10d, %%edi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_1_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10, %%rdi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        if(_2_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%esi\n", _1_operand_index);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_2_operand_type == 'v'){
                            if(_2_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%esi\n", var_stack[_2_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_2_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    leaq    -%d(%%rbp), %%rsi\n", var_stack[_2_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        if(_2_operand_type == 'p'){
                            if(_2_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_2_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10d, %%esi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_2_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_2_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10, %%rsi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        

                        sprintf(assembly_line, "\n    call    f%d\n", func_index);
                        fputs(assembly_line, arquivo_S);

                        sprintf(assembly_line, "\n    movl    %%eax, -%d(%%rbp)\n", var_stack[var_index-1]);
                        fputs(assembly_line, arquivo_S);

                        control_str[0] = 0;
                        continue;
                    }

                    if(r == 5){
                        if(_1_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%edi\n", _1_operand_index);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'v'){
                            if(_1_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%edi\n", var_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_1_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    leaq    -%d(%%rbp), %%rdi\n", var_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        if(_1_operand_type == 'p'){
                            if(_1_op_sub_type == 'i'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10d, %%edi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                            if(_1_op_sub_type == 'a'){
                                sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                                fputs(assembly_line, arquivo_S);
                                sprintf(assembly_line, "    movl    %%r10, %%rdi\n");
                                fputs(assembly_line, arquivo_S);
                            }
                        }

                        sprintf(assembly_line, "\n    call    f%d\n", func_index);
                        fputs(assembly_line, arquivo_S);

                        sprintf(assembly_line, "\n    movl    %%eax, -%d(%%rbp)\n", var_stack[var_index-1]);
                        fputs(assembly_line, arquivo_S);

                        control_str[0] = 0;
                        continue;
                    }
                }
                /*************************************************/

                /*************************************************/
                /// @brief Bloco de opeção combinada
                /// @return 
                r = sscanf(line, "vi%d = %c%*c%d %c %c%*c%d",
                    &var_index, 
                    &_1_operand_type,
                    &_1_operand_index,
                    &operand,
                    &_2_operand_type,
                    &_2_operand_index);

                if(r == 6){
                    if(operand == '+'){
                        if(_1_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%r8d\n", _1_operand_index);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'v'){
                            sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%r8d\n", var_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'p'){
                            sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    movl    %%r10d, %%r8d\n");
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_2_operand_type == 'c'){
                            sprintf(assembly_line, "    movl    $%d, %%r9d\n", _2_operand_index);
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    addl    %%r8d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%r9d, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }

                        if(_2_operand_type == 'v'){
                            sprintf(assembly_line, "    movl    -%d(%%rbp), %%r9d\n", var_stack[_2_operand_index-1]);
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    addl    %%r8d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%r9d, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }

                        if(_2_operand_type == 'p'){
                            sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    movl    %%r10d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    addl    %%r8d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%r9d, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }
                    }

                    if(operand == '-'){
                        if(_1_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%r8d\n", _1_operand_index);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'v'){
                            sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%r8d\n", var_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'p'){
                            sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    movl    %%r10d, %%r8d\n");
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_2_operand_type == 'c'){
                            sprintf(assembly_line, "    movl    $%d, %%r9d\n", _2_operand_index);
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    subl    %%r9d, %%r8d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%r8d, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }

                        if(_2_operand_type == 'v'){
                            sprintf(assembly_line, "    movl    -%d(%%rbp), %%r9d\n", var_stack[_2_operand_index-1]);
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    subl    %%r9d, %%r8d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%r8d, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }

                        if(_2_operand_type == 'p'){
                            sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    movl    %%r10d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    subl    %%r9d, %%r8d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%r8d, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }
                        
                    }

                    if(operand == '*'){
                        if(_1_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%r8d\n", _1_operand_index);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'v'){
                            sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%r8d\n", var_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'p'){
                            sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    movl    %%r10d, %%r8d\n");
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_2_operand_type == 'c'){
                            sprintf(assembly_line, "    movl    $%d, %%r9d\n", _2_operand_index);
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    imull   %%r8d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%r9d, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }

                        if(_2_operand_type == 'v'){
                            sprintf(assembly_line, "    movl    -%d(%%rbp), %%r9d\n", var_stack[_2_operand_index-1]);
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    imull   %%r8d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%r9d, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }

                        if(_2_operand_type == 'p'){
                            sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    movl    %%r10d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    imull   %%r8d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%r9d, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }
                    }

                    if(operand == '/'){
                        if(_1_operand_type == 'c'){
                            sprintf(assembly_line, "\n    movl    $%d, %%eax\n", _1_operand_index);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    cltd   \n");
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'v'){
                            sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%eax\n", var_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    cltd   \n");
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_1_operand_type == 'p'){
                            sprintf(assembly_line, "\n    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    movl    %%r10d, %%eax\n");
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    cltd\n");
                            fputs(assembly_line, arquivo_S);
                        }

                        if(_2_operand_type == 'c'){
                            sprintf(assembly_line, "    movl    $%d, %%r9d\n", _2_operand_index);
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    idivl   %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%eax, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }

                        if(_2_operand_type == 'v'){
                            sprintf(assembly_line, "    movl    -%d(%%rbp), %%r9d\n", var_stack[_2_operand_index-1]);
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    idivl   %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%eax, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }

                        if(_2_operand_type == 'p'){
                            sprintf(assembly_line, "    movq    -%d(%%rbp), %%r10\n", para_stack[_1_operand_index-1]);
                            fputs(assembly_line, arquivo_S);
                            sprintf(assembly_line, "    movl    %%r10d, %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    idivl   %%r9d\n");
                            fputs(assembly_line, arquivo_S);

                            sprintf(assembly_line, "    movl    %%eax, -%d(%%rbp)\n", var_stack[var_index-1]);
                            fputs(assembly_line, arquivo_S);

                            continue;
                        }
                    }
                }
                /*************************************************/

                /*************************************************/
                /// @brief Bloco de atribuição simples
                /// @return 
                r = sscanf(line, "vi%d = %c%*c%d", &var_index, &_1_operand_type, &_1_operand_index);

                if(r == 3){
                    if(_1_operand_type == 'c'){
                        sprintf(assembly_line, "\n    movl    $%d, -%d(%%rbp)\n", _1_operand_index, var_stack[var_index-1]);
                        fputs(assembly_line, arquivo_S);

                        continue;
                    }

                    if(_1_operand_type == 'v'){
                        sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%r8d\n", var_stack[_1_operand_index-1]);
                        fputs(assembly_line, arquivo_S);
                        sprintf(assembly_line, "    movl    %%r8d, -%d(%%rbp)\n", var_stack[var_index-1]);
                        fputs(assembly_line, arquivo_S);

                        continue;
                    }

                    if(_1_operand_type == 'p'){
                        sprintf(assembly_line, "\n    movl    -%d(%%rbp), %%r8d\n", para_stack[_1_operand_index-1]);
                        fputs(assembly_line, arquivo_S);
                        sprintf(assembly_line, "    movl    %%r8d, -%d(%%rbp)\n", var_stack[var_index-1]);
                        fputs(assembly_line, arquivo_S);

                        continue;
                    }

                }
                /*************************************************/
            }


            /// @brief bloco de comparação
            /// @return 
            {/********************************************/

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
