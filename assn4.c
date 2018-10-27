#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

//count is critical code
//buffer
//next-read
//next-write

#define MAX_BUFFER_SIZE (20)

struct shared_num{
	int num[MAX_BUFFER_SIZE];
	sem_t items;
	sem_t empty;
};

void * prod_func(void *p){
	struct shared_num * psn = (struct shared_num *)p;
	int write_next = 0;
	int num;
	while(1 == scanf("%d", &num)){
		sem_wait(&(psn->empty));

		psn->num[write_next] = num;
		write_next++;

		if(write_next >= 20) write_next = 0;

		sem_post(&(psn->items));
	}
	sem_wait(&(psn->empty));
	psn->num[write_next] = -1;
	sem_post(&(psn->items));

	return NULL;
}

//cons
void * cons_func(void *p){

	struct shared_num * psn = (struct shared_num *)p;
	int read_next = 0;
	int sum = 0;
	for(;;){
		sem_wait(&(psn->items));
		if(psn->num[read_next] < 0) break;
		for(int i = 0; i < 3; i++){
			printf("%d", psn->num[read_next + i]);
			sum += psn->num[read_next + i];
			read_next++;
			printf("The product of %d, %d, and %d is %d\n",psn->num[read_next],psn->num[read_next +1], psn->num[read_next + 2],sum);
		}
		if(read_next >= 20) read_next = 0;
		sem_post(&(psn->empty));
	}
	return NULL;
}

int main(){
	struct shared_num * psn = malloc(sizeof(struct shared_num));
	pthread_t th1, th2;
	sem_init(&(psn->items), 0, 0);
	sem_init(&(psn->empty), 0, MAX_BUFFER_SIZE);

	pthread_create(&th1, NULL, prod_func, psn);
	pthread_create(&th2, NULL, cons_func,psn);

	pthread_join(th1,NULL);
	pthread_join(th2,NULL);

	return 0;
}
