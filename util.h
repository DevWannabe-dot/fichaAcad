#pragma once
#ifndef __UTIL_h__
#define __UTIL_h__

// Inclusoes
#include <stdio.h>
#include <string.h>

// Funcoes
/* ---------- UTILIDADES ------------------------------ */
void util_converteInteiroParaTexto(int numero, char texto[]) {
	sprintf(texto, "%d", numero);
}

void util_imprimeTracinhos(int nTracinhos) {

	int i;

	for (i = 0; i < nTracinhos; i++) {
		printf("-");
	}
}

void util_removeQuebraLinhaFinal(char dados[]) {
	int tamanho;
	tamanho = strlen(dados);
	if ((tamanho > 0) && (dados[tamanho - 1] == '\n')) {
		dados[tamanho - 1] = '\0';
	}
}

#endif