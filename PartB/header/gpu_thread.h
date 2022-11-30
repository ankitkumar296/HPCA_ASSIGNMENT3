#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <time.h>
#include<cuda_runtime.h>

#define N 512

_global_ void matrixMul(const int *a, const int *b, int *c){
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;
  int rowA=row;
  int colB=col;

 if (((row %2)==0) && (row<N) && (col+1<N))
 {
   int sum=0;
     for(int iter = 0; iter < N; iter++) 
      {
        sum += a[rowA * N + iter] * b[iter * N + colB];
        sum += a[(rowA+1) * N + iter] * b[iter * N + colB];
        sum += a[rowA * N + iter] * b[iter * N + (colB+1)];
        sum += a[(rowA+1) * N + iter] * b[iter * N + (colB+1)];
      }
      int rowC = rowA>>1;
      int colC = colB>>1;
     //  int indexC = rowC * (N>>1) + colC;
     int indexC = rowC * (N>>1) + colC;
      c[indexC] = sum;
 }
//c[row * N+col]=0;
//for(int k=0;k<N;k++)
//{
//      c[row * N + col] += a[row * N + k] * b[k * N + col];
//}
}


int main() {

  int i;
  int total = N*N;
  int o_total= N*N/4;
  int h_a[total];
  int h_b[total];
  int h_c[o_total];
  int *dev_a, *dev_b, *dev_c;


  // Initialize matrices

  srand(0);
  for(i=0;i<N*N;i++)
  {
    h_a[i]=rand()%100;
    h_b[i]=rand()%100;
  }

  // Allocate device memory
  cudaError_t result;
  result = cudaMalloc((void**)&dev_a, total*sizeof(int));
  if(result!=cudaSuccess){
  printf("Error occured in allocating device memory\n");
  }
  result = cudaMalloc((void**)&dev_b, total*sizeof(int));
  if(result!=cudaSuccess){
  printf("Error occured in allocating device memory\n");
  }
  result = cudaMalloc((void**)&dev_c, o_total*sizeof(int));
  if(result!=cudaSuccess){
  printf("Error occured in allocating device memory\n");
  }

  // Copy data to the device
  result=cudaMemcpy(dev_a, h_a, total*sizeof(int), cudaMemcpyHostToDevice);
  if(result!=cudaSuccess){
  printf("Error occured in copying data from host to device\n");
  }
  result = cudaMemcpy(dev_b, h_b, total*sizeof(int), cudaMemcpyHostToDevice);
   if(result!=cudaSuccess){
  printf("Error occured in copying data from host to device\n");
  }
  // cudaMemcpy(dev_c, h_c, N/2*N/2*sizeof(int), cudaMemcpyHostToDevice);


  // Threads per CTA dimension
  int THREADS = 32;

  // Blocks per grid dimension (assumes THREADS divides N evenly)
  int BLOCKS = N / THREADS;

  // // Use dim3 structs for block  and grid dimensions
  dim3 threads(THREADS, THREADS);
  dim3 blocks(BLOCKS, BLOCKS);

  // Launch kernel
  
  printf("Calling matrixmul\n);");
  matrixMul<<<blocks, threads>>>(dev_a,dev_b,dev_c);



  // Copy back to the host
  
  result = cudaMemcpy(h_c, dev_c, o_total*sizeof(int),cudaMemcpyDeviceToHost);
  if(result!=cudaSuccess){
  printf("Error occured in copying data from device to host\n");
  }

  printf("COMPLETED SUCCESSFULLY\n");

  // Free memory on device
  cudaFree(dev_a);
  cudaFree(dev_b);
  cudaFree(dev_c);

 for(i=0;i<N/2*N/2;i++)
 {
   printf("%d\t",h_c[i]);
 }
  

  return 0;
}