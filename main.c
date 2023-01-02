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
#include <time.h>
#include "util.h"
#include "main.h"
#include "arquivos.h"

// Constantes

// Tipos

// Fun��es

void imprimeUsuarioUnico(usuario_t* usuarioAtual) {
	printf("%u: %s\n", usuarioAtual->matricula, usuarioAtual->nome);
}

void listaMatriculas(usuario_t* usuarios, int nMatriculas) {
	for (int i = 0; i < nMatriculas; i++) {
		imprimeUsuarioUnico(&usuarios[i]);
	}
}

void atribuirTreino(usuario_t* usuarioAtual) {
	char lixo;
	int escolha, nTreinos = 0;

	// listar treinos

	printf("Qual treino da lista deseja adicionar (0 PARA INTERROMPER)? ");
	scanf("%i%c", &escolha, &lixo);
}

void cadastrarUsuarios(usuario_t* usuarioAtual){
	char escolha, lixo;
	usuarioAtual->matricula = 0;
	
	printf("(0 PARA INTERROMPER)\nMatr�cula do usu�rio: ");
	scanf("%u%c", &usuarioAtual->matricula, &lixo);	
	if (usuarioAtual->matricula == 0) return;
	
	printf("Nome do usu�rio: ");
	fgets(usuarioAtual->nome, TAMANHO_NOME, stdin);
	util_removeQuebraLinhaFinal(usuarioAtual->nome);

	printf("Deseja atribuir treinos a este usu�rio (S/N)?");
	scanf("%c%c", &escolha, &lixo);

	switch (escolha) {
	case 'S':
	case 's':
		atribuirTreino(usuarioAtual);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	char lixo, backup[TAMANHO_ENDERECO], escolha;
	int nMatriculas = 0, opcao = 0;
	uint8_t status_carregamento;
	academia_t academia;
	usuario_t* usuarios = NULL;
	bool privilegiosAdmin = false;

	setlocale(LC_CTYPE, "Portuguese");

	// Le arquivo db.bin e cada arquivo de texto correspondente � matr�cula
	printf("<LENDO CREDENCIAIS DA ACADEMIA...>\n");
	usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas + 1));

	// Leitura dos dados da academia atual
	status_carregamento = carregaAcad(&usuarios, &academia, &nMatriculas);
	if (!status_carregamento) {
		memset(academia.nome, '\0', TAMANHO_NOME);
		academia.CNPJ = 0;
		memset(academia.endereco, '\0', TAMANHO_ENDERECO);
		memset(academia.email, '\0', TAMANHO_EMAIL);
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
	do {
		util_imprimeTracinhos(20);
		printf("\nACADEMIA %s\n", academia.nome);
		printf("0 - SAIR\n"
			"1 - Listar matr�culas\n"
			"2 - Acessar usu�rio (matr�cula)\n"
			"3 - Procurar matr�cula (nome)\n"
			"4 - ADMIN\n"); // criar senha caso n�o exista, ir� em db.bin
		// da� em diante, criar novo usu�rio e atribuir treinos
		if (privilegiosAdmin) {
			printf("5 - Cadastrar usu�rio\n"
					"6 - Editar academia\n");
		}
		util_imprimeTracinhos(5);
		printf(">> ");
		
		
		scanf("%i%c", &opcao, &lixo);

		switch (opcao) {
		case 0:
			// sair
			break;
		case 1:
			if (nMatriculas) {
				listaMatriculas(usuarios, nMatriculas);
			}
			else {
				if (privilegiosAdmin) {
					fprintf(stderr, "N�o h� usu�rios na sua academia. Deseja cadastrar (S/N)?\n");
					scanf("%c%c", &escolha, &lixo);
					switch (escolha) {
						case 'S':
						case 's':
							usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas + 1));
							cadastrarUsuarios(&usuarios[nMatriculas]);
							nMatriculas++;
						break;
						default:
						break;
					}
				}
			}
			break;
		case 2:
			if (status_carregamento > 1) {
				// c�digo aqui
			} else if (status_carregamento < 1) fprintf(stderr, "<N�o h� usu�rios na sua academia. Deseja cadastrar?>\n");
			break;
		case 3:
			// procurar matricula por parte do nome
			break;
		case 4:
			privilegiosAdmin = true;
			if (privilegiosAdmin) fprintf(stderr, "<Admin ativado!>\n");
			break;
		case 5:
			usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas + 1));
			cadastrarUsuarios(&usuarios[nMatriculas]);
			nMatriculas++;
			break;
		case 6:
			util_imprimeTracinhos(5);
			printf(" DADOS ATUAIS ");
			util_imprimeTracinhos(5);
			printf("\n"
				"Nome		%s\n"
				"CNPJ		%llu\n"
				"Endere�o	%s\n"
				"E-mail		%s\n"
				"Telefone	%llu\n", academia.nome, academia.CNPJ, academia.endereco, academia.email, academia.telefone);
			// editar
			break;
		default:
			fprintf(stderr, "<Erro na leitura da op��o.>\n");
			break;
		}
	} while (opcao);

	// salvamento em arquivo <db.bin>
	salvaTudo(usuarios, academia);

	return SUCESSO;
}