#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//compilar en terminal es gcc -o ejecutable programa.c
// ejecutar es ./ejecutable y enter

int main(){
    fork();
    fork();
    fork();
    fork();
    printf("Hola \n");

    return 0;
}