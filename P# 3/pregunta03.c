#include <stdio.h>
#include <string.h>
#include "mpi.h"

//NOMBRE: VICTOR GABRIEL CAPIA ALI
//CI: 4762494 LP
//PREGUNTA 3. Realice lo mismo (PREGUNTA 2)con MPI utilizando al menos 4 hilos.

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtiene el rango del hilo actual
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtiene el tamaño del comunicador

    if (size < 4) {
        printf("Se requieren al menos 4 hilos para ejecutar este programa.\n");
        MPI_Finalize();
        return 1;
    }

    char frase[] = "tres tristes tigres trigaban trigo por culpa del bolivar";
    char primera_mitad[100];
    char segunda_mitad[100];

    if (rank == 0) {  // El hilo 0 divide la frase en palabras y construye las mitades
        char *palabras[100];
        int palabra_count = 0;

        // Divide la frase en palabras utilizando strtok
        char *token = strtok(frase, " ");
        while (token != NULL) {
            palabras[palabra_count++] = token;
            token = strtok(NULL, " ");
        }

        // Construye la primera mitad con palabras en índices pares
        primera_mitad[0] = '\0'; // Inicializa con cadena vacía
        for (int i = 0; i < palabra_count; i += 2) {
            strcat(primera_mitad, palabras[i]);
            if (i + 2 < palabra_count) {
                strcat(primera_mitad, " ");
            }
        }

        // Construye la segunda mitad con palabras en índices impares
        segunda_mitad[0] = '\0'; // Inicializa con cadena vacía
        for (int i = 1; i < palabra_count; i += 2) {
            strcat(segunda_mitad, palabras[i]);
            if (i + 2 < palabra_count) {
                strcat(segunda_mitad, " ");
            }
        }
    }

    MPI_Bcast(primera_mitad, 100, MPI_CHAR, 0, MPI_COMM_WORLD);  // Difunde la primera mitad
    MPI_Bcast(segunda_mitad, 100, MPI_CHAR, 0, MPI_COMM_WORLD);  // Difunde la segunda mitad

    if (rank == 1) {
        printf("Hilo %d - Primera Mitad: %s\n", rank, primera_mitad);
    } else if (rank == 2) {
        printf("Hilo %d - Segunda Mitad: %s\n", rank, segunda_mitad);
    }

    MPI_Finalize();
    return 0;
}

