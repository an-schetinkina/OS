#include "stdlib.h"
#include "stdio.h"
#include <time.h>

int main(void)
{
    srand((int)time(0)); 
    size_t size = (size_t) 3 * 1024 * 1024 * 1024 * 5 / 4;
    char* memory = (char*)malloc(size);
    float start, end;

    start = (float)clock()/CLOCKS_PER_SEC;
	for (int i = 0; i < 2000000; ++i) 
		memory[i % size] = '1';
    end = (float)clock()/CLOCKS_PER_SEC;
    printf("Increment: %0.2f\n", end - start);
    
    start = (float)clock()/CLOCKS_PER_SEC;
	for (int i = 0; i < 2000000; ++i)
		memory[rand() % size] = '1';
    end = (float)clock()/CLOCKS_PER_SEC;
    printf("Random: %0.2f\n", end - start);

	return 0;
}














