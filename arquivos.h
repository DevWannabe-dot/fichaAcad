#pragma once
#ifndef __ARQUIVOS_h__
#define __ARQUIVOS_h__

// Inclusoes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "util.h"
#include "main.h"

// Constantes

// Tipos

// Funcoes
void criarNomeArquivo(academia_t* academia, char nome[]) {
	char str1[sizeof(unsigned long long) + 1];
	char str2[] = ".txt";

	util_converteInteiroParaTexto((int)academia->CNPJ, str1);
	strcpy(nome, str1);
	strncat(nome, str2, 4);
}

uint8_t carregaExercicios(exercicio_t** exercicios, FILE* arquivo, long* cursor) {
	return 1;

	return 0;
}

uint8_t carregaTreinos(exercicio_t** exercicios, treino_t** treinos, FILE* arquivo, long* cursor) {
	// Treino A, B, C... Z
	return carregaExercicios(exercicios, arquivo, cursor) + 1;

	return false;
}

uint8_t carregaUsuarios(exercicio_t** exercicios, treino_t** treinos, usuario_t** usuarios, int* nMatriculas, FILE* arquivo, long* cursor) {

	while (!feof(arquivo)) {
		fseek(arquivo, 0, SEEK_SET);
		fscanf(arquivo, "%i", nMatriculas);
		*cursor = ftell(arquivo);
		return carregaTreinos(exercicios, treinos, arquivo, cursor) + 1;

	}

	return false;
}

uint8_t carregaAcad(exercicio_t** exercicios, treino_t** treinos, usuario_t** usuarios, academia_t* academia, int* nMatriculas) {
	FILE* arquivo;
	long cursor = 0;
	char nome[TAMANHO_NOME];

	uint8_t status_carregamento = 0;

	arquivo = fopen("db.bin", "rb");

	if (arquivo) {
		status_carregamento++;
		fseek(arquivo, 0, SEEK_SET);

		// loop mantido apenas para evitar ler um arquivo vazio
		while (!feof(arquivo)) {
			fread(&academia->nome, sizeof(char), TAMANHO_NOME, arquivo);
			printf("Academia: %s...\n", academia->nome);
			fread(&academia->CNPJ, sizeof(unsigned long long), 1, arquivo);
			fread(&academia->endereco, sizeof(char), TAMANHO_ENDERECO, arquivo);
			fread(&academia->email, sizeof(char), TAMANHO_EMAIL, arquivo);
			fread(&academia->telefone, sizeof(unsigned long long), 1, arquivo);
			break;
		}

		fclose(arquivo);

		// academia.txt
		criarNomeArquivo(academia, nome);
		arquivo = fopen(nome, "r");

		if (arquivo) {
			status_carregamento += carregaUsuarios(exercicios, treinos, usuarios, nMatriculas, arquivo, &cursor);

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
	}
	return status_carregamento;
}

void salvaTudo(exercicio_t* exercicios, treino_t* treinos, usuario_t* usuarios, academia_t academia) {
	FILE* arquivo = fopen("db.bin", "wb");
	char nome[TAMANHO_NOME];

	if (arquivo) {
		fseek(arquivo, 0, SEEK_SET);

		fwrite(&academia.nome, sizeof(char), TAMANHO_NOME, arquivo);
		fwrite(&academia.CNPJ, sizeof(unsigned long long), 1, arquivo);
		fwrite(&academia.endereco, sizeof(char), TAMANHO_ENDERECO, arquivo);
		fwrite(&academia.email, sizeof(char), TAMANHO_EMAIL, arquivo);
		fwrite(&academia.telefone, sizeof(unsigned long long), 1, arquivo);

		fclose(arquivo);
	}
	criarNomeArquivo(&academia, nome);
	arquivo = fopen(nome, "w");
}

#endif