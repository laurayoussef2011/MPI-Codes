#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char * argv[])
{
	int my_rank;		/* rank of process	*/
	int p;			/* number of process	*/
	int source;		/* rank of sender	*/
	int dest;		/* rank of reciever	*/
	int tag = 0;		/* tag for messages	*/
	char message[100];	/* storage for message	*/
	MPI_Status status;	/* return status for 	*/
	/* recieve		*/

	/* Start up MPI */
	MPI_Init(&argc, &argv);

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	int sizeOfArray;
	int* myArray;
	if (my_rank == 0)
	{
		printf("%s \n", "Hello from master process.");
		printf("%s %d \n", "Number of slaves is ", p);
		printf("%s", "Enter Array size: ");
		scanf("%d", &sizeOfArray);
		printf("%s \n", " ");
		myArray = (int*)malloc(sizeOfArray * sizeof(int));
		printf("%s", "Enter Array of elements: ");
		int i;
		for (i = 0; i < sizeOfArray; i++)
		{
			scanf("%d", &myArray[i]);
		}
	}
	int result;
	//result = (int*)malloc(sizeOfArray * sizeof(int) / 5);
	//printf("Test");
	MPI_Bcast(&sizeOfArray, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(myArray, sizeOfArray, MPI_INT, 0, MPI_COMM_WORLD);

	if (my_rank != 0){
		myArray = (int*)malloc(sizeOfArray * sizeof(int));
	}
	
	//printf("after bcast in proc %d \n" , my_rank);

	int max;
	int portion = sizeOfArray / (p);
	int i, j;
	int idx = my_rank * portion;	
	max = myArray[idx];
	j = (my_rank * portion) + portion;
	for (i = (my_rank * portion) + 1; i < j; i++)
	{
		if (max < myArray[i])
		{
			max = myArray[i];
		}
	}

	MPI_Reduce(&max, &result, 1, MPI_INT, MPI_MAX, 0,MPI_COMM_WORLD);

	if (my_rank == 0)
	{
		printf("max = %d \n", result);	
	}
	free(myArray);
	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}
