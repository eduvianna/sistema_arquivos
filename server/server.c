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
#include <unistd.h>

#define PORT 3000
#define MSG_SIZE 5000
#define BUFFER_SIZE 4096
#define N_THREAD 10

//Declaração buffer e client message
char client_message[MSG_SIZE],
    buffer[BUFFER_SIZE];

pthread_mutex_t mutex_manage_client = PTHREAD_MUTEX_INITIALIZER;

//void criar_diretorio();
//void remover_diretorio();
//void entrar_diretorio();
void mostrar_conteudo_diretorio();
//void criar_arquivo();
//void remover_arquivo();
void escrever_arquivo();
void mostrar_arquivo();

void *connect_thread(void *arg)
{
    int size_msg;
    int new_sockfd = *((int *)arg);

    //Tratamento de mensagens entre cliente e servidor
    while (1)
    {
        memset(buffer, 0x0, MSG_SIZE);
        if ((size_msg = recv(new_sockfd, buffer, BUFFER_SIZE, 0)) > 0)
        {
            pthread_mutex_lock(&mutex_manage_client); // Checa se o mutex está no estado liberado ou não
            buffer[size_msg - 1] = '\0';

            //Checa se o comando é para criar diretório
            if (strncmp(buffer, "mkdir", 4) == 0)
            {
                if (system(buffer) == -1)
                {
                    strcpy(buffer, "Erro ao criar diretório.\n\0");
                }
                else
                {
                    strcpy(buffer, "Diretório criado com sucesso.\n\0");
                }
                send(new_sockfd, buffer, strlen(buffer), 0);
            }

            //Checa se o comando é para remover diretório
            else if ((strncmp(buffer, "rm -r", 4) == 0) && (strncmp(buffer, "rm", 1) == 0))
            {
                if (system(buffer) == -1)
                {
                    strcpy(buffer, "Erro ao remover diretório.\n\0");
                }
                else
                {
                    strcpy(buffer, "Comando executado com sucesso.\n\0");
                }
                send(new_sockfd, buffer, strlen(buffer), 0);
            }

            //Checa se o comando é para acessar um diretório
            else if (strncmp(buffer, "cd", 2) == 0)
            {
                if (system(buffer) == -1)
                {
                    strcpy(buffer, "Erro ao acessar o diretório.\n\0");
                }
                else
                {
                    strcpy(buffer, "Comando executado com sucesso.\n\0");
                }
                send(new_sockfd, buffer, strlen(buffer), 0);
            }

            //Checa se o comando é para criar um arquivo
            else if (strncmp(buffer, "touch", 2) == 0)
            {
                if (system(buffer) == -1)
                {
                    strcpy(buffer, "Erro ao criar o arquivo.\n\0");
                }
                else
                {
                    strcpy(buffer, "Comando executado com sucesso.\n\0");
                }
                send(new_sockfd, buffer, strlen(buffer), 0);
            }

            //Checa se o comando é para remover um arquivo
            else if ((strncmp(buffer, "rm", 1) == 0) && (strncmp(buffer, "rm -r ", 4) != 0))
            {
                if (system(buffer) == -1)
                {
                    strcpy(buffer, "Erro ao remover diretório.\n\0");
                }
                else
                {
                    strcpy(buffer, "Comando executado com sucesso.\n\0");
                }
                send(new_sockfd, buffer, strlen(buffer), 0);
            }

            //Checa se o comando é para sair do terminal
            else if (strcmp(buffer, "exit") == 0) //Encerra conexão com um cliente quando o mesmo digitar exit
            {
                strcpy(buffer, "Até\n\0");
                send(new_sockfd, buffer, strlen(buffer), 0);
                pthread_mutex_unlock(&mutex_manage_client); // Libera o mutex para o próximo
                close(new_sockfd);
                pthread_exit(NULL);
            }

            //Qualquer outro comando é invalido
            else
            {
                strcpy(buffer, "Comando Inválido\n\0");
                send(new_sockfd, buffer, strlen(buffer), 0);
            }

            strcpy(buffer, "-> \0");
            send(new_sockfd, buffer, strlen(buffer), 0);
            pthread_mutex_unlock(&mutex_manage_client); // Libera o mutex para o próximo
        }
    }
    sleep(1);
}

int main()
{
    //Estrutura necessaria para a criação do socket
    struct sockaddr_in local;
    struct sockaddr_storage remote;
    socklen_t size_remote = sizeof(remote);

    int sockfd, client;

    //Criando N_THREADS
    pthread_t client_thread[N_THREAD];

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Define descritor de socket com ipv4 com tcp

    if (sockfd == -1)
    { // Checka se houve erro na criação do socket
        perror("Error Socket ");
        exit(1);
    }
    else
    {
        printf("Socket Criado \n");
    }

    // Seta os valores para a configuração do servidor
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    memset(local.sin_zero, 0x0, 8);

    //cria o link entra o ip e a porta para então realizar a conexão
    if (bind(sockfd, (struct sockaddr *)&local, sizeof(local)) == -1)
    {
        perror("Error bind ");
        exit(1);
    }

    //Coloca a porta para escutar
    listen(sockfd, N_THREAD);

    int cont = 0; // contador de threads criadas
    while (1)
    {
        client = accept(sockfd, (struct sockaddr *)&remote, &size_remote);

        //Checa se houve erro na conexão do cliente
        if (client == -1)
        {
            perror("Error client");
            exit(1);
        }
        strcpy(buffer, "Bem Vindo ao Sistema de Arquivos XingLing!\n Para sair do sistema apenas digite 'exit'\n -> \0");
        if (send(client, buffer, strlen(buffer), 0))
        {
            if ((pthread_create(&client_thread[cont], NULL, connect_thread, &client)) != 0)
            {
                perror("Error Thread ");
            }
            if (cont >= N_THREAD)
            {
                cont = 0;
                while (cont < N_THREAD)
                {
                    pthread_join(client_thread[cont++], NULL);
                }
                cont = 0;
            }
        }
    }
    close(sockfd); // Encerra conexão do servidor
    printf("Servidor Encerrado!\n");
    return 0;
}