#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORTA_PADRAO 8080
#define TAM_PACOTE 1024
#define PROB_PERDA 10

typedef struct
{
    int num_seq;
    char dados[TAM_PACOTE];
} Pacote;

int simular_perda()
{
    return (rand() % 100) < PROB_PERDA;
}

void receber_arquivo(int sock)
{
    FILE *arquivo = fopen("arquivo_recebido", "wb");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo para escrita");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in cliente;
    socklen_t cliente_len = sizeof(cliente);
    Pacote pacote;
    int ultimo_num_seq = -1;

    while (1)
    {
        int recebido = recvfrom(sock, &pacote, sizeof(pacote), 0, (struct sockaddr *)&cliente, &cliente_len);
        if (recebido > 0)
        {
            if (pacote.num_seq == ultimo_num_seq)
            {
                printf("Pacote duplicado %d descartado\n", pacote.num_seq);
            }
            else
            {
                fwrite(pacote.dados, 1, TAM_PACOTE, arquivo);
                printf("Recebido pacote %d\n", pacote.num_seq);
                ultimo_num_seq = pacote.num_seq;
            }

            if (!simular_perda())
            {
                sendto(sock, &ultimo_num_seq, sizeof(int), 0, (struct sockaddr *)&cliente, cliente_len);
                printf("ACK enviado para pacote %d\n", ultimo_num_seq);
            }
            else
            {
                printf("Simulando perda do ACK %d\n", ultimo_num_seq);
            }
        }
    }

    fclose(arquivo);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <porta>\n", argv[0]);
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Erro ao criar socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in servidor;
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(atoi(argv[1]));
    servidor.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        perror("Erro ao associar socket");
        return EXIT_FAILURE;
    }

    printf("Servidor aguardando pacotes na porta %s...\n", argv[1]);
    receber_arquivo(sock);

    close(sock);
    return EXIT_SUCCESS;
}
