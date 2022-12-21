/*
* @file		main.c
* @brief	Código principal do projeto
* @author	Pedro Henrique Pinto de Oliveira
* @date		2022-12-21
*/

/* ----------- METAS ----------- */
//	Template da função para realizar a impressão, com parâmetros e argumentos passados
//	Leitura e escrita em arquivo .txt, humanly-readable, possibilidade de salvar A, B, C... Z treinos por usuário
/* -----------		 ----------- */

// Inclusões
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

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
	// Criado para possibilitar adicionar várias práticas já cadastradas ao treino
	unsigned* PraticasPorIDs;		// IDs das práticas, sem estrutura* ou ** para fins de gravação em arquivo
	unsigned nPraticas;				// número de práticas para viabilizar a alocação dinâmica
	char alongar[TAMANHO_ALONGAR];	// ex. panturrilhas, lombar
	// O treino desejado é referenciado por um char identificador = 65 (A)...
} treino_t;

typedef struct usuario_estrutura {
	unsigned matricula;				// Matrícula do usuário, até 4294967295
	char nome[TAMANHO_NOME];		// Nome completo do usuário
	int* TreinosPorIDs;				// Ponteiro para a struct treinos
	// sem variável auxiliar nTreinos, irá de 0 até 25 (A a Z) conferindo if(usuario[i]) antes de executar a iteração
} usuario_t;

typedef struct academia_estrutura {
	char nome[TAMANHO_NOME];		// Nome da academia
	unsigned long long CNPJ;		// CNPJ
	char endereco[TAMANHO_ENDERECO];// Endereço da academia
	char email[TAMANHO_EMAIL];		// Email de atendimento ao cliente da academia
	unsigned long long telefone;	// Telefone de atendimento ao cliente da academia
	// Struct com o intuito de ser variavel e não vetor, todos os membros inicializados com 0 ou \0 caso o usuário não queira registrá-los
} academia_t;

// Funções
void listaMatriculas(void) {
	
}

int main(int argc, char** argv) {
	
	academia_t academia;
	usuario_t* usuarios = NULL;
	int nMatriculas = 0;

	// Le arquivo db.bin e cada arquivo de texto correspondente à matrícula

	// Leitura dos dados da academia atual

	// Leitura dos usuários da academia
	usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas+1));


	// salvamento em arquivo <db.bin>

	return SUCESSO;
}