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
void criarNomeArquivo(academia_t* academia, char nome[], const int tamanho, int numeral) {
	char* str1 = malloc(sizeof(char) * tamanho);
	char str2[] = ".txt";

	util_converteInteiroParaTexto(numeral, str1);
	if (str1) strcpy(nome, str1);
	strncat(nome, str2, 4);

	// tamanho <= tamanho de nome
}

uint8_t carregaUsuarios(usuario_t* usuarios, int* nMatriculas, FILE* arquivo, long* cursor) {
	int i = 0;

	if (!feof(arquivo)) {
		fseek(arquivo, (*cursor), SEEK_SET);
		fread(nMatriculas, sizeof(int), 1, arquivo);
		while (!feof(arquivo)) {
			fread(&usuarios[i].matricula, sizeof(unsigned), 1, arquivo);
			fread(&usuarios[i].nome, sizeof(char), TAMANHO_NOME, arquivo);
			i++;
		}
		*cursor = ftell(arquivo);
		return 1;
	}

	return false;
}

uint8_t carregaAcad(usuario_t* usuarios, academia_t* academia, int* nMatriculas) {
	FILE* arquivo;
	long cursor = 0;
	char nome[TAMANHO_NOME];
	int i = 0;

	uint8_t status_carregamento = 0;

	arquivo = fopen("db.bin", "rb");

	if (arquivo) {
		fseek(arquivo, 0, SEEK_SET);

		if (!feof(arquivo)) {
			fread(&academia->nome, sizeof(char), TAMANHO_NOME, arquivo);
			printf("Academia: %s...\n\n", academia->nome);
			fread(&academia->CNPJ, sizeof(unsigned long long), 1, arquivo);
			fread(&academia->endereco, sizeof(char), TAMANHO_ENDERECO, arquivo);
			fread(&academia->email, sizeof(char), TAMANHO_EMAIL, arquivo);
			fread(&academia->telefone, sizeof(unsigned long long), 1, arquivo);

			cursor = ftell(arquivo);
		}

		if (arquivo) {
			status_carregamento += 1 + carregaUsuarios(usuarios, nMatriculas, arquivo, &cursor);

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

void salvaTudo(usuario_t* usuarios, academia_t academia, int nMatriculas) {
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

		fwrite(&nMatriculas, sizeof(int), 1, arquivo);

		// Matriculas (nome)
		while (nMatriculas) {
			fwrite(&usuarios[i].matricula, sizeof(unsigned), 1, arquivo);
			fwrite(&usuarios[i].nome, sizeof(char), TAMANHO_NOME, arquivo);
			nMatriculas--;
			i++;
		}

		fclose(arquivo);
	}
	criarNomeArquivo(&academia, nome, POW_2_64_CARACTERES, (int)academia.CNPJ);
	arquivo = fopen(nome, "w");
}

#endif