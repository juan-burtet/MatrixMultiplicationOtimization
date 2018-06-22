#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LIM 1024

/** Struct de uma Matriz
	* pos - indica a posição na matriz
	* linhas - quantidade de linhas
	* colunas - quantidade de colunas
	*/
typedef struct {
	int **pos;
	size_t linhas;
	size_t colunas;
}Matriz;

// Funcoes usadas no programa
Matriz *alocar_matriz(size_t linhas, size_t colunas);
Matriz *multiplica_matrizes(Matriz *A, Matriz *B);
void    complete_matrix(Matriz *A, Matriz *B);
void    free_matriz(Matriz *destroy);

/** Funcao Main
	*/
int main(void) {
	// Variaveis
	Matriz *A = NULL;
	Matriz *B = NULL;
	Matriz *Resultado = NULL;
	clock_t c2, c1;
	float tempo;

	// Aloca duas matrizes na memoria
	A = alocar_matriz(LIM, LIM);
	B = alocar_matriz(LIM, LIM);

	// Adiciona valores para elas
	complete_matrix(A,B);

	// Imprime informacoes
	printf("UTILIZANDO O ALGORITMO TRIVIAL\n");
	printf("Quantidade de Linhas/Colunas = %d\n", LIM);

	// Pegar o tempo antes da funcao
	c1 = clock();

	// Multiplicacao de Matrizes
	Resultado = multiplica_matrizes(A,B);

	// tempo depois da funcao
	c2 = clock();

	// calculo do tempo
	tempo = (c2 - c1)*1000/CLOCKS_PER_SEC;

	// imprime o tempo na tela
	printf("tempo do sequencial: %.0fms\n", tempo);

	// desaloca as matrizes da memoria
	free_matriz(A);
	free_matriz(B);
	free_matriz(Resultado);

	// Finaliza o programa
  return 0;
}//main

/** Função que aloca uma matriz de n linhas e m colunas
	* @param linhas - quantidade de linhas
	* @param colunas - quantidade de colunas
	* @return Matriz linhasXcolunas
	*/
Matriz *alocar_matriz(size_t linhas, size_t colunas){
	// Aloca o espaco da struct matriz
	Matriz *matriz = malloc(sizeof(Matriz));

	// adiciona a quantidade de linhas e colunas
	matriz->linhas  = linhas;
	matriz->colunas = colunas;

	// Aloca a quantidade de linhas e colunas
	matriz->pos = malloc(linhas * sizeof(int *));
	for(size_t i = 0; i < linhas; i++)
		matriz->pos[i] = calloc(colunas, sizeof(int));

	// Retorna a matriz
	return matriz;
}//alocar_matriz

/** Função que faz a multiplicacao de matrizes
	* @param A - Matriz A
	* @param B - Matriz B
	* @return - Retorna a Matriz Multiplicada
	*/
Matriz *multiplica_matrizes(Matriz *A, Matriz *B){
	/* Se as linhas de A forem diferentes das colunas de B,
	não pode multiplicar as matrizes */
  if(A->linhas != B->colunas)
    return NULL;

  // Aloca na Memoria onde sera armazenado a Matriz Resultado
  Matriz *Resultado = alocar_matriz(A->linhas, B->colunas);

 	// Multiplicacao de Matrizes
  for(size_t i = 0; i < A->linhas; ++i)
    for(size_t j = 0; j < B->colunas; ++j)
      for(size_t k = 0; k < A->colunas; ++k)
        Resultado->pos[i][j] += A->pos[i][k] * B->pos[k][j];

  // Retorna a Matriz Resultado
  return Resultado;
}//multiplica_matrizes

/** Função que adiciona valores as Matrizes A e B
	* @param A - Matriz A
	* @param B - Matriz B
	*/
void complete_matrix(Matriz *A, Matriz *B){
	// Adiciona valores a Matriz A
	for(size_t i = 0; i < A->linhas; i++)
		for(size_t j = 0; j < A->colunas; j++)
			A->pos[i][j] = (i * A->colunas) + 2*j;

	// Adiciona valores a Matriz B
	for(size_t i = 0; i < B->linhas; i++)
		for(size_t j = 0; j < B->colunas; j++)
			B->pos[i][j] = (i * B->colunas) + (2*j) + 1;
}//complete_matrix

/** Funcao que desaloca a matriz da memoria
	* @param destroy - matriz a ser desalocada
	*/
void free_matriz(Matriz *destroy){
	for(size_t i = 0; i < destroy->linhas; i++)
		free(destroy->pos[i]);
	free(destroy->pos);
	free(destroy);
}//free_matriz
