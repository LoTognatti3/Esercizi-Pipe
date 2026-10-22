#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(-1);
    }
    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(-1);
    }
    pid_t pid_b = fork();
    if (pid_b == -1) {
        perror("fork");
        exit(-2);
    }
    if (!pid_b) {
        close(pipe1[1]);
        close(pipe2[0]);
        int numero_ricevuto;
        while (read(pipe1[0], &numero_ricevuto, sizeof(numero_ricevuto)) > 0) {
            if (numero_ricevuto >= 50) {
                int quadrato = numero_ricevuto * numero_ricevuto;
                write(pipe2[1], &quadrato, sizeof(quadrato));
            }
        }
        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    }
    pid_t pid_c = fork();
    if (pid_c == -1) {
        perror("fork");
        exit(-2);
    }
    if (!pid_c) {
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);
        int quadrato_ricevuto;
        int somma = 0;
        int conteggio = 0;
        printf("--- Processo C: Numeri ricevuti da B ---\n");
        while (read(pipe2[0], &quadrato_ricevuto, sizeof(quadrato_ricevuto)) > 0) {
            printf("Ricevuto quadrato: %d\n", quadrato_ricevuto);
            somma += quadrato_ricevuto;
            conteggio++;
        }
        close(pipe2[0]);
        if (conteggio > 0) {
            double media = ((double)somma / conteggio)/2;
            printf("Quantità numeri validi %d\n", conteggio);
            printf("Media aritmetica dei quadrati %f\n", media);
        } else {
            printf("Nessun numero era maggiore o uguale a 50.\n");
        }

        exit(0);
    }
    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe2[1]);
    printf("Processo A: Generazione e invio di 20 numeri\n");
    for (int i = 0; i < 20; i++) {
        int num_casuale = (rand() % 100) + 1;
        printf("Inviato numero [%d/20]: %d\n", i + 1, num_casuale);
        write(pipe1[1], &num_casuale, sizeof(num_casuale));
    }
    close(pipe1[1]);
    wait(NULL);
    wait(NULL);
    printf("\nProcesso Padre: Tutti i figli hanno terminato.\n");
    return 0;
}
