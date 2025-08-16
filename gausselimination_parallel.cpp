#include <iostream>
#include <cmath>
#include <omp.h>
#include<fstream>
using namespace std;

#define N 50 // number of unknowns
double A[N][N], b[N], x[N]; // coefficient matrix, constant vector, solution vector

int main() {
   // initialize the coefficient matrix and constant vector
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         A[i][j] = 1.0 / (i + j + 1);
      }
      b[i] = 1.0 / (i + 1);
   }

   // solve the system of linear equations using OpenMP
   double start_time = omp_get_wtime(); // start the timer
 
int pivot_row, factor;
#pragma omp parallel for shared(A, b, x) private(pivot_row, factor)
for (int k = 0; k < N; k++) {
   // partial pivoting
   pivot_row = k;
   double pivot_value = A[k][k];
   #pragma omp for reduction(min:pivot_value)
   for (int i = k + 1; i < N; i++) {
      if (fabs(A[i][k]) > fabs(pivot_value)) {
         pivot_row = i;
         pivot_value = A[i][k];
      }
   }
   if (pivot_row != k) {
      for (int j = k; j < N; j++) {
         swap(A[k][j], A[pivot_row][j]);
      }
      swap(b[k], b[pivot_row]);
   }
   // elimination
   for (int i = k + 1; i < N; i++) {
      factor = A[i][k] / A[k][k];
      for (int j = k + 1; j < N; j++) {
         A[i][j] -= factor * A[k][j];
      }
      b[i] -= factor * b[k];
   }
}
// back substitution
#pragma omp for
for (int i = N - 1; i >= 0; i--) {
   x[i] = b[i];
   for (int j = i + 1; j < N; j++) {
      x[i] -= A[i][j] * x[j];
   }
   x[i] /= A[i][i];
}

   ofstream file;
   file.open("output3");
   for(int i=0;i<N;i++){
		file << x[i] << " ";
	}
	file.close();
   double end_time = omp_get_wtime(); // stop the timer
   cout << "Elapsed time: " << end_time - start_time << " seconds" << endl;
   return 0;
}

