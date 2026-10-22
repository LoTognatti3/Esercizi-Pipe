#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 10

int main(void) {
int array[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int fd[2];
    pid_t pid;
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(-1);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(-1);
    }
    if (pid == 0) {
        close(fd[0]);
        int somma_pari = 0;
        for (int i = 0; i < SIZE; i++) {
            if (array[i] % 2 == 0) {
                somma_pari += array[i];
            }
        }
        write(fd[1], &somma_pari, sizeof(somma_pari));
        close(fd[1]);
        exit(0);
    }
    else {
        close(fd[1]);
        int somma_dispari = 0;
        int somma_pari_ricevuta = 0;
        for (int i = 0; i < SIZE; i++) {
            if (array[i] % 2 != 0) {
                somma_dispari += array[i];
            }
        }
        read(fd[0], &somma_pari_ricevuta, sizeof(somma_pari_ricevuta));
        close(fd[0]);
        wait(NULL);
        int somma_totale = somma_dispari + somma_pari_ricevuta;
        printf("Somma numeri PARI (calcolata dal figlio): %d\n", somma_pari_ricevuta);
        printf("Somma numeri DISPARI (calcolata dal padre): %d\n", somma_dispari);
        printf("Somma TOTALE complessiva: %d\n", somma_totale);
    }
    return 0;
}
