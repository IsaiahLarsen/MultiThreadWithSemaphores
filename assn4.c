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
	sem_t items;
	sem_t empty;
	int count;

};
struct nums{
	int numOne;
	int numTwo;
	int numThree;
};

struct nums buffer[MAX_BUFFER_SIZE];


//cons
void * cons_func(void *p){

	struct shared_num * psn = (struct shared_num *)p;
	int read_next = 0;
	int sum = 1;
	int numCalc = 0;
	for(;;){
		sem_wait(&(psn->items));
		if(0 == psn->count) break;
		sum = buffer[read_next].numOne * buffer[read_next].numTwo * buffer[read_next].numThree;
		printf("The product of %d, %d, and %d is %d\n", buffer[read_next].numOne,buffer[read_next].numTwo,buffer[read_next].numThree,sum);
		sem_post(&(psn->empty));
		read_next++;
		numCalc++;

		if(read_next >= 20) read_next = 0;
	}
	printf("Number of Calculations: %d\n",numCalc);
	return NULL;
}

int main(int argc, char *argv[]){
	struct shared_num * psn = malloc(sizeof(struct shared_num));
	pthread_t th1;
	sem_init(&(psn->items), 0, 0);
	sem_init(&(psn->empty), 0, MAX_BUFFER_SIZE);
	int write_next = 0;
	printf("Assignment 4 by Isaiah Larsen\n");
	struct nums numbers;

	pthread_create(&th1, NULL, cons_func,psn);

//	if(argc < 1){
		//Start reading nums
		while(3 == scanf("%d%d%d",&numbers.numOne,&numbers.numTwo,&numbers.numThree)){
			sem_wait(&(psn->empty));
			if(write_next >= 20) write_next = 0;
			buffer[write_next] = numbers;
			psn->count++;
			sem_post(&(psn->items));
			write_next++;
		}
		sem_wait(&(psn->empty));
		psn->count = 0;
		sem_post(&(psn->items));
//	}

	pthread_join(th1,NULL);

	return 0;
}
