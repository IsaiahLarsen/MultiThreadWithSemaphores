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
void cons_func(void *p){

	struct shared_num * psn = (struct shared_num *)p;
	int read_next = 0;
	int sum = 1;
	int count = 0;
	while(psn->num[read_next] >= 0){
		for(int i = 0; i < 3; i++){
			if(psn->num[read_next] < 0) break;
			sum *= psn->num[read_next];
			read_next++;
		}
		count++;
		printf(" sum %d\n",sum);
		if(read_next >= 20) read_next = 0;
	}
	printf("Total num of calculations: %d",count);
}

int main(){
	struct shared_num * psn = malloc(sizeof(struct shared_num));
	pthread_t th1;
	sem_init(&(psn->items), 0, 0);
	sem_init(&(psn->empty), 0, MAX_BUFFER_SIZE);

	printf("Assignment 4 by Isaiah Larsen\n");

	pthread_create(&th1, NULL, prod_func, psn);

	pthread_join(th1,NULL);

	return 0;
}
