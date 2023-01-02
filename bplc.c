#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256


///     Le uma linha do arquivo de entrada, verificando um caracter por vez
int read_line(char line[LINESZ]){
    int i = 0;
    int ch;

    for(i = 0; i < LINESZ; i++){
        if((ch = fgetchar()) != EOF && ch != '\0'){
            line[i] = (char) ch;
        }
        else{
            line[i] = '\0';
            break;
        }
    }

    if(line[0] == '\0'){
        return 0;
    }
    return 1;
}

int main (){
    FILE *arquivo_S;
    char line[LINESZ];


    if((arquivo_S = fopen("Assembly.S", "w")) == NULL) {
        printf("Cannot open file.\n");
        exit (1);
    }

    while (read_line(line) != 0){
        fputs(line, arquivo_S);
    }


    return 0;
}
