#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "pthread.h"

// число тредов 7;
// размер матрицы 2048;
// блоков 16;
// 2048/16 = 128;


size_t** matrix1, ** matrix2, m_result[2048][2048];
size_t**** block_matrix1,**** block_matrix2, b_result[128][16][128][16];

void* matrix_mul(void *id) {
    size_t* thread = (size_t*)(id);
    for (size_t i=0; i<2048; i++) {
        if (i%7 == *thread) {
            for (size_t j=0; j<2048; j++) {
                size_t res = 0;
                for (size_t k=0; k<2048; k++) {
                    res += matrix1[i][k] * matrix2[k][j];
                }
                m_result[i][j] = res;
            }
        }
    }
}

void* block_matrix_mul(void *id)
{
    size_t* thread = (size_t*)(id);
    for (size_t i=0; i<2048; i++) {
        if (i%7 == *thread) {
            for (size_t j=0; j<2048; j++) {
                size_t res = 0;
                for (size_t k=0; k<2048; k++) {
                    res += block_matrix1[i/16][i%16][k/16][k%16] * block_matrix2[k/16][k%16][j/16][j%16];
                }
                b_result[i/16][i%16][j/16][j%16] = res;
            }
        }
    }
}


void testing_matrix()
{
    pthread_t threads[7];
    for (size_t i=0; i<7; i++)
        pthread_create(&threads[i], NULL, matrix_mul, new size_t(i));

    for (size_t i=0; i<7; i++)
        pthread_join(threads[i], NULL);
}

void testing_block_matrix()
{
    pthread_t threads[7];
    for (size_t i=0; i<7; i++)
        pthread_create(&threads[i], NULL, block_matrix_mul, new size_t(i));

    for (size_t i=0; i<7; i++)
        pthread_join(threads[i], NULL);
}


int main(void)
{
	//matrix initialization
    matrix1 = new size_t*[2048];
    matrix2 = new size_t*[2048];
    block_matrix1 = new size_t***[128];
    block_matrix2 = new size_t***[128];

    for (size_t i=0; i<2048; i++) {
        matrix1[i] = new size_t[2048];
        matrix2[i] = new size_t[2048];
    }

    for (size_t i=0; i<128; i++) {
        block_matrix1[i] = new size_t**[16];
        block_matrix2[i] = new size_t**[16];
        for (size_t j=0; j<16; j++) {
            block_matrix1[i][j] = new size_t*[128];
            block_matrix2[i][j] = new size_t*[128];
            for (size_t k=0; k<128; k++) {
                block_matrix1[i][j][k] = new size_t[16];
                block_matrix2[i][j][k] = new size_t[16];
            }
        }
    }

	float start, end;

	//test matrix
    start = (float)clock()/CLOCKS_PER_SEC;
    testing_matrix();
    end = (float)clock()/CLOCKS_PER_SEC;
    printf("Matrix: %0.2f\n", end-start);
	
	//test block_matrix
    start = (float)clock()/CLOCKS_PER_SEC;
    testing_block_matrix();
    end = (float)clock()/CLOCKS_PER_SEC;
    printf("Blocks: %0.2f\n", end-start);
    
    return 0;
}




