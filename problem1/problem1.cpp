#include<pthread.h>
#include<unistd.h>
#include<iostream>

using namespace std;

#define RANGE 300
void *myThreadFunction1(void *thId);
void *myThreadFunction2(void *thId);

pthread_mutex_t lock;
pthread_cond_t condition;

int count = 1;

int main() {
	int id1 = 1, id2 = 2;
	int *ptrId1 = &id1, *ptrId2 = &id2;

	if(pthread_mutex_init(&lock,NULL)!=0){
		cout<<"Mutex failed"<<endl;
		return 1;
	}
//	if (pthread_mutex_init(&lock, NULL) != 0){
//		cout << "Mutex init failed"<< endl;
//		return 1;
//	}

	pthread_t thread1, thread2;


	int iret1 = pthread_create(&thread1, NULL, myThreadFunction1,(void*) ptrId1);
	int iret2 = pthread_create(&thread2, NULL, myThreadFunction2,(void*) ptrId2);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&lock);

	return 0;
} //end main()

void *myThreadFunction1(void *ptr) {
	pthread_mutex_lock(&lock);

	int *number_ptr;
	number_ptr = (int*) ptr;
	int id = *(number_ptr);

	while(count <= RANGE){
		while((count%2 != 0)){

			pthread_cond_wait(&condition, &lock);
		}

		cout << "Thread-" << id << "\t" << count << endl;
		count++;
	}

	pthread_mutex_unlock(&lock);
}

void *myThreadFunction2(void *ptr) {
	pthread_mutex_lock(&lock);

	int *number_ptr;
	number_ptr = (int*) ptr;
	int id = *(number_ptr);

	while(count <= RANGE){
		while((count%2 != 0)){
			cout <<"Thread-" << id << "\t" << count << endl;
			count++;

			pthread_cond_signal(&condition);
			pthread_mutex_unlock(&lock);
		}
	}
}
