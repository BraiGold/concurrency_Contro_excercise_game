#include "RWLock.h"

RWLock :: RWLock() {
pthread_mutex_init(&mutex, 0);//NIL SON LOS ATRR ese 0
pthread_cond_init(&vc, 0);//NIL SON LOS ATRR ese 0
pthread_mutex_init(&pedidoEscritura, 0);//NIL SON LOS ATRR ese 0
lectores=0;
escritor=false;
}

void RWLock :: rlock() {
  pthread_mutex_lock(&pedidoEscritura);
  pthread_mutex_lock(&mutex);
  while (escritor)
    pthread_cond_wait(&vc, &mutex);
  lectores++;
  pthread_mutex_unlock(&mutex);
  pthread_mutex_unlock(&pedidoEscritura);
}

void RWLock :: wlock() {
  pthread_mutex_lock(&pedidoEscritura);
  pthread_mutex_lock(&mutex);
  while (escritor || (lectores > 0))
    pthread_cond_wait(&vc, &mutex);
  escritor = true;
  pthread_mutex_unlock(&mutex);
}

void RWLock :: runlock() {
  pthread_mutex_lock(&mutex);
  lectores--;
  if (lectores == 0)
    pthread_cond_broadcast(&vc);
  pthread_mutex_unlock(&mutex);
}

void RWLock :: wunlock() {
  pthread_mutex_unlock(&pedidoEscritura);
  pthread_mutex_lock(&mutex);
  escritor = false;
  pthread_cond_broadcast(&vc);
  pthread_mutex_unlock(&mutex);
}
