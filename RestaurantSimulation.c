#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

sem_t customerSem;
sem_t machineSem;


int noOfMachines;
int occupiedMachines=0;
int capacity;

void displayCustomerState(int state);
void displayCookState(int state);
void displayMachineState(int state);
bool isMachineFull();

void *customer(void* arg) {
	sem_wait(&customerSem);
	
	int *argptr = (int *) arg;
	int c = (int)argptr;
	//if(c != 1 && c % capacity == 1) {
	if(c > capacity) {
	printf("Restaurant Capacity full. Please wait\n");	
	sleep(5);
	}	

	printf("\nDetails of Customer %d:", c);
	for(int i=1;i<=5;i++) {	
	
		displayCustomerState(i);
	}
	//sleep(5);
	sem_post(&customerSem);
}

void *cook(void* arg) {
	
	int *argptr = (int *) arg;
	int c = (int)argptr;	
	
	printf("\nDetails of Cook %d:", c);
	for(int i=1;i<=5;i++) {	
	
		displayCookState(i);
	}

}

void *food(void *arg)	{
	int *argptr = (int *) arg;
	int c = (int)argptr;
	switch(c) {
		case 1:
			printf("Pizza\n");
			break;
		case 2:
			printf("Burger\n");
			break;
		case 3:
			printf("Sandwich\n");
			break;
		default:
			break;
	}
}

void *machine(void* arg) {
	
	sem_wait(&machineSem);
	int *argptr = (int *) arg;
	int c = (int)argptr;	
	pthread_t tid;
	printf("\nDetails of Machine %d:", c);
	printf("\nMachine Cooked ");
	pthread_create(&tid, NULL, &food, c);
	pthread_join(tid, NULL);
	for(int i=1;i<=3;i++) {	
	
		displayMachineState(i);
	}
	sem_post(&machineSem);

}

int main() {

	int noOfCooks;
	int customers;
	char restaurantState[20] = "Open";
	
	printf("Restaurant State: %s\n", restaurantState);
	printf("Enter the customer capacity of the restaurant: ");
	scanf("%d",&capacity);
	printf("Enter the number of customers: ");
	scanf("%d",&customers);
	printf("Enter the number of machines available in the restaurant: ");
	scanf("%d",&noOfMachines);	
	printf("Enter the number of cooks: ");
	scanf("%d",&noOfCooks);

	sem_init(&customerSem, 0, capacity);
	sem_init(&machineSem, 0, noOfCooks);


	pthread_t tid[customers];
	pthread_t tid2[noOfCooks];
	pthread_t tid3[noOfMachines];

	for(int i=0;i<customers;i++){
		pthread_create(&tid[i], NULL, &customer, (i+1));
		sleep(1);
		if(i<noOfCooks) {
		pthread_create(&tid2[i], NULL, &cook, (i+1));
		sleep(1);
		}
		if(i<noOfMachines) {
		pthread_create(&tid3[i], NULL, &machine, (i+1));
		sleep(1);
		}
	}


	for(int i=0;i<customers;i++){
		pthread_join(tid[i], NULL);

	}
	for(int i=0;i<noOfCooks;i++){
		pthread_join(tid2[i], NULL);
	}
	for(int i=0;i<noOfMachines;i++){
		pthread_join(tid3[i], NULL);
	}

	//pthread_exit(NULL);
	sem_destroy(&customerSem);

	char restaurantStat[20] = "Closed";
	printf("Restaurant State: %s\n", restaurantStat);

	return 0;	
}

void displayCustomerState(int state) {
	switch(state) {
		case 1:
			printf("\nCustomer Starting\n");
			break;
		case 2:
			printf("Customer Entered\n");
			break;
		case 3:
			printf("Customer Placed an Order\n");
			break;
		case 4:
			printf("Customer Received an Order\n");
			break;
		case 5:
			printf("Customer Leaving\n");
			break;
		default:
			break;
	}
} 

void displayCookState(int state) {
	switch(state) {
		case 1:
			printf("\nCook Starting\n");
			break;
		case 2:
			printf("Cook Received Order\n");
			break;
		case 3:
			printf("Cook Started Food\n");
			break;
		case 4:
			printf("Cook Finished Food\n");
			break;
		case 5:
			printf("Cook Leaving\n");
			break;
		default:
			break;
	}
} 

void displayMachineState(int state) {
	switch(state) {
		case 1:
			printf("Machine Starting\n");
			occupiedMachines++;
			break;
		case 2:
			printf("Machine Starting Food\n");
			break;
		case 3:
			printf("Machine Done Food\n");
			break;
		case 4:
			printf("Machine Ending\n");
			occupiedMachines--;
			break;
		default:
			break;
	}
} 


bool isMachineFull() {
	if(noOfMachines == occupiedMachines)
		return true;
	return false;
}
