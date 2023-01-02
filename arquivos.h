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

uint8_t carregaUsuarios(usuario_t** usuarios, int* nMatriculas, FILE* arquivo, long* cursor) {

	while (!feof(arquivo)) {
		fseek(arquivo, 0, SEEK_SET);
		fscanf(arquivo, "%i", nMatriculas);
		*cursor = ftell(arquivo);
		return 1;
	}

	return false;
}

uint8_t carregaAcad(usuario_t** usuarios, academia_t* academia, int* nMatriculas) {
	FILE* arquivo;
	long cursor = 0;
	char nome[TAMANHO_NOME];

	uint8_t status_carregamento = 0;

	arquivo = fopen("db.bin", "rb");

	if (arquivo) {
		fseek(arquivo, 0, SEEK_SET);

		// loop mantido apenas para evitar ler um arquivo vazio
		while (!feof(arquivo)) {
			fread(&academia->nome, sizeof(char), TAMANHO_NOME, arquivo);
			printf("Academia: %s...\n\n", academia->nome);
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
			status_carregamento++;
			status_carregamento += carregaUsuarios(usuarios, nMatriculas, arquivo, &cursor);

			switch (status_carregamento) {
			case 0:
				break;
			case 1:
			academia_section:
				printf("<Academia encontrada!>\n");
				break;
			case 2:
				printf("<Usuarios encontrados!>\n");
				goto academia_section;
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

void salvaTudo(usuario_t* usuarios, academia_t academia) {
	int i = 0;
	FILE* arquivo = fopen("db.bin", "wb");
	char nome[TAMANHO_NOME];

	if (arquivo) {
		fseek(arquivo, 0, SEEK_SET);

		fwrite(&academia.nome, sizeof(char), TAMANHO_NOME, arquivo);
		fwrite(&academia.CNPJ, sizeof(unsigned long long), 1, arquivo);
		fwrite(&academia.endereco, sizeof(char), TAMANHO_ENDERECO, arquivo);
		fwrite(&academia.email, sizeof(char), TAMANHO_EMAIL, arquivo);
		fwrite(&academia.telefone, sizeof(unsigned long long), 1, arquivo);

		// Matriculas (nome)
		while (usuarios[i].matricula) {
			fwrite(&usuarios[i].matricula, sizeof(unsigned), 1, arquivo);
			fwrite(&usuarios[i].nome, sizeof(char), TAMANHO_NOME, arquivo);
			i++;
		}

		fclose(arquivo);
	}
	criarNomeArquivo(&academia, nome);
	arquivo = fopen(nome, "w");
}

#endif