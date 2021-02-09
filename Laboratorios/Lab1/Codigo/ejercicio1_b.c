#include <stdio.h>
#include <unistd.h>
//Primer Programa, segunda parte
int main(){
    int f = fork();
    printf("Hello, World! \n")
    printf("%d\n", (int) getpid());
    if (f == 0){
        printf("Hello from Child! \n");
        execl("ejercicio1_b.c", (char*)NULL);
    }
    else{
        printf("Hello from Parent! \n");
        execl("ejercicio1_b.c",(char*)NULL);
    }
    execl("ejercicio1.c",(char*)NULL);
    return 0;
}

// Compilar en terminal es gcc -o ejercicio1 ejercicio1.c
// ejecutar es ./ejercicio1 y enter