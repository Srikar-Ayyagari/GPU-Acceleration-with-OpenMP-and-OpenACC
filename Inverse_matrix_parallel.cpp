#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
using namespace std;

const int N = 1000; // size of the matrix
double A[N][N], b[N], x[N], A_inv[N][N]; // matrix A, vector b, solution vector x, and inverse matrix A_inv

int main() {
   // initialize the matrix A and vector b with random values
   srand(time(NULL));
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         A[i][j] = rand() % 100 + 1; // random values between 1 and 100
      }
      b[i] = rand() % 100 + 1;
   }

   // create the augmented matrix [A|b]
   double aug[N][2*N];
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         aug[i][j] = A[i][j];
      }
      aug[i][N+i] = b[i];
   }

   // perform Gauss-Jordan elimination in parallel
   #pragma omp parallel for
   for (int k = 0; k < N; k++) {
      // partial pivoting
      int pivot_row = k;
      double pivot_value = aug[k][k];
      for (int i = k + 1; i < N; i++) {
         if (fabs(aug[i][k]) > fabs(pivot_value)) {
            pivot_row = i;
            pivot_value = aug[i][k];
         }
      }
      if (pivot_row != k) {
         for (int j = k; j < 2*N; j++) {
            swap(aug[k][j], aug[pivot_row][j]);
         }
      }
      // elimination
      #pragma omp parallel for
      for (int i = 0; i < N; i++) {
         if (i != k) {
            double factor = aug[i][k] / aug[k][k];
            for (int j = k; j < 2*N; j++) {
               aug[i][j] -= factor * aug[k][j];
            }
         }
      }
   }

   // extract the inverse matrix from the augmented matrix
   #pragma omp parallel for
   for (int i = 0; i < N; i++) {
      for (int j = N; j < 2*N; j++) {
         A_inv[i][j-N] = aug[i][j] / aug[i][i];
      }
   }

   // solve the system of linear equations Ax = b using the inverse matrix
   #pragma omp parallel for
   for (int i = 0; i < N; i++) {
      x[i] = 0.0;
      for (int j = 0; j < N; j++) {
         x[i] += A_inv[i][j] * b[j];
      }
   }

   // print the original matrix A, vector b, solution vector x, and inverse matrix A_inv
   cout << "Matrix A:" << endl;
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         cout << A[i][j] << " ";
      }
      cout

