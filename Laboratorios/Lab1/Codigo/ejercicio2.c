#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#define MODE 0666
#define BUF_SIZE 8192
// Inputs son de dos archivos txt. Uno con el contenido y el otro vacio.
// El archivo txt vacio sera usado para pegar el contenido del primer archivo txt.
int main(int argc, char*argv[]){
    // i = inicio, j = final
    int original, copy, i, j;
    // tamanio del archivo txt
    char size[BUF_SIZE];
    if (argc !=3) exit(1);
    original = open(argv[1], O_RDONLY);
    if (original < 0) exit (2);
    copy = creat(argv[2], MODE); if (copy < 0) exit(3);

    while (1){
        i = read(original,size,BUF_SIZE);
        if (i <=0) break;
        j = write(copy,size,i);
        if (j <= 0) break;
    }
    close(original);
    close(copy);
    exit(0);
}