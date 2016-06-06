#include "RWLock.h"

RWLock :: RWLock() {
  pthread_mutex_init(mutex, NIL);//NIL SON LOS ATRR
pthread_cond_init(&vc, NIL);
}

void RWLock :: rlock() {
  pthread_mutex_lock(&mutex);
  while (escritor)
    pthread_cond_wait(&vc, &mutex);
  lectores++;
  pthread_mutex_unlock(&mutex);
}

void RWLock :: wlock() {
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
    pthread_cond_broadcast($vc);
  pthread_mutex_unlock(&mutex);
}

void RWLock :: wunlock() {
  pthread_mutex_lock(&mutex);
  escritor = false;
  pthread_cond_broadcast(&vc);
  pthread_mutex_unlock(&mutex);
}
