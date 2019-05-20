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

#define PORT 3000
#define MSG_SIZE 4096
struct sockaddr_in local;
struct sockaddr_in remoto;

int main()
{
    int sockfd, client;
    int tam_remoto = sizeof(remoto);
    int slen;
    char buffer[4096];

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
    listen(sockfd, 1);
    client = accept(sockfd, (struct sockaddr *)&remoto, &tam_remoto);

    //Checa se houve erro na conexão do cliente
    if (client == -1)
    {
        perror("Error client");
        exit(1);
    }

    strcpy(buffer, "Bem Vindo ao Sistema de Arquivos XingLing!\n\0");

    if (send(client, buffer, strlen(buffer), 0))
    {
        printf("Aguardando ...\n");
        while (1)
        {
            memset(buffer, 0x0, MSG_SIZE);
            if ((slen = recv(client, buffer, MSG_SIZE, 0)) > 0)
            {
                buffer[slen - 1] = '\0';
                printf("Recebido: %s\n", buffer);
                close(client);
                break;
            }
        }
    }
    close(sockfd);
    printf("Servidor Encerrado!\n");
    return 0;
}