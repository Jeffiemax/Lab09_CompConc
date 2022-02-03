#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <semaphore.h>

#define NTHREADS  5

/* Variaveis globais */
int x = 3;  //Variáveis compartilhadas pelas Threads
sem_t condt1,condt2;//condt3; //Semaforos para sincronizar a ordem de execução das Threads

/* Threads que incrementam x */
void *A (void *t) {
    int *my_id = (int*)t;
    //printf("A: Comecei: thread %d\n", *my_id);
    sem_wait(&condt2); //frase a ser exibida
    printf("Volte sempre\n");
    free(my_id); 
    pthread_exit(NULL);
}

void *B (void *t) {
    int *my_id = (int*)t;
    //printf("B: Comecei: thread %d\n", *my_id);
    sem_wait(&condt1); //entrada na secao critica
    printf("Fique a vontade\n"); //frase a ser exibida
    x--; //secao critica 
    if(x == 0)sem_post(&condt2); //teste para liberar a thread final
    else sem_post(&condt1); //teste para liberar as threads intemediárias 
    free(my_id);
    //printf("E: thread %d terminou \n", *my_id);
    pthread_exit(NULL);
}

void *C (void *t) {

    int *my_id = (int*)t;
    //printf("C: Comecei: thread %d\n", *my_id);
    sem_wait(&condt1); //entrada na secao critica
    printf("Sente-se por favor\n"); //frase a ser exibida
    x--; //secao critica 
    if(x == 0)sem_post(&condt2); //teste para liberar a thread final
    else sem_post(&condt1); //teste para liberar as threads intemediárias 
    free(my_id);
    //printf("E: thread %d terminou \n", *my_id);
    pthread_exit(NULL);
}

void *D (void *t) {

    int *my_id = (int*)t;
    //printf("D: Comecei: thread %d\n", *my_id);
    sem_wait(&condt1); //espera ganharcpu para executar executar
    printf("Aceita um copo d'água\n"); //frase a ser exibida
    x--; //secao critica 
    if(x == 0)sem_post(&condt2); //teste para liberar a thread final
    else sem_post(&condt1); //teste para liberar as threads intemediárias 
    free(my_id);
    //printf("E: thread %d terminou \n", *my_id);
    pthread_exit(NULL);
}

void *E (void *t) {

    int *my_id = (int*)t;
    //printf("E: Comecei: thread %d\n", *my_id);
    printf("Seja bem-vindo!\n"); //frase a ser exibida
    sem_post(&condt1); //permite que as Threads intermediárias executem
    free(my_id); 
    //printf("E: thread %d terminou \n", *my_id);
    pthread_exit(NULL);

}

/* Funcao principal */
int main(int argc, char *argv[]) {
    int *t1, *t2, *t3, *t4, *t5;
    pthread_t threads[NTHREADS];

  //aloca espaco para os identificadores das threads
    t1 = malloc(sizeof(int));
    t2 = malloc(sizeof(int));
    t3 = malloc(sizeof(int));
    t4 = malloc(sizeof(int));
    t5 = malloc(sizeof(int));
    *t1=1, *t2=2, *t3=3, *t4=4, *t5=5; 

    //inicia os semaforos
    sem_init(&condt1, 0, 0);
    sem_init(&condt2, 0, 0);
    //sem_init(&condt3, 0, 3);

    /* Cria as threads */
    pthread_create(&threads[0], NULL, A, (void*)t1);
    pthread_create(&threads[1], NULL, B, (void*)t2);
    pthread_create(&threads[2], NULL, C, (void*)t3);
    pthread_create(&threads[3], NULL, D, (void*)t4);
    pthread_create(&threads[4], NULL, E, (void*)t5);
 

    /* Espera todas as threads completarem */
    for (int i = 0; i < NTHREADS; i++) {
        if(pthread_join(threads[i], NULL)){
            printf("--ERRO: pthread_join() \n");exit(-1);
        }
    }
    printf ("FIM.\n");
  
}