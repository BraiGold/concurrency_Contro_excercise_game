#include "RWLock.h"
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#define LECTORES 5
#define ESCRITORES 7

RWLock rw;
int compartida;

void *read(void *data) { 
        rw.rlock();
                sleep(1);
                int copiarCompartida = compartida;
        rw.runlock();
        return NULL;
}


void *write(void *data) {
        rw.wlock();
                sleep(1);
                compartida--;
        rw.wunlock();
        return NULL;
}

int test_soloLectores(){

        compartida = 10;

        pthread_t thread[LECTORES];

        //Creamos los threads lectores
        for(int i=0; i<LECTORES; i++) {
            pthread_create(&thread[i],NULL,read,NULL);
        }

        //Esperamos exits      
        for(int i=0; i<LECTORES; i++) {
            pthread_join(thread[i],NULL); //esperar exit
        }
        return compartida;
}

int test_soloEscritores(){

        compartida = 10;

        pthread_t thread[ESCRITORES];

        //Creamos los threads lectores
        for(int i=0; i<ESCRITORES; i++) {
            pthread_create(&thread[i],NULL,write,NULL);
        }

        //Esperamos exits      
        for(int i=0; i<ESCRITORES; i++) {
            pthread_join(thread[i],NULL); //esperar exit
        }
        return compartida;
}

int test_unEscritorMuchosLectores(){

        compartida = 10;

        pthread_t thread[LECTORES + 1];

        //Creamos un escritor
        pthread_create(&thread[0],NULL,write,NULL);

        //Chreamos los threads lectores
        for(int i=1; i<LECTORES+1; i++) {
               pthread_create(&thread[i],NULL,read,NULL);
        }

        // //Esperamos exits de todos
        for(int i=0; i<LECTORES+1; i++) { 
                pthread_join(thread[i],NULL); //esperar exit
        }

        return compartida;       
}


int test_lectoresEscritorLectores(){

        compartida = 10;

        pthread_t thread[LECTORES +1];

        //Creamos los threads lectores
         for(int i=0; i<(LECTORES+1) / 2; i++) {
                pthread_create(&thread[i],NULL,read,NULL);
         }

        //Creamos un escritor
        pthread_create(&thread[(LECTORES+1)/2],NULL,write,NULL);
         
        //Creamos los threads lectores
         for(int i=((LECTORES+1) / 2) +1; i<LECTORES+1;i++)
                pthread_create(&thread[i],NULL,read,NULL);

        // //Esperamos exits de todos
        for(int i=0;i<LECTORES+1;i++)
                pthread_join(thread[i],NULL);

        return compartida;       
}

int test_escritoresLectorEscritores(){

        compartida = 10;

        pthread_t thread[ESCRITORES +1];

        //Creamos los threads ESCRITORES
         for(int i=0; i<(ESCRITORES+1) / 2; i++) {
                pthread_create(&thread[i],NULL,write,NULL);
         }

        //Creamos un LECTOR
        pthread_create(&thread[(ESCRITORES+1)/2],NULL,read,NULL);
         
        //Creamos los threads ESCRITORES
         for(int i=((ESCRITORES+1) / 2) +1; i<ESCRITORES+1;i++)
                pthread_create(&thread[i],NULL,write,NULL);

        // //Esperamos exits de todos
        for(int i=0;i<ESCRITORES+1;i++)
                pthread_join(thread[i],NULL);

        return compartida;       
}

int test_lectoresEscritoresLectores(){

        compartida = 10;

        pthread_t thread[LECTORES + ESCRITORES];

        //Creamos la mitad de los threads lectores
        for(int i = 0; i<LECTORES / 2; i++)
                pthread_create(&thread[i],NULL,read,NULL);

        //Creamos los threads escritores
        for(int i = LECTORES / 2; i < ESCRITORES+LECTORES/2; i++)
                pthread_create(&thread[i],NULL,write,NULL);

        //Creamos la mitad de los threads lectores
        for(int i = LECTORES/2+ESCRITORES; i<LECTORES+ESCRITORES; i++)
                pthread_create(&thread[i],NULL,read,NULL);

        //Esperamos exits de todos
        for(int i = 0; i < LECTORES + ESCRITORES; i++)
                pthread_join(thread[i],NULL);

        return compartida;       
}

int test_muchosLectoresPocosEscritores(){

        compartida = 10;

        pthread_t thread[100];

        //Creamos los threads lectores
        for(int i=0;i<25;i++){
            pthread_create(&thread[i],NULL,read,NULL);
        }

        //Creamos un escritor      
        pthread_create(&thread[25],NULL,write,NULL);

        //Creamos mas lectores
        for(int i=26;i<50;i++){
            pthread_create(&thread[i],NULL,read,NULL);
        }
               
        //Creamos un escritor  
        pthread_create(&thread[50],NULL,write,NULL);
         
        //Creamos mas lectores
        for(int i=51;i<75;i++){
            pthread_create(&thread[i],NULL,read,NULL);
        }

        //Creamos un escritor
        pthread_create(&thread[75],NULL,write,NULL);
       
        //Y mas lectores
        for(int i=76;i<100;i++){
            pthread_create(&thread[i],NULL,read,NULL);
        }
       
        // //Esperamos exits de todos
        for(int i=0;i<100;i++)
            pthread_join(thread[i],NULL);

        return compartida;
       
}

int test_muchosEscritoresPocosLectores(){

        compartida = 100;

        pthread_t thread[100];

        //Creamos los threads escritores
        for(int i=0; i<25; i++){
            pthread_create(&thread[i],NULL,write,NULL);
        }

        //Creamos un lector      
        pthread_create(&thread[25],NULL,read,NULL);

        //Creamos escritores
        for(int i=26; i<50; i++){
            pthread_create(&thread[i],NULL,write,NULL);
        }
               
        //Creamos un lector  
        pthread_create(&thread[50],NULL,read,NULL);
         
        //Creamos escritores
        for(int i=51; i<75; i++){
            pthread_create(&thread[i],NULL,write,NULL);
        }

        //Creamos un lector
        pthread_create(&thread[75],NULL,read,NULL);
       
        //Creamos escritores
        for(int i=76; i<100; i++)
            pthread_create(&thread[i],NULL,write,NULL);
       
        // //Esperamos exits de todos
        for(int i=0; i<100; i++)
            pthread_join(thread[i],NULL);

        return compartida;
       
}

int main(int argc, char* argv[]) {

        //Creamos el lock
        rw = RWLock();

       
        //Corremos tests
        assert(test_soloLectores() == 10);
        puts("Test solo lectores OK...");

        assert(test_soloEscritores() == 3);
        puts("Test solo escritores OK...");

        assert(test_unEscritorMuchosLectores() == 9);
        puts("Test un escritor y varios lectores OK...");

        assert(test_lectoresEscritorLectores() == 9);
        puts("Test varios lectores con un escritor(en el medio) OK...");

        assert(test_escritoresLectorEscritores() == 3);
        puts("Test varios escritores con un lector(en el medio) OK...");

        assert(test_lectoresEscritoresLectores() == 3 );
        puts("Test lectores y escritores OK...");

        assert(test_muchosLectoresPocosEscritores() == 7);
        puts("Test muchos lectores y pocos escritores OK...");
   
        assert(test_muchosEscritoresPocosLectores() == 3);
        puts("Test muchos escritores y pocos lectores OK...");

        return 0;
}