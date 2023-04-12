#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <omp.h>
#define NUM_THREADS 8

using namespace std;

int main() {
//   int i, nStart = 11;

//   #pragma omp parallel private(i)
//   {

//     #pragma omp for 
//     for(i = 11; i <= 20; i+=2) {
//         cout << i << endl;
//     }
//   }

    int i;
  {
    #pragma omp for
    for(int i = 11; i <= 20; i+=2){
        cout << i << endl;
    }
  }
}