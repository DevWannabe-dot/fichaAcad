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

uint8_t carregaUsuarios(usuario_t* usuarios, int* nMatriculas, FILE* arquivo_c, long* cursor) {
	int i = 0;

	if (!feof(arquivo_c)) {
		fseek(arquivo_c, (*cursor), SEEK_SET);
		fread(nMatriculas, sizeof(int), 1, arquivo_c);

		usuarios = (usuario_t*)realloc(usuarios, (*nMatriculas + 1));

		while (!feof(arquivo_c)) {
			printf("CURSOR = %i", ftell(arquivo_c));
			fread(&usuarios[i].matricula, sizeof(unsigned), 1, arquivo_c);
			fread(&usuarios[i].nome, sizeof(char), TAMANHO_NOME, arquivo_c);
			i++;
		}
		*cursor = ftell(arquivo_c);
		return 1;
	}

	return false;
}

uint8_t carregaAcad(usuario_t* usuarios, academia_t* academia, int* nMatriculas) {
	FILE* arquivo_c;
	long cursor = 0;
	int i = 0;

	uint8_t status_carregamento = 0;

	arquivo_c = fopen("db.bin", "rb");

	if (arquivo_c) {
		fseek(arquivo_c, 0, SEEK_SET);

		if (!feof(arquivo_c)) {
			fread(&academia->nome, sizeof(char), TAMANHO_NOME, arquivo_c);
			printf("Academia: %s...\n\n", academia->nome);
			fread(&academia->CNPJ, sizeof(unsigned long long), 1, arquivo_c);
			fread(&academia->endereco, sizeof(char), TAMANHO_ENDERECO, arquivo_c);
			fread(&academia->email, sizeof(char), TAMANHO_EMAIL, arquivo_c);
			fread(&academia->telefone, sizeof(unsigned long long), 1, arquivo_c);

			cursor = ftell(arquivo_c);
		}

		status_carregamento += 1 + carregaUsuarios(usuarios, nMatriculas, arquivo_c, &cursor);

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

		// fclose(arquivo_c);
	}
	return status_carregamento;
}

void salvaTudo(usuario_t* usuarios, academia_t academia, int nMatriculas) {
	int i = 0;
	FILE* arquivo_s = fopen("db.bin", "wb");
	char nome[TAMANHO_NOME];

	if (arquivo_s) {
		fseek(arquivo_s, 0, SEEK_SET);

		fwrite(&academia.nome, sizeof(char), TAMANHO_NOME, arquivo_s);
		fwrite(&academia.CNPJ, sizeof(unsigned long long), 1, arquivo_s);
		fwrite(&academia.endereco, sizeof(char), TAMANHO_ENDERECO, arquivo_s);
		fwrite(&academia.email, sizeof(char), TAMANHO_EMAIL, arquivo_s);
		fwrite(&academia.telefone, sizeof(unsigned long long), 1, arquivo_s);

		fwrite(&nMatriculas, sizeof(int), 1, arquivo_s);

		// Matriculas (nome)
		while (nMatriculas) {
			fwrite(&usuarios[i].matricula, sizeof(unsigned), 1, arquivo_s);
			fwrite(&usuarios[i].nome, sizeof(char), TAMANHO_NOME, arquivo_s);
			nMatriculas--;
			i++;
		}

		fclose(arquivo_s);
	}
	criarNomeArquivo(&academia, nome, POW_2_64_CARACTERES, (int)academia.CNPJ);
	arquivo_s = fopen(nome, "w");
}

#endif