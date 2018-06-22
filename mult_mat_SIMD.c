#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// SIZE precisa ser em Potencia de 2
#define SIZE 1024

/** Union usada para vetorizacao
  * vec - usado para instrucoes SIMD
  * elem - elementos do vetor
  */
typedef union{
  int __attribute__((vector_size(SIZE*sizeof(int)))) vec;
  int elem[SIZE];
} intvec512;

/** Struct de uma Matriz representada por N vetores (linhas)
  * linhas - qt vetores que representam as linhas
  * qt - quantidade de linhas
  */
typedef struct{
  intvec512 *linhas;
  size_t qt;
} MatrizL;

/** Struct de uma Matriz representada por N vetores (colunas)
  * colunas - qt vetores que representam as colunas
  * qt - quantidade de colunas
  */
typedef struct{
  intvec512 *colunas;
  size_t qt;
} MatrizC;

/** Struct de uma Matriz quadrada
  * pos - posicao da matriz
  * qt - quantidade de linhas/colunas
  */
typedef struct{
  int **pos;
  size_t qt;
} Matriz;

// Funcoes usadas no programa
MatrizL *alocar_matrizL(size_t qt);
MatrizC *alocar_matrizC(size_t qt);
Matriz  *alocar_matriz(size_t qt);
void     completar_matrizes(MatrizL *A, MatrizC *B);
Matriz  *multiplica_matrizes_SIMD(MatrizL *A, MatrizC *B);
int      soma_vec(intvec512 *vetor);
void     free_matrizL(MatrizL *destroy);
void     free_matrizC(MatrizC *destroy);
void     free_matriz(Matriz *destroy);

/** Funcao Main
  */
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
  printf("tempo usando SIMD: %.0fms\n", tempo);

  // desaloca as matrizes
  free_matrizL(A);
  free_matrizC(B);
  free_matriz(Resultado);

  // finaliza o programa
  return 0;
}//main

/** Funcao que aloca uma matrizL(representada por linhas)
  * @param qt - quantidade de linhas
  * @return - Matriz de Linhas alocada
  */
MatrizL *alocar_matrizL(size_t qt){
  MatrizL *matriz = malloc(sizeof(MatrizL));
  matriz->linhas = malloc(qt * sizeof(intvec512));
  matriz->qt = qt;
  return matriz;
}//alocar_matrizL

/** Funcao que aloca uma matrizC(representada por colunas)
  * @param qt - quantidade de colunas
  * @return - Matriz de colunas alocada
  */
MatrizC *alocar_matrizC(size_t qt){
  MatrizC *matriz = malloc(sizeof(MatrizC));
  matriz->colunas = malloc(qt * sizeof(intvec512));
  matriz->qt = qt;
  return matriz;
}//alocar_matrizC

/** Funcao que aloca uma matriz quadrada
  * @param qt - quantidade de linhas/colunas
  * @return retorna uma matriz quadrada
  */
Matriz *alocar_matriz(size_t qt){
  Matriz *matriz = malloc(sizeof(Matriz));
  matriz->pos = malloc(qt * sizeof(int *));
  matriz->qt = qt;
  for(size_t i = 0; i < qt; i++)
    matriz->pos[i] = malloc(qt * sizeof(int));
  return matriz;
}//alocar_matriz

/** Funcao que adiciona valores a Matriz de Linhas e a
  Matriz de colunas
  * @param A - Matriz de Linhas
  * @param B - Matriz de Colunas
  */
void completar_matrizes(MatrizL *A, MatrizC *B){
  for(size_t i = 0; i < A->qt; i++){
    for(size_t j = 0; j < B->qt; j++){
      A->linhas[i].elem[j]  = (i * A->qt) + 2*j;
      B->colunas[j].elem[i] = (i * B->qt) + (2*j) + 1;
    }
  }
}//completar_matrizes

/** Funcao que faz a Multiplicacao de Matrizes com instrucoes SIMD
  * @param A - Matriz de Linhas
  * @param B - Matriz de Colunas
  * @return Matriz quadrada multiplicada
  */
Matriz  *multiplica_matrizes_SIMD(MatrizL *A, MatrizC *B){
  // Vetor auxiliar
  intvec512 aux;

  // Aloca na Memoria onde sera armazenado a Matriz Resultado
  Matriz *Resultado = alocar_matriz(A->qt);

  // Multiplicacao de Matrizes utilizando vetorizacao
  for(size_t i = 0; i < A->qt; i++)
    for(size_t j = 0; j < B->qt; j++){
      aux.vec = A->linhas[i].vec * B->colunas[j].vec;
      Resultado->pos[i][j] = soma_vec(&aux);
    }

  // retorna a matriz multiplicada
  return Resultado;
}//multiplica_matrizes_SIMD

/** Funcao que soma todas as posicoes de um vetor(vetorizacao)
  * @param vetor - vetor(vetorizacao)
  * @return soma de todos as posicoes do vetor
  */
int soma_vec(intvec512 *vetor){
  // variavel inicializada em 0
  int retorno = 0;

  // soma de todas as posicoes
  for(size_t i = 0; i < SIZE; i++)
    retorno += vetor->elem[i];

  // retorna a soma
  return retorno;
}//soma_vec

/** Funcao que desaloca uma matriz de linhas
  * @param destroy - matriz de linhas
  */
void free_matrizL(MatrizL *destroy){
  free(destroy->linhas);
  free(destroy);
}//free_matrizL

/** Funcao que deloca uma matriz de colunas
  * @param destroy - matriz de colunas
  */
void free_matrizC(MatrizC *destroy){
  free(destroy->colunas);
  free(destroy);
}//free_matrizC

/** Funcao que desaloca uma matriz quadrada
  * @param destroy - matriz quadrada
  */
void free_matriz(Matriz *destroy){
  for(size_t i = 0; i < destroy->qt; i++)
    free(destroy->pos[i]);
  free(destroy->pos);
  free(destroy);
}//free_matriz
