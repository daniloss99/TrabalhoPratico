#include "common.cpp"

#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <iostream>
#include <string>
#include <map> 

#define VERSAO 4
#define CARACTERES 80

using namespace std; 

//testando se muda alguma coisa

std::map<string, int> vetor_roteamento; 
string char_to_string(const char palavra[]);

int periodo; 

int main(int argc, char **argv) {

    if (argc > 3) 
    {
        printf("Argumentos demais para o servidor. Revise as intrucoes \n"); 
    }

    char versao[3]; 

    if(VERSAO == 6)
    {
        strcpy(versao, "v6"); 
    }
    else if(VERSAO == 4)
    {
        strcpy(versao, "v4"); 
    }
    else
        printf("A versao selecionada nao corresponde nem a ipv4 nem a ipv6. Revise a variavel versao \n"); 
        
    //DEFININDO O PERIODO DE ATUALIZACAO COM BASE NO ARGUMENTO PASSADO
    periodo = atoi(argv[2]); 

    //DETERMINANDO A PORT ONDE VAI ACONTECER A CONEXÃO     
    uint16_t port = 55151;
    if (port == 0) {
        printf("Porta indefinida. Revise o argumento de execucao \n"); 
    }
    port = htons(port); // colocando a porta no formato de network short - previamente em host

    //DETERMINANDO O ENDEREÇO ONDE VAI OCORRER A CONEXÃO
    char endereco_de_conexao[CARACTERES];
    strcpy(endereco_de_conexao, argv[1]); 

    //INCIALIZANDO A CONEXÃO PARA O CASO IPV4
    if(strcmp(versao, "v4") == 0) //se for ipv4
    {
        struct in_addr inaddr4; 
        inet_pton(AF_INET, endereco_de_conexao, &inaddr4); 
        struct sockaddr_in servaddr;        

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr = inaddr4;
        servaddr.sin_port = port;

        int socket_;
        socket_ = socket(AF_INET, SOCK_DGRAM, 0);
        if(socket_ == 0)
            printf("Erro ao fazer o socket \n"); 

        // Fazendo a etapa de bind, ou seja, comunicando ao cliente que ele está apto a mandar informações
        if (bind(socket_, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
        {
            printf("Algo deu errado na etada de binding...\n");
            //exit(0);
        }
        else
            printf("Bind realizado. \n");
        
        socklen_t len = sizeof(servaddr); 

        func(socket_, (struct sockaddr*)&servaddr, &len); 

        close(socket_);
    }

    else
    {

    //INCIALIZANDO A CONEXÃO PARA O CASO IPV6
    if(strcmp(versao, "v6") == 0)
    {
        struct sockaddr_in6 servaddr; 
        struct in_addr inaddr6; 
        inet_pton(AF_INET6, endereco_de_conexao, &inaddr6); 

        bzero((char *) &servaddr, sizeof(servaddr));
        servaddr.sin6_flowinfo = 0;
        servaddr.sin6_family = AF_INET6;
        memcpy(&(servaddr.sin6_addr), &inaddr6, sizeof(inaddr6));
        servaddr.sin6_port = port;

        int socket_;
        socket_ = socket(AF_INET6, SOCK_DGRAM, 0);
        if(socket_ == 0)
            printf("Erro ao fazer o socket \n");

        // Fazendo a etapa de bind, ou seja, comunicando ao cliente que ele está apto a mandar informações
        if (bind(socket_, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
        {
            printf("Algo deu errado na etada de binding...\n");
            //exit(EXIT_FAILURE);
        }
        else
            printf("Bind realizado. \n");

                
        socklen_t len = sizeof(servaddr); 

        func(socket_, (struct sockaddr*)&servaddr, &len); 

        close(socket_); 
    }


    else
        printf("Erro ao selecionar a versao.\n"); 

    }

    return 0; 
}

