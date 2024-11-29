#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 1024  


char message[MAX_MESSAGE_SIZE];
int message_index = 0;
char caractere_courant = 0;
int compteur_bits = 0;


void gestionnaire_signaux(int signal) {
    
    caractere_courant = (caractere_courant << 1) | (signal == SIGUSR2);
    compteur_bits++;


    if (compteur_bits == 8) {
        if (caractere_courant == '\n') { 
            message[message_index] = '\0';  
            printf("Message reçu : %s\n", message);
            message_index = 0;
        } else if (message_index < MAX_MESSAGE_SIZE - 1) {
            message[message_index++] = caractere_courant;  
        }
      
        caractere_courant = 0;
        compteur_bits = 0;
    }
}

int main() {
    printf("Serveur démarré...\n");
    printf("Mon PID : %d\n", getpid());

   
    struct sigaction sa;
    sa.sa_handler = gestionnaire_signaux;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("Erreur lors de la configuration des signaux");
        exit(EXIT_FAILURE);
    }

    printf("En attente de messages...\n");

    while (1) {
        pause();
    }

    return 0;
}
