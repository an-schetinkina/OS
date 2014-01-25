#include <stdio.h>
#include <stdlib.h>
#include "vector"
#include "time.h"
#include "pthread.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h> //forks
using std::vector;

void* testing(void*) {
	vector<char*> blocks;
	for (int k = 0; k < 100000; ++k) {
		float p = drand48();
		// take mem
		if (p < 0.333 || !blocks.size()) { 
			int i = 256 * (rand() % 4 + 1);
			blocks.push_back(new char[i]);
			continue;
		}
	 	// free mem
		if (p < 0.666) {
			int i = rand() % blocks.size();
			delete[] blocks[i];
			blocks[i] = blocks.back();
			blocks.pop_back();
			continue;
		}
		// write to mem
		int i = rand() % blocks.size(); 
		blocks[i][100] = '1';
		continue;
	}
}

int main(void)
{
	srand((int)time(0));
	
	// test threads
    float start = (float)clock()/CLOCKS_PER_SEC; 
    pthread_t threads[20];
    for (int i = 0; i < 20; ++i) pthread_create(&threads[i], NULL, testing, NULL);
    for (int i = 0; i < 20; ++i) pthread_join(threads[i], NULL);
    float end = (float)clock()/CLOCKS_PER_SEC;
    printf("Threads: %0.2f\n", end - start); 

	// test processes
    start = (float)clock()/CLOCKS_PER_SEC; 
    pid_t pids[20];
    for(int i = 0; i < 20; ++i) {
        pids[i] = fork();
        if (pids[i] == 0) {
            testing(NULL);
            exit(0);
        }
    }
    wait(NULL);
    end = (float)clock()/CLOCKS_PER_SEC;
    printf("Processes: %0.2f\n", end - start); 
	
return 0;
}




















