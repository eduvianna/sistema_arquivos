#include "server.h"

void criar_diretorio(int new_sockfd)
{

    if (system(buffer) == 0)
    {
        strcpy(buffer, "\033[0;32m Diretório criado com sucesso.\033[0m\n\0");
    }
    else
    {
        strcpy(buffer, "\033[0;31m Erro ao criar diretório.\033[0m\n\0");
    }
    send(new_sockfd, buffer, strlen(buffer), 0);
}

void remover_diretorio(int new_sockfd)
{
    if (system(buffer) == 0)
    {
        strcpy(buffer, "\033[0;32m Diretório removido com sucesso.\033[0m\n\0");
    }
    else
    {
        strcpy(buffer, "\033[0;31m Erro ao remover diretório.\033[0m\n\0");
    }
    send(new_sockfd, buffer, strlen(buffer), 0);
}

void entrar_diretorio(int new_sockfd, char *temp)
{
    if (chdir(temp) == 0)
    {
        strcpy(buffer, "\033[0;32m Acessório acessado com sucesso.\033[0m\n\0");
    }
    else
    {
        strcpy(buffer, "\033[0;31m Erro ao acessar o diretório.\033[0m\n\0");
    }
    send(new_sockfd, buffer, strlen(buffer), 0);
}

void mostrar_conteudo_diretorio(int new_sockfd)
{
    FILE *fp;
    char aux[255];

    fp = popen(buffer, "r");
    while (fgets(aux, sizeof(aux), fp) != NULL)
    {
        send(new_sockfd, aux, strlen(aux), 0);
    }
}

void criar_arquivo(int new_sockfd)
{
    if (system(buffer) == 0)
    {
        strcpy(buffer, "\033[0;32m Arquivo criado com sucesso.\033[0m\n\0");
    }
    else
    {
        strcpy(buffer, "\033[0;31m Erro ao criar o arquivo.\033[0m\n\0");
    }
    send(new_sockfd, buffer, strlen(buffer), 0);
}

void remover_arquivo(int new_sockfd)
{
    if (system(buffer) == 0)
    {
        strcpy(buffer, "\033[0;32m Arquivo removido com sucesso.\033[0m\n\0");
    }
    else
    {
        strcpy(buffer, "\033[0;31m Erro ao remover diretório.\033[0m\n\0");
    }
    send(new_sockfd, buffer, strlen(buffer), 0);
}

void escrever_arquivo(int new_sockfd)
{
    if (system(buffer) == 0)
    {

        strcpy(buffer, "\033[0;32m Arquivo editado com sucesso.\033[0m\n\0");
    }
    else
    {
        strcpy(buffer, "\033[0;31m Erro ao editar arquivo.\033[0m\n\0");
    }
    send(new_sockfd, buffer, strlen(buffer), 0);
}

void mostrar_arquivo(int new_sockfd)
{
    FILE *fp;
    char aux[255];

    fp = popen(buffer, "r");
    while (fgets(aux, sizeof(aux), fp) != NULL)
    {
        send(new_sockfd, aux, strlen(aux), 0);
    }
}
