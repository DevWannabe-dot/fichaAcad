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
	// Criado para possibilitar adicionar v�rias pr�ticas j� cadastradas ao treino
	exercicio_t* exercicios;		// acessar com operador * e escrever diretamente no arquivo
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
	unsigned matricula;					// Matr�cula do usu�rio, at� 4294967295
	char nome[TAMANHO_NOME];			// Nome completo do usu�rio
	int TreinosPorIDs[NUM_TREINOS_MAX];	// Ponteiro para a struct treinos
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

// Funcoes

#endif