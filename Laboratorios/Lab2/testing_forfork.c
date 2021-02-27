#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//compilar en terminal es gcc -o ejecutable programa.c
// ejecutar es ./ejecutable y enter

int main(){
    int sum, count = 0;
    for(count = 0; count <= 3; ++count){
        fork();
        printf(" Hola \n");
    }

    return 0;
}