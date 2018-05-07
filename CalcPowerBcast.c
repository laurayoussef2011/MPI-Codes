#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "mpi.h"

//Name : Laura Youssef Kamal
//Sunday 29/4/2018
//Task : find the power  using  Bcast and reduce

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
	int subProduct = 1;
	int result = 1;
	int base , power , portion;

	
	if (my_rank == 0)
	{
		printf("%s \n", "			** Finding The Power **			");
		
		printf("%s", "Enter Base Number : ");
		scanf("%d", &base);
		printf("%s", "Enter Power Number : ");
		scanf("%d", &power);
		printf("%s \n", " ");
		
		portion = power / (p-1);
	}
	
	MPI_Bcast(&base, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(my_rank != 0)
	{

		subProduct = 1;
		for (i = 0 ; i < portion ;i++)
		{
			subProduct *= base;
		}
	}
	
	MPI_Reduce(&subProduct, &result, 1, MPI_INT, MPI_PROD, 0,MPI_COMM_WORLD);
	
	if (my_rank == 0)
	{
		if(power % (p-1) != 0)
		{
			result *= base ;
		}
		printf("power ( %d , %d ) = %d \n",base , power , result);	
	}
	
	
	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}
