#include <stdio.h>
#include <string.h>
#include "mpi.h"

//Name : Laura Youssef Kamal
//ID   : 20150197
//Group: cs_5
//Lab Time : Sunday 6/5/2018
//Task : Scaling Matrix

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
	
	int i , j;
	int Portion;
	int Number;
	int Rows , Cols;
	int sizeOfArray;
	
	int* Matrix;
	int* subMatrix;
	int *result;

	
	if (my_rank == 0)
	{
		printf("%s \n", "			** Matrix Scaling **			");
		
		printf("%s", "Enter Number Of Rows OF Matrix : ");
		scanf("%d", &Rows);
		printf("%s", "Enter Number Of Cols OF Matrix : ");
		scanf("%d", &Cols);
		printf("%s", "Enter The Number : ");
		scanf("%d", &Number);
		printf("%s \n", " ");
		
		sizeOfArray = Cols* Rows;
		Matrix = (int*)malloc(sizeOfArray * sizeof(int));
		
		printf("%s", "Enter elements of Matrix: ");
		
		for (i = 0; i < sizeOfArray; i++)
		{
			scanf("%d", &Matrix[i]);
		}

		
		Portion = sizeOfArray / p;
		
		result = malloc(((p)*Portion) * sizeof(int));

		for(i=1 ; i<p ;i++)
		{
			MPI_Send( Matrix+(i*Portion),Portion , MPI_INT, i, tag, MPI_COMM_WORLD);
		}
		
	}

	MPI_Bcast(&Portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Number, 1, MPI_INT, 0, MPI_COMM_WORLD);

	

	subMatrix = (int*)malloc(Portion * sizeof(int));


	if(my_rank != 0)
	{
		MPI_Recv(subMatrix, Portion, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
	}
	
	if (my_rank == 0 )
	{
		for (i=0 ;i<Portion ;i++)
		{
			subMatrix[i] = Matrix[i];
		}
	}
	
	for (i=0 ;i<Portion ;i++)
	{
		subMatrix[i] *= Number;
	}	
	
	
    MPI_Gather(subMatrix,Portion, MPI_INT, result, Portion, MPI_INT, 0, MPI_COMM_WORLD);

	if (my_rank == 0)
    {
		printf(" Before Scaling \n");

		for (i = 0; i < Rows; i++) 
		{	
			for (j = 0 ; j < Cols ; j++)
			{
			   printf("%d  ", Matrix[j + (i*Cols)]);
			}
			printf("\n");
        }
		
		printf(" After Scaling \n");

       for (i = 0; i < Rows; i++) 
		{	
			for (j = 0 ; j < Cols ; j++)
			{
			   printf("%d  ", result[j + (i*Cols)]);
			}
			printf("\n");
        }
		
    }
	
	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}
