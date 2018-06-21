#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define LIM 512

struct Matrix{
	int **pos;
	size_t linhas;
	size_t colunas;
};

typedef struct Matrix Matriz;

Matriz *alocar_matriz(size_t linhas, size_t colunas);
Matriz *multiplica_matrizes(Matriz *A, Matriz *B);
Matriz *multiplica_matrizes_paralelo(Matriz *A, Matriz *B);
void    print_matrix(Matriz *imprime);
void    complete_matrix(Matriz *A, Matriz *B);
void    compare_matrix(Matriz *A, Matriz *B);

int main(void) {
	// Variaveis
	Matriz *A = NULL;
	Matriz *B = NULL;
	Matriz *sequencial = NULL;
	Matriz *paralelo   = NULL;
	clock_t c2, c1;
	float tempo;

	// Aloca duas matrizes na memoria
	A = alocar_matriz(LIM, LIM);
	B = alocar_matriz(LIM, LIM);

	// Adiciona valores para elas
	complete_matrix(A,B);

	// Imprime a quantidade de Linhas e Colunas
	printf("Tamanho Linhas/Colunas = %d\n", LIM);

	// Pegar o tempo antes da funcao
	c1 = clock();

	// Multiplicacao de Matrizes de forma sequencial
	sequencial = multiplica_matrizes(A,B);

	// tempo depois da funcao
	c2 = clock();

	// calculo do tempo
	tempo = (c2 - c1)*1000/CLOCKS_PER_SEC;

	// imprime o tempo na tela
	printf("tempo do sequencial: %.0fms\n", tempo);

	// Finaliza o programa
  return 0;
}//main

// Funçao que aloca uma matriz com n linhas e m colunas
Matriz *alocar_matriz(size_t linhas, size_t colunas){
	Matriz *matriz = malloc(sizeof(Matriz));

	matriz->linhas  = linhas;
	matriz->colunas = colunas;

	matriz->pos = malloc(linhas * sizeof(int *));
	for(size_t i = 0; i < linhas; i++)
		matriz->pos[i] = calloc(colunas, sizeof(int));

	return matriz;
}//alocar_matriz

// Multiplicacao de Matrizes de forma sequencial
Matriz *multiplica_matrizes(Matriz *A, Matriz *B){

  // Se as Linhas de A e as Colunas de B
  // forem diferentes, nao pode multiplicar
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
}

// Multiplicação de Matrizes de forma paralela!
Matriz *multiplica_matrizes_paralelo(Matriz *A, Matriz *B){
  // Se as Linhas de A e as Colunas de B
  // forem diferentes, não pode multiplicar
  if(A->linhas != B->colunas)
    return NULL;

  // Aloca na Memoria onde sera armazenado a Matriz Resultado
  Matriz *Resultado = alocar_matriz(A->linhas, B->colunas);

 	// Multiplicacao de Matrizes
 	#pragma omp parallel for
  for(size_t i = 0; i < A->linhas; ++i)
    for(size_t j = 0; j < B->colunas; ++j)
      for(size_t k = 0; k < A->colunas; ++k)
        Resultado->pos[i][j] += A->pos[i][k] * B->pos[k][j];

  // Retorna a Matriz Resultado
  return Resultado;
}//mult_matrix

void print_matrix(Matriz *imprime){
	if(imprime){
		for(size_t i = 0; i < imprime->linhas; i++){
			for(size_t j = 0; j < imprime->colunas; j++){
				printf("%d ", imprime->pos[i][j]);
			}
			printf("\n");
		}
		return;
	}

	printf("Matriz Nula!\n");
	return;
}//print_matrix

// Funcao que insere valores em duas matrizes
void complete_matrix(Matriz *A, Matriz *B){
	for(size_t i = 0; i < A->linhas; i++)
		for(size_t j = 0; j < A->colunas; j++)
			A->pos[i][j] = (i * A->colunas) + 2*j;

	for(size_t i = 0; i < B->linhas; i++)
		for(size_t j = 0; j < B->colunas; j++)
			B->pos[i][j] = (i * B->colunas) + (2*j) + 1;

	return;
}//complete_matrix

void compare_matrix(Matriz *A, Matriz *B){
	if(A->linhas != B->linhas || A->colunas != B->colunas){
		printf("Matrizes diferentes!\n");
		return;
	}

	for(size_t i = 0; i < A->linhas; i++){
		for(size_t j = 0; j < A->colunas; j++){
			if(A->pos[i][j] != B->pos[i][j]){
				printf("Matrizes diferentes!\n");
				return;
			}
		}
	}

	printf("Matrizes Iguais!\n");
	return;
}
