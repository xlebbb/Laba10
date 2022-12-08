#include <stdio.h>
#include <pthread.h>
#include <time.h>

int value;
pthread_cond_t cond;
pthread_mutex_t mutex;
int flag;

void initialize(){
	value = 0;
	flag = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
}

void *produce(void *arg){
	srand(time(NULL));
	while(1){
		int tmp = (rand() % 10) + 1;
		pthread_mutex_lock(&mutex);
		value = tmp;
		printf("Produce: %d\n", value);
		flag = 1;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		sleep(2);	
	}
}

void *consume(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		while(!flag){
			pthread_cond_wait(&cond, &mutex);
		}
		pthread_mutex_unlock(&mutex);
		printf("Read: %d\n", value);
		value = 0;
		flag = 0;
	}
}

int main(int argc, char **arcv){
	pthread_t producer, consumer;

    if (pthread_create(&producer, NULL, &produce, NULL) != 0) {
       	fprintf(stderr, "Error (thread)\n");
        return 1;
    }
  	
    if (pthread_create(&consumer, NULL, &consume, NULL) != 0) {
        fprintf(stderr, "Error (thread)\n");
        return 1;
      }
  	
  	sleep(10);

	pthread_cancel(producer);
	if (pthread_join(producer, NULL) != 0) {
	    printf("Error\n");   
	}

  	pthread_cancel(consumer);
    if (pthread_join(consumer, NULL) != 0) {
      	printf("Error\n");   
    }
  	
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
	return 0;
}
