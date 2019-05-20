/*
Titulo: Servidor para um Sistema de Arquivos
Objetivo: Este arquivo tem como objetivo gerenciar uma conexão socket TCP entre um servidor e varios clientes,
          onde cada cliente ficará sendo gerenciada por threads e semafóros e mutex.
Autor: Eduardo da Costa Viana
Git: https://github.com/eduvianna/sistema_arquivos.git
*/

//Bibliotecas Padrões C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> //Biblioteca para gerenciar erros

//Bibliotecas para socket e outras funções
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

//Biblioteca para threads
#include <pthread.h>

#define PORT 3000
#define MSG_SIZE 5000
#define BUFFER_SIZE 1000
#define N_THREAD 10

//Declaração buffer e client message
char client_message[MSG_SIZE],
    buffer[BUFFER_SIZE];

void *connect_thread(void *arg)
{
    int new_sockfd = *((int *)arg);
    recv(new_sockfd, client_message, MSG_SIZE, 0);

    //Tratamento de mensagens entre cliente e servidor
    pthread_mutex_lock(&lock);
    char *message = malloc(sizeof(client_message) + 20);
    strcpy(message, "Bem Vindo ao Sistema de Arquivos XingLing!\n\0");
    strcat(message, client_message);
    strcpy(buffer, message);
    free(message);
    pthread_mutex_unlock(&lock);
    sleep(1);
    send(new_sockfd, buffer, 13, 0);
    printf("Até Mais! \n");
    close(new_sockfd);
    pthread_exit(NULL);
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
struct sockaddr_in local;
struct sockaddr_in remote;

int main()
{

    int sockfd, client;
    int size_remote = sizeof(remote);
    int slen;

    //Criando N_THREADS
    pthread_t client_thread[N_THREAD];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    { // Checka se houve erro na criação do socket
        perror("Error Socket ");
        exit(1);
    }
    else
    {
        printf("Socket Criado \n");
    }

    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    //local.sin_addr.s_addr = inet_addr("IP");
    memset(local.sin_zero, 0x0, 8);

    //cria o link entra o ip e a porta para então realizar a conexão
    if (bind(sockfd, (struct sockaddr *)&local, sizeof(local)) == -1)
    {
        perror("Error bind ");
        exit(1);
    }

    //Coloca a porta para escutar
    listen(sockfd, N_THREAD);

    strcpy(buffer, "Bem Vindo ao Sistema de Arquivos XingLing!\n\0");

    int i = 0;
    while (1)
    {
        client = accept(sockfd, (struct sockaddr *)&remote, &size_remote);

        //Checa se houve erro na conexão do cliente
        if (client == -1)
        {
            perror("Error client");
            exit(1);
        }

        if (send(client, buffer, strlen(buffer), 0))
        {
            pthread_create(&client_thread[i], NULL, connect_thread);
            printf("Aguardando ...\n");
            while (1)
            {
                memset(buffer, 0x0, MSG_SIZE);
                if ((slen = recv(client, buffer, MSG_SIZE, 0)) > 0)
                {
                    buffer[slen - 1] = '\0';
                    printf("Recebido: %s\n", buffer);
                    if (strcmp(buffer, "exit") == 0)
                    {
                        close(client);
                        break;
                    }
                }
            }
        }
    }
    close(sockfd);
    printf("Servidor Encerrado!\n");
    return 0;
}