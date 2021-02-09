#include <stdio.h>
#include <unistd.h>
//Primer Programa
int main(){
    printf("Hello, World! \n")
    printf("%d\n", (int) getpid());
    execl("ejercicio1.c",(char*)NULL);
    return 0;
}

// Compilar en terminal es gcc -o ejercicio1 ejercicio1.c
// ejecutar es ./ejercicio1 y enter