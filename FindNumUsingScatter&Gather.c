#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"



//Name : Laura Youssef Kamal
//ID   : 20150197
//Group: cs_5
//Lab Time :  2/4/2018
//Task : find number in matrix user scatter and gather 

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
	
	int i , r ,c;
	int Portion;
	int Rows , Cols , Number;
	int sizeOfArray;
	int index;
	
	int* result; int* resultR; int* resultC;
	int* myArray;
	int* subArray;

	
	if (my_rank == 0)
	{
		printf("%s \n", "			**Hello from master process**			");
		printf("%s %d \n", "Number of slaves is ", p);
		printf("%s", "Enter Number Of Rows : ");
		scanf("%d", &Rows);
		printf("%s", "Enter Number Of Cols : ");
		scanf("%d", &Cols);
		printf("%s", "Enter The Number : ");
		scanf("%d", &Number);
		printf("%s \n", " ");
		
		sizeOfArray = Cols* Rows;
		myArray = (int*)malloc(sizeOfArray * sizeof(int));
		result = (int*) malloc (p * sizeof(int));
		resultR = (int*) malloc (p * sizeof(int));
		resultC = (int*) malloc (p * sizeof(int));
		
		printf("%s", "Enter Elements Of Array : ");
		
		for (i = 0; i < sizeOfArray; i++)
		{
			scanf("%d", &myArray[i]);
		}
		
		Portion = sizeOfArray / p; 
		
	}

	MPI_Bcast(&Portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Rows, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// sending the Number 
	
	MPI_Bcast(&Number, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	subArray = (int*)malloc(Portion * sizeof(int));
	
	MPI_Scatter(myArray, Portion, MPI_INT, subArray, Portion, MPI_INT, 0, MPI_COMM_WORLD);
	
	// searching for the number

	int j;
	index = -1;
	for(j = 0 ;j< (Rows/p) ; j++)
	{
		for (i = 0 ; i < Cols ; i++ )
		{
			if(index == -1 && subArray[i+(Cols*j)] == Number){
				// this if we need index in 1D array
				index = i+(Cols*j)+(Portion*my_rank);
				
				
				// if we need the index in 2D array
				r = (j)+((Portion/Cols)*my_rank);
				c = i;
				printf("%d %d ",r , c);
				
				j = (Rows/p);
				break;
			}	
		}
	}
	
	// this if we need index in 1D array
	MPI_Gather(&index, 1, MPI_INT, result, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	
    // if we need the index in 2D array
	//MPI_Gather(&r, 1, MPI_INT, resultR, 1, MPI_INT, 0, MPI_COMM_WORLD);
	//MPI_Gather(&c, 1, MPI_INT, resultC, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
    if (my_rank == 0)
    {
        int i;
        for (i = 0; i < p; i++) {
			if(result[i] != -1){
				printf("Index = %d \n ",  result[i]);
			}         
        }
    }

	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}
