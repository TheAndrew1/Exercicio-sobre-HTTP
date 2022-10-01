#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mostrar_linha(FILE *protocolo, char *argv[]);
void separar_cabecalho(FILE *protocolo);

typedef struct {
	char tipo[20];
	char metodo[20];
	char url[1000];
	char versao[10];
} solicitacao;

typedef struct {
	char tipo[20];
	char versao[10];
	char codigo[5];
	char frase[50];
} resposta;

typedef struct {
	char chave[100];
	char valor[1000];
} cabecalho;

int main(int argc, char *argv[]) {
	
	FILE *protocolo;
	protocolo = fopen(argv[1], "r");

	printf("\n============================INTERPRETADOR PROTOCOLO HTTP============================\n\n");
	mostrar_linha(protocolo, argv);

	printf("\n======================================CABECALHO======================================\n\n");
	separar_cabecalho(protocolo);

	printf("\n========================================CORPO========================================\n\n");
	char corpo[10000];
	fread(corpo, sizeof(char), 10000, protocolo);
	printf("%s\n", corpo);

	fclose(protocolo);
	return 0;
}

void mostrar_linha(FILE *protocolo, char *argv[])
{
	char pega_linha[3];

	if(argv[1][0] == 'p')
	{
		solicitacao linha;
		strcpy(linha.tipo, "Solicitacao");
		printf("Tipo de Mensagem: %s\n", linha.tipo);
		fscanf(protocolo, "%s", linha.metodo);
		printf("Metodo: %s\n", linha.metodo);
		fscanf(protocolo, "%s", linha.url);
		printf("URL: %s\n", linha.url);
		fscanf(protocolo, "%s", linha.versao);
		printf("Versao: %s\n", linha.versao);
		fgets(pega_linha, 3, protocolo);
	}
	else if(argv[1][0] == 'r')
	{
		resposta linha;
		strcpy(linha.tipo, "Resposta");
		printf("Tipo de Mensagem: %s\n", linha.tipo);
		fscanf(protocolo, "%s", linha.versao);
		printf("Versao: %s\n", linha.versao);
		fscanf(protocolo, "%s", linha.codigo);
		printf("Codigo: %s\n", linha.codigo);
		fgets(linha.frase, 50, protocolo);
		printf("Frase: %s", linha.frase);
	}
}

void separar_cabecalho(FILE *protocolo)
{
	int n = 1;
	cabecalho *info;
	info = (cabecalho *)calloc(n, sizeof(cabecalho));

	while(1)
	{
		char pega_linha[3];
		
		fscanf(protocolo, "%s", info[n-1].chave);
		printf("%s", info[n-1].chave);
		fgets(info[n-1].valor, 1000, protocolo);
		printf("%s", info[n-1].valor);

		int pos = ftell(protocolo);
		fgets(pega_linha, 3, protocolo);
		pos = ftell(protocolo) - pos;

		if(!strcmp(pega_linha, "\n") || feof(protocolo))
			break;
		else
		{
			fseek(protocolo, -pos, 1);
			n += 1;
			info = realloc(info, n*sizeof(cabecalho));
			
		}
	}
}