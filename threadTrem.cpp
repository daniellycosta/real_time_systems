//
//  thread7.cpp
//
//
//  Created by Affonso on 25/10/16.
//
//

// #include "thread7.hpp"

// http: pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_mutex_init.html

// Programa que sincroniza threads utilizando-se mutexes
// Para compilá-lo utilise: g++ -o thread7 thread7.cpp -lpthread

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

void *trem1(void *arg);
void *trem2(void *arg);
pthread_mutex_t m1; /* proteção para: work_area e time_to_exit */
void L(int trem, int trilho);

int main()
{
  int res;
  pthread_t thread1;
  pthread_t thread2;

  void *trem1, *trem2;
  void *thread_result;
  // ------ criando multex m1 ------
  res = pthread_mutex_init(&m1, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex falhou");
    exit(EXIT_FAILURE);
  }

  //------ Thread 1 (executa a fn: trem 1) ------
  res = pthread_create(&thread1, NULL, trem1, NULL);
  if (res != 0)
  {
    perror("Criação da thread 1 falhou");
    exit(EXIT_FAILURE);
  }

  //------ Thread 2 (executa a fn: trem 2) ------
  res = pthread_create(&thread2, NULL, trem2, NULL);
  if (res != 0)
  {
    perror("Criação da thread 2 falhou");
    exit(EXIT_FAILURE);
  }

  pthread_mutex_lock(&m1);

  while (true)
  {
    printf("MAIN() --> Entre com algum texto. Entre com 'fim' para terminar\n");

    pthread_mutex_unlock(&m1);
    while (1)
    {
    }
  }
  // ----- Espera termino das threads
  res = pthread_join(thread1, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread falhou");
    exit(EXIT_FAILURE);
  }
  res = pthread_join(thread2, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread falhou");
    exit(EXIT_FAILURE);
  }

  printf("MAIN() --> Thread foi juntada com sucesso\n");

  //----- destruíndo mutex
  pthread_mutex_destroy(&m1);
  exit(EXIT_SUCCESS);
}

void *trem1(void *arg)
{

  while (1)
  {
    L(1, 1);
    sleep(1);
    L(1, 2);
    sleep(1);
    pthread_mutex_lock(&m1);
    L(1, 3);
    sleep(1);
    pthread_mutex_unlock(&m1);
    L(1, 4);
    sleep(1);
  }
  pthread_exit(0);
}

void *trem2(void *arg)
{
  while (1)
  {
    L(2, 5);
    sleep(1);
    L(2, 6);
    sleep(1);
    pthread_mutex_lock(&m1);
    L(2, 7);
    sleep(1);
    pthread_mutex_unlock(&m1);
    L(2, 8);
    sleep(1);
  }
  pthread_exit(0);
}

void L(int trem, int trilho)
{
  printf("trem %d no trilho %d\n", trem, trilho);
}
