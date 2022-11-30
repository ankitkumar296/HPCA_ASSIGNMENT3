#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>



struct parameters{
  int *A_matrix;
  int *B_matrix;
  int *result_matrix;
  int row_start;
  int row_end;
  int SIZE;
};

void *worker( void *arguments )
{
  struct parameters *args=(struct parameters *) arguments;
  int *A_matrix =args->A_matrix;
  int *B_matrix =args->B_matrix;
  int *result_matrix=args->result_matrix;
  int row_start=args->row_start;
  int row_end=args->row_end;
  int SIZE=args->SIZE;
  

    for (int j = row_start; j < row_end; j+=2)
        {   
            int column1[SIZE];
            int column2[SIZE];
            for (int k=0; k<SIZE; k++){
                column1[k] = B_matrix[(k*SIZE) + j];
                column2[k] = B_matrix[(k*SIZE) + j+1];       
            } 
            for(int pt=0; pt<SIZE; pt+=2){
                int temp_sum = 0;
                for (int k=0; k<SIZE; k+=8){
                    temp_sum += A_matrix[(pt*SIZE)+k] * column1[k];
                    temp_sum += A_matrix[(pt*SIZE)+k] * column2[k]; 
                    temp_sum += A_matrix[(pt+1)*(SIZE)+k] * column1[k];
                    temp_sum += A_matrix[(pt+1)*(SIZE)+k] * column2[k]; 
                }
            int r = pt>>1;
            int s = j>>1;
            result_matrix[(r*(SIZE>>1))+s] = temp_sum; 
        }  
    }

}

void multiThread(int SIZE, int *matA, int *matB, int *output)
{

  int NumberOfThreads=8;
  int rowsPerThread= SIZE/NumberOfThreads;
  pthread_t * threads;

  threads = (pthread_t *) malloc( NumberOfThreads * sizeof(pthread_t) );
struct parameters *args=(struct parameters*) malloc(NumberOfThreads*sizeof(pthread_t));
 
 int j=0;
 
for (int pt = 0; pt < NumberOfThreads; ++pt ) {
  args[pt].A_matrix=matA;
  args[pt].B_matrix=matB;
  args[pt].result_matrix=output;
  args[pt].row_start=j;
  args[pt].row_end=j+rowsPerThread-1;
  args[pt].SIZE=SIZE;
  j += rowsPerThread;
  
  }
  for (int pt=0;pt<NumberOfThreads;pt++){
    pthread_create( &threads[pt], NULL, worker, (void *)&args[i] );
  }

  for (int pt = 0; pt < NumberOfThreads; ++pt ) {
    pthread_join( threads[pt], NULL );
  }
    
}

