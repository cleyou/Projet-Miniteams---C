#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


void envoyer_caractere(pid_t pid_serveur, char caractere) {
    for (int i = 7; i >= 0; --i) {
        int bit = (caractere >> i) & 1;  
        int signal = (bit == 0) ? SIGUSR1 : SIGUSR2;  
        if (kill(pid_serveur, signal) == -1) {
            perror("Erreur lors de l'envoi du signal");
            exit(EXIT_FAILURE);
        }
        usleep(1000);  
    }
}


void envoyer_message(pid_t pid_serveur, const char *message) {
    while (*message) {
        envoyer_caractere(pid_serveur, *message++);
    }
    envoyer_caractere(pid_serveur, '\n');  
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Utilisation : %s <pid_serveur> <message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid_serveur = atoi(argv[1]);
    if (pid_serveur <= 0) {
        fprintf(stderr, "PID du serveur invalide\n");
        return EXIT_FAILURE;
    }


    char message[1024] = "";
    for (int i = 2; i < argc; ++i) {
        strcat(message, argv[i]);
        if (i < argc - 1) {
            strcat(message, " ");
        }
    }

    printf("Envoi du message au serveur (PID : %d) : %s\n", pid_serveur, message);
    envoyer_message(pid_serveur, message);

    printf("Message envoyé avec succès.\n");
    return EXIT_SUCCESS;
}
