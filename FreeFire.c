/*
 * Sistema de Inventário (Mochila de Loot Inicial)
 *
 * Objetivo:
 *  - Representar itens coletados pelo jogador usando struct e vetor (lista sequencial).
 *  - Permitir cadastrar, remover, listar e buscar itens por nome.
 *
 * Requisitos atendidos:
 *  - Struct Item com campos nome, tipo e quantidade.
 *  - Vetor com capacidade para 10 itens.
 *  - Funções: inserirItem(), removerItem(), listarItens(), buscarItem().
 *  - Leitura: scanf para inteiros e fgets para strings com tratamento de newline.
 *  - Busca sequencial por nome.
 *
 * Usabilidade:
 *  - Interface textual clara com mensagens orientativas após cada operação.
 *
 * Manutenibilidade:
 *  - Nomes de funções e variáveis descritivos.
 *  - Comentários explicativos do fluxo e das funções.
 *
 * Como usar:
 *  - Opção 1: Cadastrar um novo item informando nome, tipo e quantidade.
 *  - Opção 2: Remover um item existente pelo nome.
 *  - Opção 3: Buscar um item pelo nome e exibir seus dados.
 *  - Opção 4: Listar todos os itens cadastrados.
 *  - Opção 5: Sair do programa.
 */

#include <stdio.h>   // Entrada e saída
#include <string.h>  // Manipulação de strings
#include <stdlib.h>  // Utilidades gerais

#define CAPACIDADE_MAXIMA 10
#define TAM_NOME 30
#define TAM_TIPO 20

/*
 * Struct Item: agrega os dados essenciais de cada objeto coletado.
 */
typedef struct {
	char nome[TAM_NOME];     // Ex.: "AK-47", "Bandagem", "9mm"
	char tipo[TAM_TIPO];     // Ex.: "arma", "munição", "cura"
	int quantidade;          // Quantidade do item
} Item;

/*
 * Resumo do programa:
 *  - Este programa simula a mochila inicial de um jogo de sobrevivência.
 *  - O jogador pode cadastrar, remover, listar e buscar itens por nome.
 *  - Os dados são armazenados em um vetor de structs com capacidade fixa.
 *  - A interação ocorre via menu textual, com respostas claras a cada ação.
 */

/*
 * Protótipos das funções (declarações antecipadas):
 *  - Facilitam a organização do código, permitindo que `main()` e outras partes
 *    referenciem funções antes das suas definições completas.
 */
void limparBufferEntrada(void);
void lerLinha(char *destino, size_t tamanho);
int buscarItem(const Item mochila[], int tamanho, const char *nomeBuscado);
void listarItens(const Item mochila[], int tamanho);
int inserirItem(Item mochila[], int *tamanho, const char *nome, const char *tipo, int quantidade);
int removerItem(Item mochila[], int *tamanho, const char *nome);
int exibirMenu(void);

/*
 * Função principal
 *
 * Descrição:
 *  - Gerencia o loop do menu e chama as operações de inventário.
 *
 * Retorno:
 *  - 0 ao finalizar a execução do programa.
 */
int main(void) {
	Item mochila[CAPACIDADE_MAXIMA];
	int tamanho = 0;
	int executando = 1;

	printf("Bem-vindo! Organize seus recursos iniciais com rapidez.\n");
	listarItens(mochila, tamanho);

	while (executando) {
		int opcao = exibirMenu();
		switch (opcao) {
			case 1: {
				char nome[TAM_NOME];
				char tipo[TAM_TIPO];
				int quantidade;

				printf("\n-- Cadastro de Item --\n");
				printf("Nome do item (ex.: AK-47, Bandagem): ");
				lerLinha(nome, sizeof(nome));

				printf("Tipo do item (ex.: arma, munição, cura): ");
				lerLinha(tipo, sizeof(tipo));

				printf("Quantidade: ");
				if (scanf("%d", &quantidade) != 1) {
					printf("Entrada inválida para quantidade. Operação cancelada.\n");
					limparBufferEntrada();
					break;
				}
				limparBufferEntrada();

				inserirItem(mochila, &tamanho, nome, tipo, quantidade);
				listarItens(mochila, tamanho);
				break;
			}
			case 2: {
				char nome[TAM_NOME];
				printf("\n-- Remoção de Item --\n");
				printf("Informe o nome do item a remover: ");
				lerLinha(nome, sizeof(nome));

				removerItem(mochila, &tamanho, nome);
				listarItens(mochila, tamanho);
				break;
			}
			case 3: {
				char nome[TAM_NOME];
				printf("\n-- Busca de Item --\n");
				printf("Informe o nome do item a buscar: ");
				lerLinha(nome, sizeof(nome));

				int idx = buscarItem(mochila, tamanho, nome);
				if (idx >= 0) {
					printf("Encontrado: Nome: %s | Tipo: %s | Quantidade: %d\n",
						   mochila[idx].nome, mochila[idx].tipo, mochila[idx].quantidade);
				} else {
					printf("Item '%s' não encontrado.\n", nome);
				}
				listarItens(mochila, tamanho);
				break;
			}
			case 4:
				listarItens(mochila, tamanho);
				break;
			case 5:
				executando = 0;
				printf("Saindo... Bons saques e boa sorte!\n");
				break;
			default:
				printf("\nOpção inválida. Tente novamente.\n\n");
				break;
		}
	}

	return 0;
}

/*
 * limparBufferEntrada
 *
 * Descrição:
 *  - Limpa o buffer de entrada até o próximo '\n'.
 *  - Útil após ler inteiros com scanf para evitar que o '\n' atrapalhe leituras de linhas.
 */
void limparBufferEntrada(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
		/* descarta */
	}
}

/*
 * lerLinha
 *
 * Descrição:
 *  - Lê uma linha segura do stdin (até '\n' ou até tamanho-1 caracteres),
 *    removendo o '\n' final, caso presente.
 *
 * Parâmetros:
 *  - destino: ponteiro para buffer de destino da string.
 *  - tamanho: capacidade total do buffer (inclui o terminador '\0').
 */
void lerLinha(char *destino, size_t tamanho) {
	if (fgets(destino, (int)tamanho, stdin) == NULL) {
		destino[0] = '\0';
		return;
	}
	// Remover '\n' caso tenha sido lido
	size_t len = strlen(destino);
	if (len > 0 && destino[len - 1] == '\n') {
		destino[len - 1] = '\0';
	}
}

/*
 * buscarItem
 *
 * Descrição:
 *  - Realiza busca sequencial pelo nome do item no vetor da mochila.
 *
 * Parâmetros:
 *  - mochila: vetor de itens.
 *  - tamanho: quantidade atual de itens armazenados.
 *  - nomeBuscado: nome do item a localizar.
 *
 * Retorno:
 *  - Índice do item encontrado (0..tamanho-1) ou -1 se não encontrado.
 */
int buscarItem(const Item mochila[], int tamanho, const char *nomeBuscado) {
	for (int i = 0; i < tamanho; i++) {
		if (strcmp(mochila[i].nome, nomeBuscado) == 0) {
			return i;
		}
	}
	return -1;
}

/*
 * listarItens
 *
 * Descrição:
 *  - Exibe todos os itens presentes na mochila com nome, tipo e quantidade.
 *
 * Parâmetros:
 *  - mochila: vetor de itens.
 *  - tamanho: quantidade atual de itens armazenados.
 */
void listarItens(const Item mochila[], int tamanho) {
	printf("\n==== Inventário Atual (%d/%d) ====\n", tamanho, CAPACIDADE_MAXIMA);
	if (tamanho == 0) {
		printf("Mochila vazia.\n");
	} else {
		for (int i = 0; i < tamanho; i++) {
			printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
				   i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
		}
	}
	printf("=================================\n\n");
}

/*
 * inserirItem
 *
 * Descrição:
 *  - Insere um novo item na mochila, se houver capacidade e quantidade válida.
 *
 * Parâmetros:
 *  - mochila: vetor de itens.
 *  - tamanho: ponteiro para a quantidade atual (será incrementada).
 *  - nome: nome do item a inserir.
 *  - tipo: tipo do item (ex.: "arma", "munição", "cura").
 *  - quantidade: quantidade a registrar (deve ser positiva).
 *
 * Retorno:
 *  - 1 se inseriu com sucesso, 0 caso contrário.
 */
int inserirItem(Item mochila[], int *tamanho, const char *nome, const char *tipo, int quantidade) {
	if (*tamanho >= CAPACIDADE_MAXIMA) {
		printf("[Aviso] Mochila cheia. Capacidade máxima (%d) atingida.\n", CAPACIDADE_MAXIMA);
		return 0;
	}
	if (quantidade <= 0) {
		printf("[Aviso] Quantidade deve ser positiva.\n");
		return 0;
	}

	// Copiar dados para a próxima posição livre
	strncpy(mochila[*tamanho].nome, nome, TAM_NOME - 1);
	mochila[*tamanho].nome[TAM_NOME - 1] = '\0';

	strncpy(mochila[*tamanho].tipo, tipo, TAM_TIPO - 1);
	mochila[*tamanho].tipo[TAM_TIPO - 1] = '\0';

	mochila[*tamanho].quantidade = quantidade;
	(*tamanho)++;
	printf("[Ok] Item cadastrado com sucesso.\n");
	return 1;
}

/*
 * removerItem
 *
 * Descrição:
 *  - Remove um item da mochila pelo nome, deslocando os seguintes para manter a sequência.
 *
 * Parâmetros:
 *  - mochila: vetor de itens.
 *  - tamanho: ponteiro para a quantidade atual (será decrementada).
 *  - nome: nome do item a remover.
 *
 * Retorno:
 *  - 1 se removeu, 0 caso não encontrado.
 */
int removerItem(Item mochila[], int *tamanho, const char *nome) {
	int idx = buscarItem(mochila, *tamanho, nome);
	if (idx == -1) {
		printf("[Aviso] Item '%s' não encontrado.\n", nome);
		return 0;
	}
	// Desloca elementos à esquerda para preencher o espaço
	for (int i = idx; i < (*tamanho) - 1; i++) {
		mochila[i] = mochila[i + 1];
	}
	(*tamanho)--;
	printf("[Ok] Item removido com sucesso.\n");
	return 1;
}

/*
 * exibirMenu
 *
 * Descrição:
 *  - Exibe o menu principal e retorna a opção escolhida pelo usuário.
 *
 * Retorno:
 *  - Inteiro representando a opção selecionada.
 */
int exibirMenu(void) {
	int opcao;
	printf("==== Mochila de Loot Inicial ====\n");
	printf("1) Cadastrar item\n");
	printf("2) Remover item\n");
	printf("3) Buscar item por nome\n");
	printf("4) Listar itens\n");
	printf("5) Sair\n");
	printf("Selecione uma opção: ");
	if (scanf("%d", &opcao) != 1) {
		printf("\nEntrada inválida. Tente novamente.\n");
		opcao = 0; // força repetição
	}
	limparBufferEntrada();
	return opcao;
}


