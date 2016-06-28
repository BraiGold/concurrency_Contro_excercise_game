#include "RWLock.h"
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


RWLock rwl;
int global;//variable en memoria "compartida" osea global

void* read(void *data);
void* write(void *data);
void test_deadlock();
void test_inanicion();
void* writeTermino(void *data);
void* readTermino(void *data);

int main(int argc, char* argv[]) {
  std::cout << "Empezando test:" << std::endl;
  test_deadlock();
  std::cout << "no hay deadLock" << std::endl;
  test_inanicion();
  std::cout << "no hay inanicion" << std::endl;
  return 0;
}

void test_deadlock(){
  pthread_t thread[50];
  //creo los thread alternados entre read y write
  for(int i=0; i<50; i++){
    if(i % 8 ==0){
      pthread_create(&thread[i],NULL,write,NULL);
    }else{
      pthread_create(&thread[i],NULL,read,NULL);
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
  std::vector<pthread_t> tidsParaJoin;
    pthread_t tidR;
    pthread_create(&tidR,NULL,readTermino,NULL);
    pthread_t tidW;
    pthread_create(&tidW,NULL,writeTermino,NULL);
    while(!terminoR || !terminoW){
      pthread_t tid1;
      pthread_create(&tid1,NULL,read,NULL);
      pthread_t tid2;
      pthread_create(&tid2,NULL,write,NULL);
      tidsParaJoin.push_back(tid1);
      tidsParaJoin.push_back(tid2);
    }
    std::cout << "termino bien" << std::endl;
    for (int i = 0; i < tidsParaJoin.size() ; i++) {
      pthread_join(tidsParaJoin[i],NULL);
    }
}

void* readTermino(void *data) {
        rwl.rlock();
        sleep(3);//para darle tiempo a los conflictos
        int aux=global;
        rwl.runlock();
        return NULL;
}


void* writeTermino(void *data) {
        rwl.wlock();
        sleep(3);//para darle tiempo a los conflictos
        global++;
        rwl.wunlock();
        terminoW=true;
        return NULL;
}

void* read(void *data) {
        rwl.rlock();
        sleep(1);//para darle tiempo a los conflictos
        int aux=global;
        rwl.runlock();
        terminoR=true;
        return NULL;
}


void* write(void *data) {
        rwl.wlock();
        sleep(1);//pa darle tiempo a los conflictos
        global++;
        rwl.wunlock();
        return NULL;
}
