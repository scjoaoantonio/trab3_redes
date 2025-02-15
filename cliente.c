#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/time.h>

#define PORTA_PADRAO 8080
#define TAM_PACOTE 1024
#define TIMEOUT 2
#define PROB_PERDA 10

typedef struct {
    int num_seq;
    int tamanho;
    char dados[TAM_PACOTE];
} Pacote;

int simular_perda() {
    return (rand() % 100) < PROB_PERDA;
}

void enviar_arquivo(int sock, struct sockaddr_in servidor, char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    Pacote pacote;
    int num_seq = 0, bytes_lidos, servidor_len = sizeof(servidor);
    int total_enviados = 0, total_retransmissoes = 0;

    struct timeval timeout = {TIMEOUT, 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    while ((bytes_lidos = fread(pacote.dados, 1, TAM_PACOTE, arquivo)) > 0) {
        pacote.num_seq = num_seq;
        pacote.tamanho = bytes_lidos;
        int ack_recebido = 0;

        while (!ack_recebido) {
            if (!simular_perda()) {
                sendto(sock, &pacote, sizeof(pacote), 0, (struct sockaddr *)&servidor, servidor_len);
                printf("Enviado pacote %d\n", num_seq);
                total_enviados++;
            } else {
                printf("Simulando perda do pacote %d\n", num_seq);
            }

            int ack;
            if (recvfrom(sock, &ack, sizeof(int), 0, NULL, NULL) > 0 && ack == num_seq) {
                printf("ACK recebido para pacote %d\n", num_seq);
                ack_recebido = 1;
            } else {
                printf("Timeout, retransmitindo pacote %d\n", num_seq);
                total_retransmissoes++;
            }
        }
        num_seq++;
    }
    fclose(arquivo);

    pacote.num_seq = -1;
    pacote.tamanho = 0;
    sendto(sock, &pacote, sizeof(pacote), 0, (struct sockaddr *)&servidor, servidor_len);
    printf("Fim da transmissão enviado.\n");

    printf("Total de pacotes enviados: %d\n", total_enviados);
    printf("Total de pacotes retransmitidos: %d\n", total_retransmissoes);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <IP> <porta> <arquivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Erro ao criar socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in servidor;
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &servidor.sin_addr);

    enviar_arquivo(sock, servidor, argv[3]);
    close(sock);
    return EXIT_SUCCESS;
}
