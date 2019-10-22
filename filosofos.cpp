//
//  threadTrem.cpp
//
//
//  Created by Affonso on 25/10/16.
//
//

// http: pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_mutex_init.html

// Programa que sincroniza threads utilizando-se mutexes
// Para compilá-lo utilise: g++ -o threadTrem threadTrem.cpp -lpthread

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

pthread_mutex_t h1,h2,h3,h4,h5; /* Hashis */


void L(int trem,int trilho, int sleepTime){
      printf("trem %d no trilho %d\n", trem, trilho);
	  sleep(sleepTime);
}

void *fil1(void *arg)
{
  int fil = 1;
  while(true){
	  L(fil, 1, 1);
	  pthread_mutex_lock(&t5);
	  L(trem, 5, 5);
	  pthread_mutex_unlock(&t5);
	  pthread_mutex_lock(&t4);
	  L(trem, 4, 1);
	  pthread_mutex_unlock(&t4);
  }
  pthread_exit(0);
}

void *trem2(void *arg)
{
  int trem = 2;
  while(true){
    pthread_mutex_lock(&t5);
    L(trem, 5, 1);
    pthread_mutex_unlock(&t5);
    L(trem, 2, 1);
    pthread_mutex_lock(&t6);
    L(trem, 6, 5);
    pthread_mutex_unlock(&t6);
  }
  pthread_exit(0);
}

void *trem3(void *arg)
{
  int trem = 3;
  while(true){
	  pthread_mutex_lock(&t6);
	  L(trem, 6, 1);
	  pthread_mutex_unlock(&t6);
	  pthread_mutex_lock(&t4);
	  L(trem, 4, 5);
	  pthread_mutex_unlock(&t4);
	  L(trem, 3, 1);
  }
  pthread_exit(0);
}

int main()
{
  int res;
  pthread_t filosofo1, filosofo2, filosofo3,filosofo4,filosofo5;

  void *thread_result;

  // ------ criando multex h1 ------
  res = pthread_mutex_init(&h1, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h1 falhou");
    exit(EXIT_FAILURE);
  }

  // ------ criando multex h2 ------
  res = pthread_mutex_init(&h2, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h2 falhou");
    exit(EXIT_FAILURE);
  }

  // ------ criando multex h3 ------
  res = pthread_mutex_init(&h3, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h3 falhou");
    exit(EXIT_FAILURE);
  }

  // ------ criando multex h4 ------
  res = pthread_mutex_init(&h4, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h4 falhou");
    exit(EXIT_FAILURE);
  }

  // ------ criando multex h5 ------
  res = pthread_mutex_init(&h5, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex h5 falhou");
    exit(EXIT_FAILURE);
  }

  

  //------ Thread 1 (executa a fn: fil 1) ------
  res = pthread_create(&filosofo1, NULL, fil1, NULL);
  if (res != 0)
  {
    perror("Criação da thread: filosofo 1 falhou");
    exit(EXIT_FAILURE);
  }

    //------ Thread 2 (executa a fn: fil 1) ------
  res = pthread_create(&filosofo2, NULL, fil2, NULL);
  if (res != 0)
  {
    perror("Criação da thread: filosofo 2 falhou");
    exit(EXIT_FAILURE);
  }
    //------ Thread 3 (executa a fn: fil 3) ------
  res = pthread_create(&filosofo3, NULL, fil1, NULL);
  if (res != 0)
  {
    perror("Criação da thread: filosofo 3 falhou");
    exit(EXIT_FAILURE);
  }
    //------ Thread 4 (executa a fn: fil 4) ------
  res = pthread_create(&filosofo4, NULL, fil4, NULL);
  if (res != 0)
  {
    perror("Criação da thread: filosofo 4 falhou");
    exit(EXIT_FAILURE);
  }
    //------ Thread 5 (executa a fn: fil 5) ------
  res = pthread_create(&filosofo5, NULL, fil5, NULL);
  if (res != 0)
  {
    perror("Criação da thread: filosofo 5 falhou");
    exit(EXIT_FAILURE);
  }

 

  // ----- Espera termino das threads
  res = pthread_join(filosofo1, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread: filosofo 1 falhou");
    exit(EXIT_FAILURE);
  }
  res = pthread_join(filosofo2, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread: filosofo 2 falhou");
    exit(EXIT_FAILURE);
  }
  res = pthread_join(filosofo3, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread:filosofo 3 falhou");
    exit(EXIT_FAILURE);
  }
    res = pthread_join(filosofo4, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread:filosofo 4 falhou");
    exit(EXIT_FAILURE);
  }
    res = pthread_join(filosofo5, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread:filosofo 5 falhou");
    exit(EXIT_FAILURE);
  }

  printf("MAIN() --> Thread foi juntada com sucesso\n");

  //----- destruíndo mutex
  pthread_mutex_destroy(&h1);
  pthread_mutex_destroy(&h2);
  pthread_mutex_destroy(&h3);
  pthread_mutex_destroy(&h4);
  pthread_mutex_destroy(&h5);
 
  exit(EXIT_SUCCESS);
}
