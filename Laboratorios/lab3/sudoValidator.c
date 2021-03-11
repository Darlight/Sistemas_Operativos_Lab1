
/*
Universidad del Valle de Guatemala
Sistemas operativos
Ing. Erick Pineda
sudokoValidator.c
Proposito: Dado una solucion en txt, se verifica si la solucion es correcta
Lab3
*/
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <omp.h>

//Sudoko instanciado de caracteres strins
char sudoku[9][9];
int correct_columns, correct_rows;

// Verificacion de filas en el sudoku
int verify_rows()
{
    //Paralelizacion nested 
    omp_set_nested(1);
    // 9 hilos para cada celda/columna
    omp_set_num_threads(9);

    int grid[9];
    int valid = 0;
    
    #pragma omp parallel for private(grid) schedule(dynamic)
    for (int i = 0; i < 9; i++)
    {
        char nums_validos[] = "123456789";
        char *num;
        //Basado en panda, descartananddo \0
        for (num = &nums_validos[0]; *num != '\0'; num++)
        {
            
            int not_num = 0;
            int j = 0;
            while (not_num == 0 && j < 9)
            {
                if (sudoku[i][j] == *num)
                    not_num = 1;
                j++;
            }
            if (not_num == 0)
                valid = -1;
        }
        printf("En la verificacion de las filas el hilo en proceso es: %ld \n", syscall(SYS_gettid));
    }
    return valid;
}

int verify_columns()
{   
    //Paralelizacion nested 
    omp_set_nested(1);
    omp_set_num_threads(9);

    int grid[9];
    int valid = 0;
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < 9; i++)
    {
        char nums_validos[] = "123456789";
        char *num;
         //Basado en panda, descartananddo \0
         
        for (num = &nums_validos[0]; *num != '\0'; num++)
        {
            int not_num = 0;
            int j = 0;
            while (not_num == 0 && j < 9)
            {
                if (sudoku[j][i] == *num)
                    not_num = 1;
                j++;
            }
            if (not_num == 0)
                valid = -1;
        }
        printf("En la verificacion de las columnas el hilo en proceso es: %ld \n", syscall(SYS_gettid));
    }
    return valid;
}


int verify_rows_nums(char temp[9][9])
{
    omp_set_nested(1);
    omp_set_num_threads(9);

    int grid[9];
    int valid = 0;
    #pragma omp parallel for private(grid) schedule(dynamic)
    for (int i = 0; i < 9; i++)
    {
        char nums_validos[] = "123456789";
        char *num;
        //Basado en panda, descartananddo \0
        for (num = &nums_validos[0]; *num != '\0'; num++)
        {
            int not_num = 0;
            int j = 0;
            while (not_num == 0 && j < 9)
            {
                if (temp[i][j] == *num)
                    not_num = 1;
                j++;
            }
            if (not_num == 0)
                valid = -1;
        }
    }
    return valid;
}


//Verificacion de los subarrays sacados de: https://www.geeksforgeeks.org/check-if-given-sudoku-solution-is-valid-or-not/
int verify_Subarrays()
{
    omp_set_nested(1);
    omp_set_num_threads(3);

    char temp_sudoku[9][9];
    
    int row = 0, column = 0;

    int grid[9];
    // #pragma omp parallel for private(grid)
    for (int x = 0; x < 3; x++)
    {
        //#pragma omp parallel for
        for (int y = 0; y < 3; y++)
        {
            // #pragma omp parallel for
            for (int i = 0; i < 3; i++)
            {
                // #pragma omp parallel for
                for (int j = 0; j < 3; j++)
                {
                    temp_sudoku[9][9] = sudoku[i + (x * 3)][j + (y * 3)];
                    column++;
                }
            }
            column = 0;
            row++;
        }
    }
    return verify_rows_nums(temp_sudoku);
}

void *complete_column_verification()
{
    printf("Hijo Columna ID: %ld \n", syscall(SYS_gettid));
    correct_columns = verify_columns();
    pthread_exit(0);
}

void *complete_row_verification()
{
    printf("Hijo Fila ID: %ld \n", syscall(SYS_gettid));
    correct_rows = verify_rows();
    pthread_exit(0);
}


/* Maps Sudoku string to array */
void mapping_Sudoku(int fd)
{
    //Paralelizacion nested 
    omp_set_nested(1);
    omp_set_num_threads(9);

    struct stat stat_s;
    int status_sudoku = fstat(fd, &stat_s);
    int size = stat_s.st_size;
    //Mappeando el archivo txt
    char *ptr = (char *)mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);

    int char_pos = 0;
    int grid[9];
    #pragma omp parallel for private(grid) schedule(dynamic)
    for (int i = 0; i < 9; i++)
    {
        #pragma omp parallel for
        for (int j = 0; j < 9; j++)
        {
            sudoku[i][j] = ptr[char_pos];
            char_pos++;
        }
    }
    munmap(ptr,size);
    close(fd);
}

int main(int argc, char *argv[])
{
    omp_set_num_threads(1);
    //gcc -fopenmp -pthreads -o sudokoValidator sudokoValidator.c
    if (argc < 2)
    {
        printf("Archivo de suduko ingresado incorrectamente. \n");
        return 1;
    }

    int input;
    // Lee el txt, y saca error en caso no se encuentra el archivo txt
    if ((input = open(argv[1], O_RDONLY)) < 0)
    {
        perror("Archivo de sudoko que fue ingresado no se puede abrir. \n");
        return 1;
    }
    else
    {
        mapping_Sudoku(input);
        // Idetinficiacion del padre antes del primer hijo
        pid_t parent_pid = getpid();

        int child = fork();
        if (child < 0)
        {
            perror("Error de hacer Fork.");
            return 1;
        }
        else if (child == 0)
        {
            // Identidades a strings del proceso hijo
            char p_pid[6];
            sprintf(p_pid, "%d", (int)parent_pid);
            execlp("ps", "ps", "-p", p_pid, "-lLf", NULL);
        }
        else
        {
            // Proceso padre al inicio del programa
            pthread_t col_verification;
            // instancia de los hilos que verifica las columnas
            if (pthread_create(&col_verification, NULL, complete_column_verification, NULL))
            {
                perror("Error al crear el Hilo");
                return 1;
            }
            if (pthread_join(col_verification, NULL))
            {
                perror("Error al intentar unirse el hilo.");
                return 1;
            }

            printf("Hijo principal es : %ld \n", syscall(SYS_gettid));
            usleep(30000);
            printf("Hijo terminado ... \n");

            // instancia de los hilos que verifica las filas
            pthread_t row_verification;
            if (pthread_create(&row_verification, NULL, complete_row_verification, NULL))
            {
                perror("Error al crear el Hilo");
                return 1;
            }
            if (pthread_join(row_verification, NULL))
            {
                perror("Error al intentar unirse el hilo.");
                return 1;
            }

            // Verificacion del sudoku
            if (correct_rows == 0 && correct_columns == 0)
            {
                printf("|--- Solucion Valida ---| \n");
            }
            else
            {
                printf("|--- Solucion Invalida :/ ---|\n");
            }

        // Segundo hijo
        int child2 = fork();
        if (child2 == 0)
        { 
            //Mismo procedimiento que el anterior
            char p_pid[6];
            sprintf(p_pid, "%d", (int)parent_pid);
            execlp("ps", "ps", "-p", p_pid, "-lLf", NULL);
        }
        else
        {
            //Proceso sigue en el padre
            usleep(30000);
            printf("Hijo terminado... \n");
            return 0;
        }
    }

}
}