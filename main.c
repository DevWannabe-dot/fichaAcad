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
#include "util.h"

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
	unsigned* ExerPorIDs;			// IDs das práticas, sem estrutura* ou ** para fins de gravação em arquivo
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

// Funções
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

void imprimeUsuarioUnico(usuario_t* usuarioAtual) {
	printf("%u: %s", usuarioAtual->matricula, usuarioAtual->nome);
}

void listaMatriculas(usuario_t* usuarios, int nMatriculas) {
	for (int i = 0; i < nMatriculas; i++) {
		imprimeUsuarioUnico(&usuarios[i]);
	}
}

void atribuirTreino(usuario_t* usuarioAtual, treino_t* treinos) {
	char lixo;
	int escolha, i;

	// listar treinos
	printf("Qual treino da lista deseja adicionar (0 PARA INTERROMPER)? ");
	scanf("%i%c", &escolha, &lixo);

	for(i = 0; i < NUM_TREINOS_MAX; i++) {
		if (usuarioAtual->TreinosPorIDs[i] == 0) {
			usuarioAtual->TreinosPorIDs[i] = escolha;
			break;
		}
	}
}

void cadastrarUsuarios(usuario_t* usuarioAtual, treino_t* treinos){
	char escolha, lixo;
	
	printf("Matrícula do usuário: ");
	scanf("%u%c", &usuarioAtual->matricula, &lixo);	
	
	printf("Nome do usuário: ");
	fgets(usuarioAtual->nome, TAMANHO_NOME, stdin);
	util_removeQuebraLinhaFinal(usuarioAtual->nome);

	memset(usuarioAtual->TreinosPorIDs, 0, NUM_TREINOS_MAX*sizeof(int));
	printf("Deseja atribuir treinos a este usuário (S/N)?");
	scanf("%c%c", &escolha, &lixo);

	switch (escolha) {
	case 'S':
	case 's':
		atribuirTreino(usuarioAtual, treinos);
		break;
	default:
		break;
	}
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

int main(int argc, char** argv) {
	char lixo, backup[TAMANHO_ENDERECO], escolha;
	int nMatriculas = 0, opcao = 0;
	uint8_t status_carregamento;
	academia_t academia;
	usuario_t* usuarios = NULL;
	treino_t* treinos = NULL;
	exercicio_t* exercicios = NULL;

	setlocale(LC_CTYPE, "Portuguese");

	// Le arquivo db.bin e cada arquivo de texto correspondente à matrícula
	printf("<LENDO CREDENCIAIS DA ACADEMIA...>\n");
	exercicios = (exercicio_t*)realloc(exercicios, sizeof(exercicio_t));
	treinos = (treino_t*)realloc(treinos, sizeof(treino_t));
	usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t) * (nMatriculas + 1));

	// Leitura dos dados da academia atual
	status_carregamento = carregaAcad(&exercicios, &treinos, &usuarios, &academia, &nMatriculas);
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
				fprintf(stderr, "Não há usuários na sua academia. Deseja cadastrar (S/N)?\n");
				scanf("%c%c", &escolha, &lixo);
				switch (escolha) {
				case 'S':
				case 's':
					usuarios = (usuario_t*)realloc(usuarios, sizeof(usuario_t*) * (nMatriculas + 1));
					cadastrarUsuarios(&usuarios[nMatriculas], treinos);
					nMatriculas++;
					break;
				default:
					break;
				}
			}
			break;
		case 2:
			if (status_carregamento > 1) {
				// código aqui
			} else if (status_carregamento < 1) fprintf(stderr, "<Não há usuários na sua academia. Deseja cadastrar?>\n");
			break;
		default:
			fprintf(stderr, "<Erro na leitura da opção.>\n");
			break;
		}
	} while (opcao);

	// salvamento em arquivo <db.bin>
	salvaTudo(exercicios, treinos, usuarios, academia);

	return SUCESSO;
}