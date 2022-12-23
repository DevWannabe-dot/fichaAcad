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
typedef struct exercicio_estrutura {
	char nomeMaquina[TAMANHO_NOME];	// "Supino maquina"
	unsigned series;				// 3
	unsigned reps;					// 15
	float carga_kg;					// Carga em kg (inicializado 0.00kg para ser indefinido)
} exercicio_t;

typedef struct treino_estrutura {
	// Criado para possibilitar adicionar v�rias pr�ticas j� cadastradas ao treino
	unsigned* ExerPorIDs;			// IDs das pr�ticas, sem estrutura* ou ** para fins de grava��o em arquivo
	unsigned nExer;					// n�mero de pr�ticas para viabilizar a aloca��o din�mica
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
uint8_t carregaExercicios(exercicio_t** exercicios, FILE* arquivo, long* cursor) {
	return 1;
	
	return 0;
}

uint8_t carregaTreinos(exercicio_t** exercicios, treino_t** treinos, FILE* arquivo, long* cursor) {
	// Treino A, B, C... Z
	return carregaExercicios(exercicios, arquivo, cursor) + 1;
	
	return false;
}

uint8_t carregaUsuarios(exercicio_t** exercicios, treino_t** treinos, usuario_t** usuarios, FILE* arquivo, long* cursor) {
	
	while (!feof(arquivo)) {
		// fscanf
		return carregaTreinos(exercicios, treinos, arquivo, cursor) + 1;
	}
	return false;
}

bool carregaAcad(exercicio_t** exercicios, treino_t** treinos, usuario_t** usuarios, academia_t* academia) {
	FILE* arquivo;
	int i = 1;
	long cursor = 0;
	char nome[TAMANHO_NOME];
	char str1[sizeof(unsigned long long) + 1];
	char str2[] = ".txt";
	uint8_t status_carregamento = 0;

	arquivo = fopen("db.bin", "rb");

	if (arquivo) {
		fseek(arquivo, 0, SEEK_SET);

		while (!feof(arquivo)) {
			fread(nome, sizeof(char), TAMANHO_NOME, arquivo);
			printf("Academia %i: %s;", i, nome);
			i++;
		}
		printf("Deseja entrar em qual das academias registradas? ");
		scanf("%i", &i);
		if (!i) return false;

		fseek(arquivo, (i-- * sizeof(academia_t)), SEEK_SET);
		fread(&academia->nome, sizeof(char), TAMANHO_NOME, arquivo);
		fread(&academia->CNPJ, sizeof(unsigned long long), 1, arquivo);
		fread(&academia->endereco, sizeof(char), TAMANHO_ENDERECO, arquivo);
		fread(&academia->email, sizeof(char), TAMANHO_EMAIL, arquivo);
		fread(&academia->telefone, sizeof(unsigned long long), 1, arquivo);

		fclose(arquivo);
		
		// academia.txt
		util_converteInteiroParaTexto((int)academia->CNPJ, str1);
		strcpy(nome, str1);
		strncat(nome, str2, 4);
		arquivo = fopen(nome, "r");

		if (arquivo) {
			status_carregamento += carregaUsuarios(exercicios, treinos, usuarios, arquivo, &cursor);
			
			switch (status_carregamento) {
			case 0:
				break;
			case 1:
				usuarios_section:
				printf("<Usuarios encontrados!>\n");
				break;
			case 2:
				treinos_section:
				printf("<Treinos encontrados!>\n");
				goto usuarios_section;
				break;
			case 3:
				printf("<Exercicios encontrados!>\n");
				goto treinos_section;
				break;
			default:
				fprintf(stderr, "<Erro desconhecido.>\n");
				break;
			}

			fclose(arquivo);
		}
		
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
	treino_t* treinos = NULL;
	exercicio_t* exercicios = NULL;
	int nMatriculas = 0;
	char backup[TAMANHO_ENDERECO];

	setlocale(LC_CTYPE, "Portuguese");

	// Le arquivo db.bin e cada arquivo de texto correspondente � matr�cula
	printf("<LENDO CREDENCIAIS DA ACADEMIA...>\n");
	exercicios = (exercicio_t*)realloc(exercicios, sizeof(exercicio_t));
	treinos = (treino_t*)realloc(treinos, sizeof(treino_t));
	usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas + 1));

	if (!carregaAcad(&exercicios, &treinos, &usuarios, &academia)) {
		// Leitura dos dados da academia atual
		memcpy(academia.nome, '\0', TAMANHO_NOME);
		academia.CNPJ = 0;
		memcpy(academia.endereco, '\0', TAMANHO_ENDERECO);
		memcpy(academia.email, '\0', TAMANHO_EMAIL);
		academia.telefone = 0;


		printf("<INSIRA 0 PARA PULAR UM PASSO>\n");
		printf("Nome da academia: ");
		fgets(backup, TAMANHO_NOME, stdin);
		if (backup[0] != 0) {
			strcpy(academia.nome, backup);
			util_removeQuebraLinhaFinal(academia.nome);
		}

		do {
			printf("CNPJ (obrigat�rio): ");
			scanf("%llu%c", &academia.CNPJ, &lixo);
		} while (!(academia.CNPJ));

		printf("Onde se encontra a academia? ");
		fgets(backup, TAMANHO_ENDERECO, stdin);
		if (backup[0] != 0) {
			strcpy(academia.endereco, backup);
			util_removeQuebraLinhaFinal(academia.endereco);
		}

		printf("E-mail de atendimento: ");
		fgets(backup, TAMANHO_EMAIL, stdin);
		if (backup[0] != 0) {
			strcpy(academia.email, backup);
			util_removeQuebraLinhaFinal(academia.email);
		}

		printf("Telefone de atendimento: ");
		scanf("%llu%c", &academia.telefone, &lixo);
	}

	// Leitura dos usu�rios da academia
	
	// salvamento em arquivo <db.bin>

	return SUCESSO;
}