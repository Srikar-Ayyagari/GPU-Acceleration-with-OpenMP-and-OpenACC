#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<iostream>
#include<chrono>
#define ARRAY_SIZE 10000000
using namespace std;
int main () {
		cout << "Enter number of threads: " ;
		int num_threads;
		cin >> num_threads;
		int i, chunk_size;
		double dot_product = 0.0;
		double *a = (double*) malloc(ARRAY_SIZE * sizeof(double));
		double *b = (double*) malloc(ARRAY_SIZE * sizeof(double));
		for (i = 0; i < ARRAY_SIZE; i++) {
				a[i] = i * 0.0001;
				b[i] = i * 0.001;
		}
		chunk_size = ARRAY_SIZE / num_threads;
		auto start = chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(num_threads) shared(a,b) private(i) reduction(+:dot_product)
		{
				int tid = omp_get_thread_num();
				int start_index = tid * chunk_size;
				int end_index = start_index + chunk_size;
				double local_dot_product = 0.0;
				for (i = start_index; i < end_index; i++) {
						local_dot_product += a[i] * b[i];
				}
				dot_product += local_dot_product;
		}
		auto end = chrono::high_resolution_clock::now();
		auto dur = chrono::duration_cast<chrono::microseconds>(end - start);
		cout << "The time taken is " << dur.count() << endl;
		printf("The dot product of the arrays is: %f\n", dot_product);
		dot_product = 0;
		start = chrono::high_resolution_clock::now();
		for(int i=0;i<ARRAY_SIZE;i++){
				dot_product += a[i]*b[i];
		}
		end = chrono::high_resolution_clock::now();
		auto dur2 = chrono::duration_cast<chrono::microseconds>(end - start);
		cout << "The time taken is " << dur2.count() << endl;
		cout << "The time difference is " << dur2.count()-dur.count() << endl;
		double sp = dur2.count();
		sp /= dur.count();
		cout << "The speed up is " << sp << endl;
		free(a);
		free(b);
		return 0;
}

