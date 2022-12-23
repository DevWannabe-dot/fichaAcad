/*
* @file		main.c
* @brief	C�digo principal do projeto
* @author	Pedro Henrique Pinto de Oliveira
* @date		2022-12-21
*/

/* ----------- METAS ----------- */
//	Template da fun��o para realizar a impress�o, com par�metros e argumentos passados
//	Leitura e escrita em arquivo .txt, humanly-readable, possibilidade de salvar A, B, C... Z treinos por usu�rio
/* -----------		 ----------- */

// Inclus�es
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <locale.h>
#include "util.h"

// Constantes
#define SUCESSO				(0)
#define TAMANHO_NOME		(50+1)
#define TAMANHO_ENDERECO	(100+1)
#define TAMANHO_EMAIL		(50+1)
#define TAMANHO_ALONGAR		(50+1)

// Tipos
typedef struct pratica_estrutura {
	char nomeMaquina[TAMANHO_NOME];	// "Supino maquina"
	unsigned series;				// 3
	unsigned reps;					// 15
	float carga_kg;					// Carga em kg (inicializado 0.00kg para ser indefinido)
} pratica_t;

typedef struct treino_estrutura {
	// Criado para possibilitar adicionar v�rias pr�ticas j� cadastradas ao treino
	unsigned* PraticasPorIDs;		// IDs das pr�ticas, sem estrutura* ou ** para fins de grava��o em arquivo
	unsigned nPraticas;				// n�mero de pr�ticas para viabilizar a aloca��o din�mica
	char alongar[TAMANHO_ALONGAR];	// ex. panturrilhas, lombar
	// O treino desejado � referenciado por um char identificador = 65 (A)...
	
	/* time.h
	void inicio;
	void termino;
	void reaval;
	*/
} treino_t;

typedef struct usuario_estrutura {
	unsigned matricula;				// Matr�cula do usu�rio, at� 4294967295
	char nome[TAMANHO_NOME];		// Nome completo do usu�rio
	int* TreinosPorIDs;				// Ponteiro para a struct treinos
	// sem vari�vel auxiliar nTreinos, ir� de 0 at� 25 (A a Z) conferindo if(usuario[i]) antes de executar a itera��o
} usuario_t;

typedef struct academia_estrutura {
	char nome[TAMANHO_NOME];		// Nome da academia
	unsigned long long CNPJ;		// CNPJ
	char endereco[TAMANHO_ENDERECO];// Endere�o da academia
	char email[TAMANHO_EMAIL];		// Email de atendimento ao cliente da academia
	unsigned long long telefone;	// Telefone de atendimento ao cliente da academia
	// Struct com o intuito de ser variavel e n�o vetor, todos os membros inicializados com 0 ou \0 caso o usu�rio n�o queira registr�-los
} academia_t;

// Fun��es
bool carregaTudo(academia_t* academia) {
	FILE* arquivo;
	int i = 1;

	arquivo = fopen("db.bin", "rb");
	if (arquivo) {
		while (!feof(arquivo)) {
			// fread
			printf("Academia %i: %s;", i, academia->nome);
			i++;
		}
		printf("Deseja entrar em qual das academias registradas? ");
		scanf("%i", &i);
		// fseek + fread

		fclose(arquivo);
		return true;
	}
	return false;
}

void listaMatriculas(void) {
	
}

int main(int argc, char** argv) {
	char lixo;
	academia_t academia;
	usuario_t* usuarios = NULL;
	int nMatriculas = 0;

	setlocale(LC_CTYPE, "Portuguese");

	// Le arquivo db.bin e cada arquivo de texto correspondente � matr�cula
	printf("<LENDO CREDENCIAIS DA ACADEMIA...>\n");
	if (!carregaTudo(&academia)) {
		// Leitura dos dados da academia atual
		printf("<INSIRA 0 PARA PULAR UM PASSO>\n");
		printf("Nome da academia: ");
		fgets(academia.nome, TAMANHO_NOME, stdin);
		util_removeQuebraLinhaFinal(academia.nome);

		printf("CNPJ: ");
		scanf("%llu%c", &academia.CNPJ, &lixo);

		printf("Onde se encontra a academia? ");
		fgets(academia.endereco, TAMANHO_ENDERECO, stdin);
		util_removeQuebraLinhaFinal(academia.endereco);

		printf("E-mail de atendimento: ");
		fgets(academia.email, TAMANHO_EMAIL, stdin);
		util_removeQuebraLinhaFinal(academia.email);

		printf("Telefone de atendimento: ");
		scanf("%llu%c", &academia.telefone, &lixo);
	}

	// Leitura dos usu�rios da academia
	usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas+1));


	// salvamento em arquivo <db.bin>

	return SUCESSO;
}