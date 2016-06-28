#include "RWLock.h"
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NBR_THREADS 150


RWLock rw_lock;
int global;//variable en memoria "compartida" osea global

void* read(void *data);
void* write(void *data);
void test_deadlock();
void test_inanicion();
void test_verificador();
void* readGrande(void *data);

int main(int argc, char* argv[]) {
  std::cout << "Empezando test Deadlock:" << std::endl;
  test_deadlock();
  std::cout << "no hay deadLock" << std::endl;
  std::cout << "Empezando test Inanicion:" << std::endl;
  test_inanicion();
  std::cout << "no hay inanicion" << std::endl;
  std::cout << "Empezando test Verificador:" << std::endl;
  test_verificador();
  std::cout << "test OK" << std::endl;
  return 0;
}

void* read(void *data) 
{
  int id = *((int*) data);
  sleep(1);
  printf("r req [%02d] pide leer\n", id);
  rw_lock.rlock();
  printf("R IN [%02d] empieza a leer\n", id);
  sleep(1);
  printf("R OUT [%02d] termina de leer\n", id);
  rw_lock.runlock();
  pthread_exit(0);
}

void* write(void *data) 
{
  int id = *((int*) data);
  sleep(1);
  printf("w req [%02d] pide escribir\n", id);
  rw_lock.wlock();
  printf("W IN [%02d] empieza a escribir\n", id);
  sleep(1);
  printf("W OUT [%02d] termina de escribir\n", id);
  rw_lock.wunlock();
  pthread_exit(0);
}

void* readGrande(void *data) 
{
  int id = *((int*) data);
  printf("r req [%02d] pide leer\n", id);
  rw_lock.rlock();
  printf("R IN [%02d] empieza a leer\n", id);
  sleep(10);
  printf("R OUT [%02d] termina de leer\n", id);
  rw_lock.runlock();
  pthread_exit(0);
}


void test_deadlock(){
  pthread_t thread[50];
  int threads_ids[50];
  for (int i = 0; i < 50; i++) {
    threads_ids[i] = i;
  }
  //creo los thread alternados entre read y write
  for(int i=0; i<50; i++){
    if(i % 8 == 0){
      pthread_create(&thread[i],NULL,write,&threads_ids[i]);
    }else{
      pthread_create(&thread[i],NULL,read,&threads_ids[i]);
    }
  }
  //espero a que terminen
  for(int i=0; i<50; i++){
      pthread_join(thread[i],NULL);
    }
}


bool terminoR=false;
bool terminoW=false;

void test_inanicion(){
  pthread_t threads[NBR_THREADS];
  int threads_ids[NBR_THREADS];

  for (int i=0; i < NBR_THREADS; i++) 
  {
    threads_ids[i] = i;
    if (i % 15 == 0)
      pthread_create(&threads[i], NULL, write, &threads_ids[i]);
    else
      pthread_create(&threads[i], NULL, read, &threads_ids[i]);
  }

  for (int i=0; i < NBR_THREADS; i++) 
    pthread_join(threads[i], NULL);
}

void test_verificador() {
  pthread_t tidR;
  int threads_ids[2];
  threads_ids[0] = 0;
  threads_ids[1] = 1;  
  pthread_create(&tidR,NULL,readGrande,&threads_ids[0]);
  pthread_t tidW;
  pthread_create(&tidW,NULL,write,&threads_ids[1]);
  pthread_join(tidR, NULL);
  pthread_join(tidW, NULL);
}