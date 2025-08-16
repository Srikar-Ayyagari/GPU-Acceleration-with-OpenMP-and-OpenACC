#include<iostream>
#include<stdio.h>
#include<omp.h>
using namespace std;
#define SIZE 1200
#define num_threads 2
int main(){
    static int mat1[SIZE][SIZE];
    static int mat2[SIZE][SIZE];
    double start = omp_get_wtime();
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            mat1[i][j] = i+j;
            mat2[i][j] = i-j;
        }
    }
    static int mat3[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            mat3[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                mat3[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    double end = omp_get_wtime();
    double time1 = end-start;
    cout << "Time taken is " << time1 << endl;

    omp_set_num_threads(num_threads);
    static int mat4[SIZE][SIZE];
    static int mat5[SIZE][SIZE];
    static int mat6[SIZE][SIZE];
    start = omp_get_wtime();
    #pragma omp parallel
    {
	#pragma omp single nowait
		cout << "The number of threads are : " << omp_get_num_threads() << endl;
    #pragma omp for
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                mat4[i][j] = i+j;
                mat5[i][j] = i-j;
            }
        }
    #pragma omp barrier
    #pragma omp for
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                mat6[i][j] = 0;
                for (int k = 0; k < SIZE; k++) {
                 mat6[i][j] += mat4[i][k] * mat5[k][j];
                }
            }
        }
    }
    end = omp_get_wtime();
    double time2 = end-start;
    cout << "Time taken is " << time2 << endl;
    cout << "The Speed up is :" << time1/time2 << endl;
}
