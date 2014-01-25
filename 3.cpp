#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "pthread.h"

volatile int counters[300];
volatile int counters128[300][128];

void* test(void* id) {
    int* i = (int*)id;
    for (int j = 0; j < 60000; j++) counters[*i] += 1;
}


void* test_128(void* id) {
    int* i = (int*)id;
    for (int j = 0; j < 60000; j++) counters128[*i][0] += 1;
}

void testing(void*(*function)(void*)) {
    pthread_t threads[300];
    for (int i = 0; i < 300; i++) pthread_create(&threads[i], NULL, function, new int(i));
    for (int i = 0; i < 300; i++) pthread_join(threads[i], NULL);
}

int main(void) {

    srand((int)time(0));
	float start, end;

	//test usual
    start = (float)clock()/CLOCKS_PER_SEC;
    testing(test);   
    end = (float)clock()/CLOCKS_PER_SEC;
    printf("Usual: %0.2f\n", end-start);

    //test 128
    start = (float)clock()/CLOCKS_PER_SEC;
    testing(test_128);
    end = (float)clock()/CLOCKS_PER_SEC;
    printf("128: %0.2f\n", end-start);

	return 0;
}
