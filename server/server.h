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
#define BUFFER_SIZE 4096
#define N_THREAD 10

//Declaração buffer, diretório raiz e o diretório atual
char buffer[BUFFER_SIZE], dir_raiz[BUFFER_SIZE], dir_atual[BUFFER_SIZE], *temp;
;

// Inicializa thread
pthread_mutex_t mutex_manage_client = PTHREAD_MUTEX_INITIALIZER;

//Função onde a thread rodará
void *connect_thread(void *arg);

//Funções para executar comandos digitados pelos clientes
void criar_diretorio(int new_sockfd);
void remover_diretorio(int new_sockfd);
void entrar_diretorio(int new_sockfd, char *temp);
void mostrar_conteudo_diretorio(int new_sockfd);
void criar_arquivo(int new_sockfd);
void remover_arquivo(int new_sockfd);
void escrever_arquivo(int new_sockfd);
void mostrar_arquivo(int new_sockfd);
