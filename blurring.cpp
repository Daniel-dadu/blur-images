#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
// #define NUM_THREADS 100

using namespace std;

int main(){

  // omp_set_num_threads(NUM_THREADS);
  FILE *image, *outputImage;
  image = fopen("f3.bmp","rb"); // Imagen original a transformar
  outputImage = fopen("f4_blur.bmp","wb"); // Imagen transformada
  
  long ancho, alto;
  unsigned char* ptr;
  unsigned char xx[54];
  vector<vector<vector<unsigned char>>> matrix;
  
  for(int i=0; i < 54; i++){
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage); // Copia cabecera a nueva imagen
  }

  ancho = (long)xx[20]*65536 + (long)xx[19]*256 + (long)xx[18];
  alto = (long)xx[24]*65536 + (long)xx[23]*256 + (long)xx[22];
  cout << "alto img " << alto << endl;
  cout << "ancho img " << ancho << endl;
  ptr = (unsigned char*)malloc(alto*ancho*3*sizeof(unsigned char));

  long padding = ancho%4;

  for(int i = 0; i < alto; i++){

    vector<vector<unsigned char>> temporal;
    unsigned char b, g, r;
    
    // for(int p = 0; p < (padding/2); p++){
    //   b = fgetc(image);
    //   g = fgetc(image);
    //   r = fgetc(image);
    // }

    for(int j = 0; j < (ancho - padding); j++){
      vector<unsigned char> channels;
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);
      unsigned char pixel = ((0.21*r) + (0.72*g) + (0.07*b));
      channels.push_back(pixel); // B
      channels.push_back(pixel); // G
      channels.push_back(pixel); // R
      temporal.push_back(channels);
    }

    // for(int p = 0; p < (padding/2); p++){
    //   b = fgetc(image);
    //   g = fgetc(image);
    //   r = fgetc(image);
    // }
    
    matrix.push_back(temporal);

  }

  // for(int i = 0; !feof(image); i++){
  //   unsigned char b, g, r;
  //   b = fgetc(image);
  //   g = fgetc(image);
  //   r = fgetc(image);
  //   temporal.push_back((unsigned char)((0.21*r) + (0.72*g) + (0.07*b)));
  // }

  // matrix.push_back(temporal);

  for(vector<vector<unsigned char>> v : matrix){
    for(vector<unsigned char> c : v){
      fputc(c[0], outputImage);
      fputc(c[1], outputImage);
      fputc(c[2], outputImage);
    }
  }

  // ----------- YA FUNCIONA ------------- //
  // for(vector<unsigned char> v : matrix){
  //   for(int i = 0; i < v.size(); i++){
  //     fputc(v[i], outputImage);
  //     fputc(v[i+1], outputImage);
  //     fputc(v[i+2], outputImage);
  //   }
  // }
  
  // for(int i = 0; i < alto*ancho*3; ++i){
  //   fputc(matrix[i], outputImage);
  //   fputc(matrix[i+1], outputImage);
  //   fputc(matrix[i+2], outputImage);
  // }

  free(ptr);
  fclose(image);
  fclose(outputImage);
  return 0;

}