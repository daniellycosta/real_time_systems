// http: pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_mutex_init.html

// Programa que sincroniza threads utilizando-se semaforos
// Para compilá-lo utilise: g++ -o semaphoreTrem semaphoreTrem.cpp -lpthread

//   Os trens circulam de forma horária entre os trilhos

//   + - - - 1 - - - + - - - 2 - - - + - - - 3 - - - +
//   |               |               |               |
//   |               |               |               |
//   |               |               |               |
//   1(4)  trem1     5     trem2     6     trem3     3(7)
//   |               |               |               |
//   |               |               |               |
//   |               |               |               |
//   + - - 1(8)- - - + - - - 9 - - - + - - 3(10)- - - +

#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

using namespace BlackLib;
sem_t bin_sem_t5;
sem_t bin_sem_t6;

// ----------- leds trem 1 ----------
BlackGPIO l1(GPIO_38, output);       //3
BlackGPIO l5_trem1(GPIO_39, output); //4
// ----------- leds trem 2 ----------
BlackGPIO l5_trem2(GPIO_62, output); //21
BlackGPIO l2(GPIO_37, output);       //22
BlackGPIO l6_trem2(GPIO_36, output); //23
BlackGPIO l9(GPIO_33, output);       //24
// ----------- leds trem 3 ----------
BlackGPIO l6_trem3(GPIO_72, output); //43
BlackGPIO l3(GPIO_73, output);       //44
//------------ Adc -----------------
ADC vel_trem1(AINx::AIN4);
ADC vel_trem2(AINx::AIN6);
ADC vel_trem3(AINx::AIN5);

int normalizeAdc(int trem, int leituraADC)
{
  printf("trem %d com velocidade %d\n", trem, (leituraADC % 10) + 1);
  return (leituraADC % 5) + 1;
}

void L(int trem, int trilho)
{
  printf("trem %d no trilho %d\n", trem, trilho);
}

void *trem1(void *arg)
{
  int trem = 1;
  while (true)
  {
    int sleepTime = normalizeAdc(trem, vel_trem1.getIntValue());
    l1.setValue(high);
    L(trem, 1);
    sleep(sleepTime);
    sem_wait(&bin_sem_t5);
    l1.setValue(low);
    l5_trem1.setValue(high);
    L(trem, 5);
    sleep(sleepTime);
    sem_post(&bin_sem_t5);
    l5_trem1.setValue(low);
    l1.setValue(high);
    L(trem, 8);
    sleep(sleepTime);
    l1.setValue(low);
    l1.setValue(high);
    L(trem, 4);
    sleep(sleepTime);
    l1.setValue(low);
  }
  pthread_exit(0);
}

void *trem2(void *arg)
{
  int trem = 2;
  while (true)
  {
    int sleepTime = normalizeAdc(trem, vel_trem2.getIntValue());
    l9.setValue(high);
    L(trem, 9);
    sleep(sleepTime);
    sem_wait(&bin_sem_t5);
    l9.setValue(low);
    l5_trem2.setValue(high);
    L(trem, 5);
    sleep(sleepTime);
    sem_post(&bin_sem_t5);
    l5_trem2.setValue(low);
    l2.setValue(high);
    L(trem, 2);
    sleep(sleepTime);
    sem_wait(&bin_sem_t6);
    l2.setValue(low);
    l6_trem2.setValue(high);
    L(trem, 6);
    sleep(sleepTime);
    sem_post(&bin_sem_t6);
    l6_trem2.setValue(low);
  }
  pthread_exit(0);
}

void *trem3(void *arg)
{
  int trem = 3;
  while (true)
  {
    int sleepTime = normalizeAdc(trem, vel_trem3.getIntValue());
    l3.setValue(high);
    L(trem, 10);
    sleep(sleepTime);
    sem_wait(&bin_sem_t6);
    l3.setValue(low);
    l6_trem3.setValue(high);
    L(trem, 6);
    sleep(sleepTime);
    sem_post(&bin_sem_t6);
    l6_trem3.setValue(low);
    l3.setValue(high);
    L(trem, 3);
    sleep(sleepTime);
    l3.setValue(low);
    l3.setValue(high);
    L(trem, 7);
    sleep(sleepTime);
    l3.setValue(low);
  }
  pthread_exit(0);
}

int main()
{

  int res;
  pthread_t thread1, thread2, thread3;
  // ----------- Testando LEDS ----------
  l1.setValue(low);       //3
  l5_trem1.setValue(low); //4
  l5_trem2.setValue(low); //21
  l2.setValue(low);       //22
  l6_trem2.setValue(low); //23
  l9.setValue(low);       //24
  l6_trem3.setValue(low); //43
  l3.setValue(low);       //44

  sleep(2);

  l1.setValue(high);       //3
  l5_trem1.setValue(high); //4
  l5_trem2.setValue(high); //21
  l2.setValue(high);       //22
  l6_trem2.setValue(high); //23
  l9.setValue(high);       //24
  l6_trem3.setValue(high); //43
  l3.setValue(high);       //44

  void *thread_result;

  // ------ criando multex t5 ------
  res = sem_init(&bin_sem_t5, 0, 2);
  if (res != 0)
  {
    perror("Semaphore t5 initialization failed");
    exit(EXIT_FAILURE);
  }

  // ------ criando multex t6 ------
  res = sem_init(&bin_sem_t6, 0, 2);
  if (res != 0)
  {
    perror("Semaphore t6 initialization failed");
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
  sem_destroy(&bin_sem_t5);
  sem_destroy(&bin_sem_t6);
  exit(EXIT_SUCCESS);
}
