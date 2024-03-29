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


//   Os trens circulam de forma horária entre os trilhos

//   + - - - 1 - - - + - - - 2 - - - + - - - 3 - - - +
//   |               |               |               |
//   |               |               |               |
//   |               |               |               |
//   4     trem1     5     trem2     6     trem3     7
//   |               |               |               |
//   |               |               |               |
//   |               |               |               |
//   + - - - 8 - - - + - - - 9 - - - + - - - 10- - - +


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

pthread_mutex_t t5; /* proteção para o trilho 5 */
pthread_mutex_t t6; /* proteção para o trilho 6 */

void L(int trem,int trilho){
      printf("trem %d no trilho %d\n", trem, trilho);
}

void *trem1(void *arg)
{
  int trem = 1;
  while(true){
	  L(trem, 1);
	  sleep(trem);
	  pthread_mutex_lock(&t5);
	  L(trem, 5);
	  sleep(trem);
	  pthread_mutex_unlock(&t5);
	  L(trem, 8);
	  sleep(trem);
	  L(trem, 4);
	  sleep(trem);
  }
  pthread_exit(0);
}

void *trem2(void *arg)
{
  int trem = 2;
  while(true){
    L(trem, 9);
    sleep(trem);
    pthread_mutex_lock(&t5);
    L(trem, 5);
    sleep(trem);
    pthread_mutex_unlock(&t5);
    L(trem, 2);
    sleep(trem);
    pthread_mutex_lock(&t6);
    L(trem, 6);
    sleep(trem);
    pthread_mutex_unlock(&t6);
  }
  pthread_exit(0);
}

void *trem3(void *arg)
{
  int trem = 3;
  while(true){
	  L(trem, 10);
	  sleep(trem);
	  pthread_mutex_lock(&t6);
	  L(trem, 6);
	  sleep(trem);
	  pthread_mutex_unlock(&t6);
	  L(trem, 3);
	  sleep(trem);
	  L(trem, 7);
	  sleep(trem);
  }
  pthread_exit(0);
}

int main()
{
  int res;
  pthread_t thread1, thread2, thread3;

  void *thread_result;

  // ------ criando multex t5 ------
  res = pthread_mutex_init(&t5, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex t5 falhou");
    exit(EXIT_FAILURE);
  }

  // ------ criando multex t6 ------
  res = pthread_mutex_init(&t6, NULL);
  if (res != 0)
  {
    perror("Iniciação do Mutex t6 falhou");
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

  //------ Thread 3 (executa a fn: trem 3) ------
  res = pthread_create(&thread3, NULL, trem3, NULL);
  if (res != 0)
  {
    perror("Criação da thread 3 falhou");
    exit(EXIT_FAILURE);
  }

  // ----- Espera termino das threads
  res = pthread_join(thread1, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread 1 falhou");
    exit(EXIT_FAILURE);
  }
  res = pthread_join(thread2, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread 2 falhou");
    exit(EXIT_FAILURE);
  }
  res = pthread_join(thread3, &thread_result);
  if (res != 0)
  {
    perror("Juncao da Thread 3 falhou");
    exit(EXIT_FAILURE);
  }

  printf("MAIN() --> Thread foi juntada com sucesso\n");

  //----- destruíndo mutex
  pthread_mutex_destroy(&t5);
  pthread_mutex_destroy(&t6);
  exit(EXIT_SUCCESS);
}
