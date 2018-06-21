#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 512 // Precisa ser em Potencia de 2

// Tipo para Vetorizacao
typedef union{
  int __attribute__((vector_size(SIZE*sizeof(int)))) vec;
  int elem[SIZE];
} intvec512;

// Conjunto com as Linhas da Matriz
typedef struct{
  intvec512 *linhas;
  size_t qt;
} MatrizL;

// Conjunto com as Colunas da Matriz
typedef struct{
  intvec512 *colunas;
  size_t qt;
} MatrizC;

// Matriz de 2 dimensoes de Inteiros
typedef struct{
  int **pos;
  size_t qt;
} Matriz;

//--------------------------------------------

MatrizL *alocar_matrizL(size_t qt);
MatrizC *alocar_matrizC(size_t qt);
Matriz  *alocar_matriz(size_t qt);
void     completar_matrizes(MatrizL *A, MatrizC *B);
Matriz  *multiplica_matrizes_SIMD(MatrizL *A, MatrizC *B);
int      soma_vec(intvec512 vetor);

// Funcao Main
int main (void){
  // Variaveis
  MatrizL *A = NULL;
  MatrizC *B = NULL;
  Matriz  *Resultado = NULL;
	clock_t c2, c1;
	float tempo;

  // Aloca as matrizes
  A = alocar_matrizL(SIZE);
  B = alocar_matrizC(SIZE);

  // Aloca valores as matrizes
  completar_matrizes(A,B);

  // Imprime informacoes
  printf("UTILIZANDO INSTRUCOES SIMD\n");
  printf("Quantidade de Linhas/Colunas = %d\n", SIZE);

  // tempo antes da funcao
  c1 = clock();

  // Multiplicacao de Matrizes por Vetorizacao
  Resultado = multiplica_matrizes_SIMD(A,B);

  // tempo depois da funcao
  c2 = clock();

  // calculo do tempo
  tempo = (c2 - c1)*1000/CLOCKS_PER_SEC;

  // imprime o tempo
  printf("tempo do simd: %.0fms\n", tempo);

  // finaliza o programa
  return 0;
}//main

// Aloca matriz das Linhas
MatrizL *alocar_matrizL(size_t qt){
  MatrizL *matriz = malloc(sizeof(MatrizL));
  matriz->linhas = malloc(qt * sizeof(intvec512));
  matriz->qt = qt;
  return matriz;
}//alocar_matrizL

// Aloca matriz das colunas
MatrizC *alocar_matrizC(size_t qt){
  MatrizC *matriz = malloc(sizeof(MatrizC));
  matriz->colunas = malloc(qt * sizeof(intvec512));
  matriz->qt = qt;
  return matriz;
}//alocar_matrizC

// Aloca matriz de duas dimensoes de inteiros
Matriz  *alocar_matriz(size_t qt){
  Matriz *matriz = malloc(sizeof(Matriz));
  matriz->pos = malloc(qt * sizeof(int *));
  matriz->qt = qt;
  for(size_t i = 0; i < qt; i++)
    matriz->pos[i] = malloc(qt * sizeof(int));
  return matriz;
}//alocar_matriz

// Adiciona valores para as Matrizes
void     completar_matrizes(MatrizL *A, MatrizC *B){
  for(size_t i = 0; i < A->qt; i++){
    for(size_t j = 0; j < B->qt; j++){
      A->linhas[i].elem[j]  = (i * A->qt) + 2*j;
      B->colunas[j].elem[i] = (i * B->qt) + (2*j) + 1;
    }
  }
}//completar_matrizes

// Multiplica as matrizes com vetorizacao
Matriz  *multiplica_matrizes_SIMD(MatrizL *A, MatrizC *B){
  // Vetor auxiliar
  intvec512 aux;

  // Aloca na Memoria onde sera armazenado a Matriz Resultado
  Matriz *Resultado = alocar_matriz(A->qt);

  // Multiplicacao de Matrizes utilizando vetorizacao
  for(size_t i = 0; i < A->qt; i++){
    for(size_t j = 0; j < B->qt; j++){
      aux.vec = A->linhas[i].vec * B->colunas[j].vec;
      Resultado->pos[i][j] = soma_vec(aux);
    }
  }

  return Resultado;
}//multiplica_matrizes_SIMD

// Soma todas os valores do vetor
int soma_vec(intvec512 vetor){
  int retorno = 0;

  for(size_t i = 0; i < SIZE; i++)
    retorno += vetor.elem[i];

  return retorno;
}
