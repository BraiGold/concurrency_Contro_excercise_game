#include "RWLock.h"
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#define CANT_THREAD_LECTORES 5
#define CANT_THREAD_ESCRITORES 2

RWLock rw;
int compartida;
int num;
void *write(void *data);
void *read(void *data);
void test_lectores_basico();
void test_lectores_unEscritor();
void test_lectores_unEscritor_mas_lectores();
void test_lectores_escritores_mas_lectores();
void test_muchos_lectores_escritores_intercaladosinanicion();

int main(int argc, char* argv[]) {

        //Creamos el lock
        rw = RWLock();

       
        //Corremos tests
        assert(test_lectores_basico(), 10);
        puts("Test_lectores_basico OK...");

        test_lectores_unEscritor();
        puts("Test_lectores_unEscritor Ok...");

        test_lectores_unEscritor_mas_lectores();

        puts("Test_lectores_unEscritor_mas lectores OK...");

        test_lectores_escritores_mas_lectores();
        puts("test_lectores_escritores_mas_lectores Ok...");

        test_muchos_lectores_escritores_intercaladosinanicion();
        puts("Ok");
   
    return 0;
}      

int test_lectores_basico(){

        compartida = 10;

        pthread_t thread[CANT_THREAD_LECTORES];

        //Creamos los threads lectores
        for(int i=0; i<CANT_THREAD_LECTORES; i++) {
            pthread_create(&thread[i],NULL,read,NULL);
        }

        //Esperamos exits      
        for(int i=0; i<CANT_THREAD_LECTORES; i++) {
            pthread_join(thread[i],NULL); //esperar exit
        }
        return compartida;
}

void test_lectores_unEscritor(){

        compartida = 4;

        pthread_t thread[CANT_THREAD_LECTORES +1];
        int i = 0;

        //Creamos un escritor
        pthread_create(&thread[i],NULL,write,NULL);
        //Chreamos los threads lectores
         for(i=1;i<CANT_THREAD_LECTORES+1;i++)
                pthread_create(&thread[i],NULL,read,NULL);


        // //Esperamos exits de todos
        for(i=0;i<CANT_THREAD_LECTORES+1;i++)
                pthread_join(thread[i],NULL);

        num = 0;
       
}


void test_lectores_unEscritor_mas_lectores(){

        compartida = 4;

        pthread_t thread[CANT_THREAD_LECTORES +1];
        int i = 0;

        //Chreamos los threads lectores
         for(i=0;i<(CANT_THREAD_LECTORES+1) / 2;i++)
                pthread_create(&thread[i],NULL,read,NULL);

        //Creamos un escritor
        pthread_create(&thread[i],NULL,write,NULL);
         
        //Chreamos los threads lectores
         for(i=((CANT_THREAD_LECTORES+1) / 2) +1;i<CANT_THREAD_LECTORES+1;i++)
                pthread_create(&thread[i],NULL,read,NULL);

        // //Esperamos exits de todos
        for(i=0;i<CANT_THREAD_LECTORES+1;i++)
                pthread_join(thread[i],NULL);

        num  = 0;
       
}



void test_lectores_escritores_mas_lectores(){

        compartida = 4;

        pthread_t thread[CANT_THREAD_LECTORES +CANT_THREAD_ESCRITORES];
        int i = 0;

        //Chreamos los threads lectores
         for(i=0;i<CANT_THREAD_LECTORES / 2;i++)
                pthread_create(&thread[i],NULL,read,NULL);

        //Creamos un escritor
          for(i=CANT_THREAD_LECTORES / 2;i < CANT_THREAD_ESCRITORES+CANT_THREAD_LECTORES/2;i++)
                pthread_create(&thread[i],NULL,write,NULL);

        //Chreamos los threads lectores
         for(i=CANT_THREAD_LECTORES/2+CANT_THREAD_ESCRITORES;i<CANT_THREAD_LECTORES+CANT_THREAD_ESCRITORES;i++)
                pthread_create(&thread[i],NULL,read,NULL);

        // //Esperamos exits de todos
        for(i=0;i<CANT_THREAD_LECTORES+CANT_THREAD_ESCRITORES;i++)
                pthread_join(thread[i],NULL);

        num = 0;
       
}

void test_muchos_lectores_escritores_intercaladosinanicion(){

        compartida = 4;

        pthread_t thread[1000];
        int i = 0;

        //Chreamos los threads lectores
         for(i=0;i<250;i++)
                pthread_create(&thread[i],NULL,read,NULL);

        //Creamos un escritor
          for(i=250;i < 251;i++)
                pthread_create(&thread[i],NULL,write,NULL);

        //Chreamos mas lectores
         for(i=251;i<500;i++)
                pthread_create(&thread[i],NULL,read,NULL);
               
         //Creamos un escritor  
          pthread_create(&thread[500],NULL,write,NULL);
         
          //Creamos mas lectores
         for(i=501;i<750;i++)
                pthread_create(&thread[i],NULL,read,NULL);

        //Creamos un escritor

         pthread_create(&thread[750],NULL,write,NULL);
       
         //Y mas lectores
          for(i=751;i<1000;i++)
                pthread_create(&thread[i],NULL,read,NULL);
       
        // //Esperamos exits de todos
        for(i=0;i<1000;i++)
                        pthread_join(thread[i],NULL);

        num = 0;
       
}

void *read(void *data) { 
        int tid = pthread_self();
        rw.rlock();
                sleep(3);
                int copiarCompartida = compartida;
        rw.runlock();
        return NULL;
}


void *write(void *data) {
        int tid = pthread_self();
        rw.wlock();
                num++;
                sleep(3);
                compartida--;
        rw.wunlock();
        return NULL;
}
