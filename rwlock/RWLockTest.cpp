#include "RWLock.h"
#include <iostream>

using namespace std;

 int main(int argc, const char* argv[]){
	int cantThreads;
	cin >> cantThreads;
	int a;
	pthread_t thread[cantThreads]; 
	int tid;
	RWLock rwl;
	for (tid = 0; tid < cantThreads; ++tid){
		pthread_create(&thread[tid], NULL, aloha_honua, &tid);
	}
	return 0;
}
