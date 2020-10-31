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

using namespace std;

#define VERSAO 4
#define CARACTERES 80

std::map<string, int> vetor_roteamento; 
string char_to_string(const char palavra[]);

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










