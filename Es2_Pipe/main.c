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
    pid_t ritorno_fork = fork();
    if (ritorno_fork == -1) {
        perror("fork");
        exit(-2);
    }
    if (!ritorno_fork) {
        close(pipe1[1]);
        close(pipe2[0]);
        char stringa_ricevuta[100];
        read(pipe1[0], stringa_ricevuta, sizeof(stringa_ricevuta));
        close(pipe1[0]);
        char stringa_maiuscola[100];
        int numero_vocali = 0;
        for (int i = 0; stringa_ricevuta[i] != '\0'; i++) {
            char c = stringa_ricevuta[i];
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
                c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
                numero_vocali++;
            }
            if (c >= 'a' && c <= 'z') {
                stringa_maiuscola[i] = c - 32;
            } else {
                stringa_maiuscola[i] = c;
            }
            stringa_maiuscola[i + 1] = '\0';
        }
        write(pipe2[1], stringa_maiuscola, sizeof(stringa_maiuscola));
        write(pipe2[1], &numero_vocali, sizeof(numero_vocali));
        close(pipe2[1]);
        exit(0);
    }
    close(pipe1[0]);
    close(pipe2[1]);
    char input_utente[100];
    printf("Inserisci una stringa di testo: ");
    scanf("%s", input_utente);
    write(pipe1[1], input_utente, sizeof(input_utente));
    close(pipe1[1]);
    char buffer_maiuscolo[100];
    int vocali_ricevute = 0;
    read(pipe2[0], buffer_maiuscolo, sizeof(buffer_maiuscolo));
    read(pipe2[0], &vocali_ricevute, sizeof(vocali_ricevute));
    close(pipe2[0]);
    printf("Messaggio convertito dal figlio: %s\n", buffer_maiuscolo);
    printf("Numero totale di vocali: %d\n", vocali_ricevute);
    return 0;
}