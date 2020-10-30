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

std::map<string, int> vetor_roteamento; 
string char_to_string(const char palavra[]);

int periodo; 

//FUNCOES AUXILIARES
void obtem_comando(char entrada[], char command[], char arg1[], int &arg2) //recebe um comando com um ou dois parametros do teclado e armazena cada um separadamente
{
	const char s[2] = " "; 
	char *palavra;

	palavra = strtok(entrada, s); 
	strcpy(command, palavra); 
	
	palavra = strtok(NULL, s); 
	strcpy(arg1, palavra); 
	
	if(strcmp(command, "add") == 0)
	{
		palavra = strtok(NULL, s); 
		arg2 = atoi(palavra);   
	}
}

void salva_endereco(string endereco_ip, int peso)
{
    if(vetor_roteamento[endereco_ip] != 0)
    	cout << "Peso de " << endereco_ip << " atualizado para " << peso << endl; 
    else 
        cout << "Peso " << peso << " salvo para o endereco ip " << endereco_ip << endl; 

    vetor_roteamento[endereco_ip] = peso; //já silenciosamente substitui em caso de chave já inicializada 
    map<string, int>::iterator it = vetor_roteamento.begin();
    
    while(it != vetor_roteamento.end())
    {
        cout << it->first << " : " << it->second << endl;
        it++;
    }
}

void deleta_endereco(string endereco)
{
    std::map<string, int>::iterator it;
    it = vetor_roteamento.find(endereco); 
    vetor_roteamento.erase(it);  
}

int consulta(string endereco_ip)
{
    int peso = vetor_roteamento[endereco_ip]; 
    return peso; 
}

string char_to_string(const char palavra[])
{
    int len = strlen(palavra); 
    string palavra_string; 
    for(int i = 0; i < len; i++)
    {
        palavra_string.push_back(palavra[i]); 
    }
    return palavra_string; 
}

//FUNCAO PRINCIPAL 
void func(int socket_, struct sockaddr* src_addr, socklen_t *addrlen)
{
    char entrada[CARACTERES];     
    char comando[10]; 
    char argumento1[50];
    int argumento2; 

    string arg1; 
    int arg2; 

    while(1)
    {      
        cout << "Digite o comando que deseja executar: " << endl; 
        cin.getline(entrada,sizeof(entrada));

        obtem_comando(entrada, comando, argumento1, argumento2); 

        if(strcmp(comando, "add") == 0)
        {
            arg1 = char_to_string(argumento1); 
            arg2 = argumento2; 
            salva_endereco(arg1, arg2); 
        }

        else if(strcmp(comando, "del") == 0)
        {
            arg1 = char_to_string(argumento1); 
            deleta_endereco(argumento1);  
        }

        else if(strcmp(comando, "quit") == 0)
        {
            break; 
        }

        memset(&entrada, 0, sizeof(entrada)); 
        memset(&comando, 0, sizeof(comando)); 
        memset(&argumento1, 0, sizeof(argumento1)); 
        memset(&argumento2, 0, sizeof(argumento2));  
    }
    
}


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

