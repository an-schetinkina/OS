#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "pthread.h"

// число тредов 7;
// размер матрицы 2048;
// блоков 16;
// 2048/16 = 128;

size_t matrix1[2048][2048], matrix2[2048][2048], m_result[2048][2048];
size_t block_matrix1[128][128][16][16], block_matrix2[128][128][16][16], b_result[128][128][16][16];

void* matrix_mul(void *id) {
    size_t* thread = (size_t*)(id);

    for (size_t i=*thread; i<2048; i+=7) {
        for (size_t j=0; j<2048; j++) {
			size_t res = 0;
            for (size_t k = 0; k < 2048; k++) {
                res += matrix1[i][k] * matrix2[k][j];
            }
			m_result[i][j] = res;
        }
    }
}

void* block_matrix_mul(void *id) {

    size_t* thread = (size_t*)(id);

    for (size_t i=*thread; i<2048; i+=7) 
        for (size_t j=0; j<128; j++)
            for (size_t jj=0; jj<16; jj++)
                for (size_t k=0; k<128; k++)
                    for (size_t kk=0; kk<16; kk++)
                        b_result[i/16][j][i%16][jj] += block_matrix1[i/16][k][i%16][kk] * block_matrix2[k][j][kk][jj];

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




