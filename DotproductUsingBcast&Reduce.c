#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"



//Name : Laura Youssef Kamal
//ID   : 20150197
//Group: cs_5
//Lab Time : sunday 3/11/2018
//Task : get the dot product of each row in matrix using Bcast and Reduce

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
	
	int i;
	int Portion;
	int Rows , Cols;
	int VectorSize;
	int sizeOfArray;
	
	int* myArray;
	int* myVector;
	
	if (my_rank == 0)
	{
		printf("%s \n", "			**Hello from master process**			");
		printf("%s %d \n", "Number of slaves is ", p);
		printf("%s", "Enter Number Of Rows : ");
		scanf("%d", &Rows);
		printf("%s", "Enter Number Of Cols : ");
		scanf("%d", &Cols);
		printf("%s \n", " ");
		
		sizeOfArray = Cols* Rows;
		myArray = (int*)malloc(sizeOfArray * sizeof(int));
		myVector = (int*)malloc(Cols * sizeof(int));
		
		printf("%s", "Enter elements of Array: ");
		
		for (i = 0; i < sizeOfArray; i++)
		{
			scanf("%d", &myArray[i]);
		}
		
		printf("%s", "Enter elements of Vector: "); 
		for (i = 0; i < Cols; i++)
		{
			scanf("%d", &myVector[i]);
		}
		
		Portion = sizeOfArray / (p-1);
		
		
		for(i=0 ; i<p-1 ;i++)
		{
			MPI_Send( myArray+(i*Portion),Portion , MPI_INT, i+1, tag, MPI_COMM_WORLD);
		}
		
	}

	MPI_Bcast(&Portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (my_rank != 0){
		myArray = (int*)malloc(Portion * sizeof(int));
		myVector = (int*)malloc(Cols * sizeof(int));
	}

	MPI_Bcast(myVector, Cols, MPI_INT, 0, MPI_COMM_WORLD);
	
	int dotProduct=0;
	int result;
	int Max;
	if(my_rank != 0)
	{
		MPI_Recv(myArray, Portion, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		for (i=0 ;i<Portion ;i++)
		{
			printf(" %d %s", myArray[i] , " ");
		}
		printf(" %s \n ", "");
		
		int j;
		Max=0;
		for(j=0 ;j<Rows/(p-1) ;j++)
		{
			dotProduct=0;
			for (i=0 ;i<Cols ;i++ )
			{
				dotProduct += myArray[i+(Cols*j)] * myVector[i];
				
			}
			if(dotProduct > Max)
			{
				Max=dotProduct;
			}
		}
	
	}
	MPI_Reduce(&Max, &result, 1, MPI_INT, MPI_MAX, 0,MPI_COMM_WORLD);
	
	if (my_rank == 0)
	{
		printf("dot product = %d \n", result);	
	}
	
	

	free(myArray);
	free(myVector);
	
	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}
