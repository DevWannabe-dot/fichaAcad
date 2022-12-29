#pragma once
#ifndef __MAIN_h__
#define __MAIN_h__

// Inclusoes

// Constantes
#define SUCESSO				(0)

#define TAMANHO_NOME		(50+1)
#define TAMANHO_ENDERECO	(100+1)
#define TAMANHO_EMAIL		(50+1)
#define TAMANHO_ALONGAR		(50+1)
#define NUM_TREINOS_MAX		(26)

// Tipos
typedef struct exercicio_estrutura {
	char nomeMaquina[TAMANHO_NOME];	// "Supino maquina"
	unsigned series;				// 3
	unsigned reps;					// 15
	float carga_kg;					// Carga em kg (inicializado 0.00kg para ser indefinido)
} exercicio_t;

typedef struct treino_estrutura {
	// Criado para possibilitar adicionar várias práticas já cadastradas ao treino
	exercicio_t* exercicios;		// acessar com operador * e escrever diretamente no arquivo
	unsigned nExer;					// número de práticas para viabilizar a alocação dinâmica
	char alongar[TAMANHO_ALONGAR];	// ex. panturrilhas, lombar
	// O treino desejado é referenciado por um char identificador = 65 (A)...

	/* time.h
	void inicio;
	void termino;
	void reaval;
	*/
} treino_t;

typedef struct usuario_estrutura {
	unsigned matricula;					// Matrícula do usuário, até 4294967295
	char nome[TAMANHO_NOME];			// Nome completo do usuário
	int TreinosPorIDs[NUM_TREINOS_MAX];	// Ponteiro para a struct treinos
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

// Funcoes

#endif