# MatrixMultiplicationOtimization

## Otimizações no algoritmo de Multiplicação de Matrizes

Dois arquivos estão adicionados no repositório:
  * [mult_mat_simples.c](https://github.com/juan-burtet/MatrixMultiplicationOtimization/blob/master/mult_mat_simples.c)
  * [mult_mat_SIMD.c](https://github.com/juan-burtet/MatrixMultiplicationOtimization/blob/master/mult_mat_SIMD.c)

## mult_mat_simples.c

Faz a multiplicação de matrizes de maneira trivial.

Para executar o código, insira:
  > gcc mult_mat_simples.c -o __executavelsimples__

Para executar o código com as flags de vetorização automática, insira:
  > gcc -funsafe-math-optimizations -march=native -O3 -ftree-vectorize -fopt-info-vec-optimized -fopt-info-vec-missed mult_mat_simples.c -o __executavelsimplesflags__
  
## mult_mat_SIMD.c

Faz a multiplicação de matrizes utilizando Instruções SIMD + Vetorização

Para executar o código, insira:
  > gcc -march=native -O2 mult_mat_SIMD.c -o __executavelsimd__

Para executar o código com as flags de vetorização automática, insira:
  > gcc -funsafe-math-optimizations -march=native -O -ftree-vectorize -fopt-info-vec-optimized -fopt-info-vec-missed mult_mat_simples.c -o __executavelsimdsflags__
