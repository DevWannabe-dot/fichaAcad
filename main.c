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
#include <locale.h>
#include <time.h>
#include <math.h>
#include "util.h"
#include "main.h"
#include "arquivos.h"

// Constantes

// Tipos

// Funções
void imprimeUsuarioUnico(usuario_t* usuarioAtual) {
	printf("%u: %s\n", usuarioAtual->matricula, usuarioAtual->nome);
}

void listaMatriculas(usuario_t* usuarios, int nMatriculas) {
	for (int i = 0; i < nMatriculas; i++) {
		imprimeUsuarioUnico(&usuarios[i]);
	}
}

void atribuirTreino(academia_t* academia, usuario_t* usuarioAtual) {
	char lixo, nome_arquivo[POW_2_32_CARACTERES];
	int escolha, nTreinos = 0;
	FILE* arquivo_usuario;

	criarNomeArquivo(academia, nome_arquivo, POW_2_32_CARACTERES, usuarioAtual->matricula);
	arquivo_usuario = fopen(nome_arquivo, "w");
	// listar treinos cadastrados ou vazios (A-Z)
	
	printf("\nQual treino deseja editar? ");
	scanf("%d%c", &escolha, &lixo);
}

int cadastrarUsuarios(academia_t academia, usuario_t* usuarioAtual){
	char escolha, lixo;
	usuarioAtual->matricula = 0;
	memset(usuarioAtual->nome, '\0', TAMANHO_NOME);

	printf("(0 PARA INTERROMPER)\nMatrícula do usuário: ");
	scanf("%u%c", &usuarioAtual->matricula, &lixo);	
	if (usuarioAtual->matricula == 0) return ERRO_CADASTRO;
	
	printf("Nome do usuário: ");
	fgets(usuarioAtual->nome, TAMANHO_NOME, stdin);
	util_removeQuebraLinhaFinal(usuarioAtual->nome);

	printf("Deseja atribuir treinos a este usuário (S/N)?");
	scanf("%c%c", &escolha, &lixo);

	switch (escolha) {
	case 'S':
	case 's':
		atribuirTreino(&academia, usuarioAtual);
		break;
	default:
		break;
	}

	return CADASTRO_OK;
}

int buscaUsuario(usuario_t* usuarios, int matricula, int nMatriculas) {
	for (int i = 0; i < nMatriculas; i++) {
		if (usuarios[i].matricula == matricula) {
			return i;
		}
	}
	return NADA_ENCONTRADO;
}

void acessarUsuario(usuario_t* usuarioAtual){
	
	imprimeUsuarioUnico(usuarioAtual);
	puts("Treinos: ");
}

int main(int argc, char** argv) {
	char lixo, backup[TAMANHO_ENDERECO], escolha;
	int nMatriculas = 0, opcao = 0, mat_lida;
	uint8_t status_carregamento;
	academia_t academia;
	usuario_t* usuarios = NULL;
	bool privilegiosAdmin = false;

	setlocale(LC_CTYPE, "Portuguese");

	// Le arquivo db.bin e cada arquivo de texto correspondente à matrícula
	printf("<LENDO CREDENCIAIS DA ACADEMIA...>\n");
	usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas + 1));

	// Leitura dos dados da academia atual
	status_carregamento = carregaAcad(usuarios, &academia, &nMatriculas);
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
			printf("CNPJ (obrigatório): ");
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

	// Leitura dos usuários da academia
	do {
		util_imprimeTracinhos(20);
		printf("\nACADEMIA %s\n", academia.nome);
		printf("0 - SAIR\n"
			"1 - Listar matrículas\n"
			"2 - Acessar usuário (matrícula)\n"
			"3 - Procurar matrícula (nome)\n"
			"4 - ADMIN\n"); // criar senha caso não exista, irá em db.bin
		// daí em diante, criar novo usuário e atribuir treinos
		if (privilegiosAdmin) {
			printf("5 - Cadastrar usuário\n"
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
			else if (privilegiosAdmin) {
				fprintf(stderr, "Não há usuários na sua academia. Deseja cadastrar (S/N)?\n");
				scanf("%c%c", &escolha, &lixo);
				switch (escolha) {
					case 'S':
					case 's':
						usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas + 1));
						nMatriculas += cadastrarUsuarios(academia, &usuarios[nMatriculas]);
					break;
					default:
					break;
				}
			}
			break;
		case 2:
			if (status_carregamento > 1) {
				printf("\nInsira a matricula: ");
				scanf("%i%c", &mat_lida, &lixo);
				mat_lida = buscaUsuario(usuarios, mat_lida, nMatriculas);
				if (mat_lida != -1) {
					acessarUsuario(&usuarios[mat_lida]);
				}
			}
			else if (privilegiosAdmin) {
				fprintf(stderr, "Não há usuários na sua academia. Deseja cadastrar (S/N)?\n");
				scanf("%c%c", &escolha, &lixo);
				switch (escolha) {
				case 'S':
				case 's':
					usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas + 1));
					nMatriculas += cadastrarUsuarios(academia, &usuarios[nMatriculas]);
					break;
				default:
					break;
				}
			}
			break;
		case 3:
			// procurar matricula por parte do nome
			break;
		case 4:
			privilegiosAdmin = true;
			if (privilegiosAdmin) fprintf(stderr, "<Admin ativado!>\n");
			break;
		case 5:
			if (privilegiosAdmin) {
				usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas + 1));
				nMatriculas += cadastrarUsuarios(academia, &usuarios[nMatriculas]);
			}
			break;
		case 6:
			if (privilegiosAdmin) {
				util_imprimeTracinhos(5);
				printf(" DADOS ATUAIS ");
				util_imprimeTracinhos(5);
				printf("\n"
					"Nome		%s\n"
					"CNPJ		%llu\n"
					"Endereço	%s\n"
					"E-mail		%s\n"
					"Telefone	%llu\n", academia.nome, academia.CNPJ, academia.endereco, academia.email, academia.telefone);
				// editar
			}
			break;
		default:
			fprintf(stderr, "<Erro na leitura da opção.>\n");
			break;
		}
	} while (opcao);

	// salvamento em arquivo <db.bin>
	salvaTudo(usuarios, academia, nMatriculas);

	return SUCESSO;
}