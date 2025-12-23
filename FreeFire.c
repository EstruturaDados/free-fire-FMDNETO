/*
 * Sistema de mochila comparando vetor x lista encadeada
 * Operações: inserir, remover, listar, buscar (sequencial nas duas estruturas
 * e binária apenas no vetor após ordenação). Exibe contadores de comparações
 * para as buscas, destacando diferenças de desempenho.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CAPACIDADE_MAXIMA 50
#define TAM_NOME 30
#define TAM_TIPO 20
#define MAX_COMPONENTES 20

typedef struct {
	char nome[TAM_NOME];
	char tipo[TAM_TIPO];
	int quantidade;
} Item;

typedef struct No {
	Item dados;
	struct No *proximo;
} No;

/* Estrutura para os componentes da torre de fuga */
typedef struct {
	char nome[TAM_NOME];
	char tipo[TAM_TIPO];
	int prioridade;
} Componente;

/* Utilidades de entrada: limpam buffer e leem linha com corte do \n. */
void limparBufferEntrada(void);
void lerLinha(char *destino, size_t tamanho);

int menuEstrutura(void);
int menuVetor(void);
int menuLista(void);
int menuTorreFuga(void);

int inserirItemVetor(Item mochila[], int *tamanho, const char *nome, const char *tipo, int quantidade, int *ordenado);
int removerItemVetor(Item mochila[], int *tamanho, const char *nome, int *ordenado);
void listarItensVetor(const Item mochila[], int tamanho);
int buscarSequencialVetor(const Item mochila[], int tamanho, const char *nome, long *comparacoes);
int ordenarVetor(Item mochila[], int tamanho);
int buscarBinariaVetor(const Item mochila[], int tamanho, const char *nome, long *comparacoes);

int inserirItemLista(No **inicio, const char *nome, const char *tipo, int quantidade);
int removerItemLista(No **inicio, const char *nome);
void listarItensLista(const No *inicio);
No *buscarSequencialLista(const No *inicio, const char *nome, long *comparacoes);
void liberarLista(No **inicio);

/* Funções do módulo Torre de Fuga */
void cadastrarComponentes(Componente componentes[], int *total);
void bubbleSortNome(Componente componentes[], int total, long *comparacoes);
void insertionSortTipo(Componente componentes[], int total, long *comparacoes);
void selectionSortPrioridade(Componente componentes[], int total, long *comparacoes);
int buscaBinariaPorNome(Componente componentes[], int total, const char *nomeBuscado, long *comparacoes);
void mostrarComponentes(const Componente componentes[], int total);
double medirTempoOrdenacao(void (*algoritmo)(Componente[], int, long*), Componente componentes[], int total, long *comparacoes);

/* Loop principal: permite escolher vetor ou lista e testar as operações em cada uma. */
int main(void) {
	Item mochilaVetor[CAPACIDADE_MAXIMA];
	int tamanhoVetor = 0;
	int vetorOrdenado = 0;
	No *inicioLista = NULL;
	Componente componentesTorre[MAX_COMPONENTES];
	int totalComponentes = 0;
	int ordenadoPorNome = 0;

	printf("Comparador de mochila: vetor x lista encadeada\n");
	printf("+ MODULO TORRE DE FUGA\n\n");

	for (;;) {
		int escolha = menuEstrutura();
		if (escolha == 0) {
			printf("Saindo... Bons saques!\n");
			break;
		}

		if (escolha == 1) {
			for (;;) {
				int opcao = menuVetor();
				if (opcao == 0) {
					break;
				}

				switch (opcao) {
					case 1: {
						char nome[TAM_NOME];
						char tipo[TAM_TIPO];
						int quantidade;

						printf("Nome do item: ");
						lerLinha(nome, sizeof(nome));
						printf("Tipo do item: ");
						lerLinha(tipo, sizeof(tipo));
						printf("Quantidade: ");
						if (scanf("%d", &quantidade) != 1) {
							printf("Quantidade inválida.\n");
							limparBufferEntrada();
							break;
						}
						limparBufferEntrada();
						inserirItemVetor(mochilaVetor, &tamanhoVetor, nome, tipo, quantidade, &vetorOrdenado);
						listarItensVetor(mochilaVetor, tamanhoVetor);
						break;
					}
					case 2: {
						char nome[TAM_NOME];
						printf("Nome do item a remover: ");
						lerLinha(nome, sizeof(nome));
						removerItemVetor(mochilaVetor, &tamanhoVetor, nome, &vetorOrdenado);
						listarItensVetor(mochilaVetor, tamanhoVetor);
						break;
					}
					case 3:
						listarItensVetor(mochilaVetor, tamanhoVetor);
						break;
					case 4:
						if (ordenarVetor(mochilaVetor, tamanhoVetor)) {
							vetorOrdenado = 1;
							printf("Vetor ordenado por nome (bubble sort).\n");
						} else {
							printf("Nada para ordenar.\n");
						}
						break;
					case 5: {
						char nome[TAM_NOME];
						long comparacoes = 0;
						printf("Nome a buscar (sequencial): ");
						lerLinha(nome, sizeof(nome));
						int pos = buscarSequencialVetor(mochilaVetor, tamanhoVetor, nome, &comparacoes);
						if (pos >= 0) {
							printf("Encontrado na posicao %d. Comparacoes: %ld\n", pos + 1, comparacoes);
						} else {
							printf("Nao encontrado. Comparacoes: %ld\n", comparacoes);
						}
						break;
					}
					case 6: {
						char nome[TAM_NOME];
						long comparacoes = 0;
						if (!vetorOrdenado) {
							printf("Ordene o vetor antes de usar busca binaria.\n");
							break;
						}
						printf("Nome a buscar (binaria): ");
						lerLinha(nome, sizeof(nome));
						int pos = buscarBinariaVetor(mochilaVetor, tamanhoVetor, nome, &comparacoes);
						if (pos >= 0) {
							printf("Encontrado na posicao %d. Comparacoes: %ld\n", pos + 1, comparacoes);
						} else {
							printf("Nao encontrado. Comparacoes: %ld\n", comparacoes);
						}
						break;
					}
					default:
						printf("Opcao invalida.\n");
						break;
				}
			}
		} else if (escolha == 2) {
			for (;;) {
				int opcao = menuLista();
				if (opcao == 0) {
					break;
				}

				switch (opcao) {
					case 1: {
						char nome[TAM_NOME];
						char tipo[TAM_TIPO];
						int quantidade;

						printf("Nome do item: ");
						lerLinha(nome, sizeof(nome));
						printf("Tipo do item: ");
						lerLinha(tipo, sizeof(tipo));
						printf("Quantidade: ");
						if (scanf("%d", &quantidade) != 1) {
							printf("Quantidade invalida.\n");
							limparBufferEntrada();
							break;
						}
						limparBufferEntrada();
						inserirItemLista(&inicioLista, nome, tipo, quantidade);
						listarItensLista(inicioLista);
						break;
					}
					case 2: {
						char nome[TAM_NOME];
						printf("Nome do item a remover: ");
						lerLinha(nome, sizeof(nome));
						removerItemLista(&inicioLista, nome);
						listarItensLista(inicioLista);
						break;
					}
					case 3:
						listarItensLista(inicioLista);
						break;
					case 4: {
						char nome[TAM_NOME];
						long comparacoes = 0;
						printf("Nome a buscar (sequencial lista): ");
						lerLinha(nome, sizeof(nome));
						No *encontrado = buscarSequencialLista(inicioLista, nome, &comparacoes);
						if (encontrado) {
							printf("Encontrado: %s | %s | qtd %d. Comparacoes: %ld\n", encontrado->dados.nome, encontrado->dados.tipo, encontrado->dados.quantidade, comparacoes);
						} else {
							printf("Nao encontrado. Comparacoes: %ld\n", comparacoes);
						}
						break;
					}
					default:
						printf("Opcao invalida.\n");
						break;
				}
			}
		} else if (escolha == 3) {
			/* Módulo Torre de Fuga */
			for (;;) {
				int opcao = menuTorreFuga();
				if (opcao == 0) {
					break;
				}

				switch (opcao) {
					case 1: {
						cadastrarComponentes(componentesTorre, &totalComponentes);
						ordenadoPorNome = 0;
						break;
					}
					case 2: {
						if (totalComponentes == 0) {
							printf("Nenhum componente cadastrado.\n");
							break;
						}
						{
							long comparacoes = 0;
							double tempo;
							printf("\n>>> BUBBLE SORT por NOME...\n");
							tempo = medirTempoOrdenacao(bubbleSortNome, componentesTorre, totalComponentes, &comparacoes);
							printf("Ordenacao concluida!\n");
							printf("Comparacoes: %ld | Tempo: %.6f s\n", comparacoes, tempo);
							mostrarComponentes(componentesTorre, totalComponentes);
							ordenadoPorNome = 1;
						}
						break;
					}
					case 3: {
						if (totalComponentes == 0) {
							printf("Nenhum componente cadastrado.\n");
							break;
						}
						{
							long comparacoes = 0;
							double tempo;
							printf("\n>>> INSERTION SORT por TIPO...\n");
							tempo = medirTempoOrdenacao(insertionSortTipo, componentesTorre, totalComponentes, &comparacoes);
							printf("Ordenacao concluida!\n");
							printf("Comparacoes: %ld | Tempo: %.6f s\n", comparacoes, tempo);
							mostrarComponentes(componentesTorre, totalComponentes);
							ordenadoPorNome = 0;
						}
						break;
					}
					case 4: {
						if (totalComponentes == 0) {
							printf("Nenhum componente cadastrado.\n");
							break;
						}
						{
							long comparacoes = 0;
							double tempo;
							printf("\n>>> SELECTION SORT por PRIORIDADE...\n");
							tempo = medirTempoOrdenacao(selectionSortPrioridade, componentesTorre, totalComponentes, &comparacoes);
							printf("Ordenacao concluida!\n");
							printf("Comparacoes: %ld | Tempo: %.6f s\n", comparacoes, tempo);
							mostrarComponentes(componentesTorre, totalComponentes);
							ordenadoPorNome = 0;
						}
						break;
					}
					case 5: {
						if (totalComponentes == 0) {
							printf("Nenhum componente cadastrado.\n");
							break;
						}
						if (!ordenadoPorNome) {
							printf("\n[ATENCAO] Ordene por NOME antes (opcao 2)!\n");
							break;
						}
						{
							char nomeBuscado[TAM_NOME];
							long comparacoes = 0;
							int pos;
							printf("\n>>> BUSCA BINARIA <<<\n");
							printf("Nome do componente-chave: ");
							lerLinha(nomeBuscado, sizeof(nomeBuscado));
							pos = buscaBinariaPorNome(componentesTorre, totalComponentes, nomeBuscado, &comparacoes);
							if (pos >= 0) {
								printf("\n[COMPONENTE ENCONTRADO!]\n");
								printf("Posicao: %d\n", pos + 1);
								printf("%s | %s | prioridade %d\n", componentesTorre[pos].nome, componentesTorre[pos].tipo, componentesTorre[pos].prioridade);
								printf("Comparacoes: %ld\n", comparacoes);
								printf(">>> Torre destrancada! <<<\n");
							} else {
								printf("\nNao encontrado. Comparacoes: %ld\n", comparacoes);
							}
						}
						break;
					}
					case 6: {
						if (totalComponentes == 0) {
							printf("Nenhum componente cadastrado.\n");
						} else {
							mostrarComponentes(componentesTorre, totalComponentes);
						}
						break;
					}
					default:
						printf("Opcao invalida.\n");
						break;
				}
			}
		}
	}

	liberarLista(&inicioLista);
	return 0;
}

/* Descarta caracteres até o próximo \n para evitar lixo em leituras futuras. */
void limparBufferEntrada(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
	}
}

/* Lê linha segura e remove o \n final, se presente. */
void lerLinha(char *destino, size_t tamanho) {
	if (fgets(destino, (int)tamanho, stdin) == NULL) {
		destino[0] = '\0';
		return;
	}
	size_t len = strlen(destino);
	if (len > 0 && destino[len - 1] == '\n') {
		destino[len - 1] = '\0';
	}
}

int menuEstrutura(void) {
	int opcao;
	printf("\n=== Selecione a estrutura ===\n");
	printf("1) Vetor (lista sequencial)\n");
	printf("2) Lista encadeada\n");
	printf("3) Torre de Fuga (DESAFIO FINAL)\n");
	printf("0) Sair\n");
	printf("Escolha: ");
	if (scanf("%d", &opcao) != 1) {
		opcao = 0;
	}
	limparBufferEntrada();
	return opcao;
}

/* Menu dedicado ao vetor para operar com inserção, remoção, ordenação e buscas. */
int menuVetor(void) {
	int opcao;
	printf("\n--- Mochila (vetor) ---\n");
	printf("1) Inserir item\n");
	printf("2) Remover item\n");
	printf("3) Listar itens\n");
	printf("4) Ordenar por nome\n");
	printf("5) Buscar sequencial\n");
	printf("6) Buscar binaria\n");
	printf("0) Voltar\n");
	printf("Escolha: ");
	if (scanf("%d", &opcao) != 1) {
		opcao = 0;
	}
	limparBufferEntrada();
	return opcao;
}

/* Menu dedicado à lista encadeada para inserir, remover, listar e buscar. */
int menuLista(void) {
	int opcao;
	printf("\n--- Mochila (lista) ---\n");
	printf("1) Inserir item\n");
	printf("2) Remover item\n");
	printf("3) Listar itens\n");
	printf("4) Buscar sequencial\n");
	printf("0) Voltar\n");
	printf("Escolha: ");
	if (scanf("%d", &opcao) != 1) {
		opcao = 0;
	}
	limparBufferEntrada();
	return opcao;
}

/* Inserção no fim do vetor; invalida ordenação para forçar reordenação antes da binária. */
int inserirItemVetor(Item mochila[], int *tamanho, const char *nome, const char *tipo, int quantidade, int *ordenado) {
	if (*tamanho >= CAPACIDADE_MAXIMA) {
		printf("Mochila cheia (%d itens).\n", CAPACIDADE_MAXIMA);
		return 0;
	}
	if (quantidade <= 0) {
		printf("Quantidade deve ser positiva.\n");
		return 0;
	}
	strncpy(mochila[*tamanho].nome, nome, TAM_NOME - 1);
	mochila[*tamanho].nome[TAM_NOME - 1] = '\0';
	strncpy(mochila[*tamanho].tipo, tipo, TAM_TIPO - 1);
	mochila[*tamanho].tipo[TAM_TIPO - 1] = '\0';
	mochila[*tamanho].quantidade = quantidade;
	(*tamanho)++;
	*ordenado = 0;
	return 1;
}

/* Remove por nome deslocando elementos; também invalida a ordenação. */
int removerItemVetor(Item mochila[], int *tamanho, const char *nome, int *ordenado) {
	int idx = -1;
	for (int i = 0; i < *tamanho; i++) {
		if (strcmp(mochila[i].nome, nome) == 0) {
			idx = i;
			break;
		}
	}
	if (idx == -1) {
		printf("Item nao encontrado.\n");
		return 0;
	}
	for (int i = idx; i < (*tamanho) - 1; i++) {
		mochila[i] = mochila[i + 1];
	}
	(*tamanho)--;
	*ordenado = 0;
	return 1;
}

/* Exibe inventário armazenado no vetor. */
void listarItensVetor(const Item mochila[], int tamanho) {
	printf("\nItens no vetor (%d/%d):\n", tamanho, CAPACIDADE_MAXIMA);
	if (tamanho == 0) {
		printf("(vazio)\n");
		return;
	}
	for (int i = 0; i < tamanho; i++) {
		printf("%d) %s | %s | qtd %d\n", i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
	}
}

/* Busca sequencial no vetor com contador de comparações. */
int buscarSequencialVetor(const Item mochila[], int tamanho, const char *nome, long *comparacoes) {
	for (int i = 0; i < tamanho; i++) {
		(*comparacoes)++;
		if (strcmp(mochila[i].nome, nome) == 0) {
			return i;
		}
	}
	return -1;
}

/* Bubble sort por nome para habilitar busca binária. */
int ordenarVetor(Item mochila[], int tamanho) {
	if (tamanho < 2) {
		return 0;
	}
	for (int i = 0; i < tamanho - 1; i++) {
		for (int j = 0; j < tamanho - i - 1; j++) {
			if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
				Item tmp = mochila[j];
				mochila[j] = mochila[j + 1];
				mochila[j + 1] = tmp;
			}
		}
	}
	return 1;
}

/* Busca binária no vetor ordenado, contando comparações. */
int buscarBinariaVetor(const Item mochila[], int tamanho, const char *nome, long *comparacoes) {
	int inicio = 0;
	int fim = tamanho - 1;
	while (inicio <= fim) {
		int meio = (inicio + fim) / 2;
		(*comparacoes)++;
		int cmp = strcmp(mochila[meio].nome, nome);
		if (cmp == 0) {
			return meio;
		} else if (cmp < 0) {
			inicio = meio + 1;
		} else {
			fim = meio - 1;
		}
	}
	return -1;
}

/* Insere novo nó ao final da lista encadeada (custo linear). */
int inserirItemLista(No **inicio, const char *nome, const char *tipo, int quantidade) {
	if (quantidade <= 0) {
		printf("Quantidade deve ser positiva.\n");
		return 0;
	}
	No *novo = (No *)malloc(sizeof(No));
	if (!novo) {
		printf("Falha ao alocar memoria.\n");
		return 0;
	}
	strncpy(novo->dados.nome, nome, TAM_NOME - 1);
	novo->dados.nome[TAM_NOME - 1] = '\0';
	strncpy(novo->dados.tipo, tipo, TAM_TIPO - 1);
	novo->dados.tipo[TAM_TIPO - 1] = '\0';
	novo->dados.quantidade = quantidade;
	novo->proximo = NULL;

	if (*inicio == NULL) {
		*inicio = novo;
		return 1;
	}
	No *atual = *inicio;
	while (atual->proximo) {
		atual = atual->proximo;
	}
	atual->proximo = novo;
	return 1;
}

/* Remove nó pelo nome ajustando encadeamento. */
int removerItemLista(No **inicio, const char *nome) {
	No *anterior = NULL;
	No *atual = *inicio;
	while (atual) {
		if (strcmp(atual->dados.nome, nome) == 0) {
			if (anterior == NULL) {
				*inicio = atual->proximo;
			} else {
				anterior->proximo = atual->proximo;
			}
			free(atual);
			return 1;
		}
		anterior = atual;
		atual = atual->proximo;
	}
	printf("Item nao encontrado.\n");
	return 0;
}

/* Percorre e exibe a lista encadeada. */
void listarItensLista(const No *inicio) {
	printf("\nItens na lista: \n");
	if (!inicio) {
		printf("(vazio)\n");
		return;
	}
	int idx = 1;
	while (inicio) {
		printf("%d) %s | %s | qtd %d\n", idx, inicio->dados.nome, inicio->dados.tipo, inicio->dados.quantidade);
		inicio = inicio->proximo;
		idx++;
	}
}

/* Busca sequencial na lista com contagem de comparações. */
No *buscarSequencialLista(const No *inicio, const char *nome, long *comparacoes) {
	while (inicio) {
		(*comparacoes)++;
		if (strcmp(inicio->dados.nome, nome) == 0) {
			return (No *)inicio;
		}
		inicio = inicio->proximo;
	}
	return NULL;
}

/* Libera nós alocados dinamicamente para evitar vazamento. */
void liberarLista(No **inicio) {
	No *atual = *inicio;
	while (atual) {
		No *prox = atual->proximo;
		free(atual);
		atual = prox;
	}
	*inicio = NULL;
}

/* ========== MÓDULO TORRE DE FUGA ========== */

/* Menu do módulo Torre de Fuga */
int menuTorreFuga(void) {
	int opcao;
	printf("\n=== TORRE DE FUGA - DESAFIO FINAL ===\n");
	printf("1) Cadastrar componentes\n");
	printf("2) Ordenar por NOME (Bubble Sort)\n");
	printf("3) Ordenar por TIPO (Insertion Sort)\n");
	printf("4) Ordenar por PRIORIDADE (Selection Sort)\n");
	printf("5) Buscar componente-chave (Busca Binaria)\n");
	printf("6) Listar componentes\n");
	printf("0) Voltar\n");
	printf("Escolha: ");
	if (scanf("%d", &opcao) != 1) {
		opcao = 0;
	}
	limparBufferEntrada();
	return opcao;
}

/* Cadastra componentes da torre com nome, tipo e prioridade */
void cadastrarComponentes(Componente componentes[], int *total) {
	int quantidade;
	printf("\n>>> CADASTRO DE COMPONENTES <<<\n");
	printf("Quantos componentes deseja cadastrar? (max %d): ", MAX_COMPONENTES);
	if (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
		printf("Quantidade invalida.\n");
		limparBufferEntrada();
		return;
	}
	limparBufferEntrada();
	if (quantidade > MAX_COMPONENTES) {
		quantidade = MAX_COMPONENTES;
		printf("Limitado a %d componentes.\n", MAX_COMPONENTES);
	}
	*total = 0;
	for (int i = 0; i < quantidade; i++) {
		printf("\n--- Componente %d ---\n", i + 1);
		printf("Nome: ");
		lerLinha(componentes[*total].nome, TAM_NOME);
		printf("Tipo (controle/suporte/propulsao): ");
		lerLinha(componentes[*total].tipo, TAM_TIPO);
		printf("Prioridade (1-10): ");
		if (scanf("%d", &componentes[*total].prioridade) != 1) {
			printf("Prioridade invalida, definindo como 1.\n");
			componentes[*total].prioridade = 1;
		}
		limparBufferEntrada();
		if (componentes[*total].prioridade < 1 || componentes[*total].prioridade > 10) {
			if (componentes[*total].prioridade < 1) componentes[*total].prioridade = 1;
			if (componentes[*total].prioridade > 10) componentes[*total].prioridade = 10;
		}
		(*total)++;
	}
	printf("\n%d componentes cadastrados!\n", *total);
}

/* Bubble Sort: ordena componentes por NOME (ordem alfabética) */
void bubbleSortNome(Componente componentes[], int total, long *comparacoes) {
	*comparacoes = 0;
	for (int i = 0; i < total - 1; i++) {
		for (int j = 0; j < total - i - 1; j++) {
			(*comparacoes)++;
			if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
				Componente temp = componentes[j];
				componentes[j] = componentes[j + 1];
				componentes[j + 1] = temp;
			}
		}
	}
}

/* Insertion Sort: ordena componentes por TIPO (ordem alfabética) */
void insertionSortTipo(Componente componentes[], int total, long *comparacoes) {
	*comparacoes = 0;
	for (int i = 1; i < total; i++) {
		Componente chave = componentes[i];
		int j = i - 1;
		while (j >= 0) {
			(*comparacoes)++;
			if (strcmp(componentes[j].tipo, chave.tipo) > 0) {
				componentes[j + 1] = componentes[j];
				j--;
			} else {
				break;
			}
		}
		componentes[j + 1] = chave;
	}
}

/* Selection Sort: ordena componentes por PRIORIDADE (ordem crescente) */
void selectionSortPrioridade(Componente componentes[], int total, long *comparacoes) {
	*comparacoes = 0;
	for (int i = 0; i < total - 1; i++) {
		int indiceMinimo = i;
		for (int j = i + 1; j < total; j++) {
			(*comparacoes)++;
			if (componentes[j].prioridade < componentes[indiceMinimo].prioridade) {
				indiceMinimo = j;
			}
		}
		if (indiceMinimo != i) {
			Componente temp = componentes[i];
			componentes[i] = componentes[indiceMinimo];
			componentes[indiceMinimo] = temp;
		}
	}
}

/* Busca Binária: busca componente por NOME (requer ordenação prévia por nome) */
int buscaBinariaPorNome(Componente componentes[], int total, const char *nomeBuscado, long *comparacoes) {
	*comparacoes = 0;
	int inicio = 0;
	int fim = total - 1;
	while (inicio <= fim) {
		int meio = inicio + (fim - inicio) / 2;
		(*comparacoes)++;
		int cmp = strcmp(componentes[meio].nome, nomeBuscado);
		if (cmp == 0) {
			return meio;
		} else if (cmp < 0) {
			inicio = meio + 1;
		} else {
			fim = meio - 1;
		}
	}
	return -1;
}

/* Exibe todos os componentes da torre formatados */
void mostrarComponentes(const Componente componentes[], int total) {
	printf("\n=== COMPONENTES DA TORRE (%d/%d) ===\n", total, MAX_COMPONENTES);
	if (total == 0) {
		printf("(Nenhum componente cadastrado)\n");
		return;
	}
	printf("%-4s %-25s %-18s %s\n", "#", "NOME", "TIPO", "PRIORIDADE");
	printf("---------------------------------------------------\n");
	for (int i = 0; i < total; i++) {
		printf("%-4d %-25s %-18s %d\n", i + 1, componentes[i].nome, componentes[i].tipo, componentes[i].prioridade);
	}
	printf("===================================================\n");
}

/* Mede o tempo de execução de um algoritmo de ordenação */
double medirTempoOrdenacao(void (*algoritmo)(Componente[], int, long*), Componente componentes[], int total, long *comparacoes) {
	clock_t inicio = clock();
	algoritmo(componentes, total, comparacoes);
	clock_t fim = clock();
	return (double)(fim - inicio) / CLOCKS_PER_SEC;
}


