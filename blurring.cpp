#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
// #define NUM_THREADS 100

using namespace std;

vector<vector<vector<unsigned char>>> add_pixels(vector<vector<vector<unsigned char>>> matrix, int mask, long alto, long ancho) {
  unsigned char white_pixel = '0';
  vector<unsigned char> channels (3, white_pixel);

  for (int i = 0; i < alto; i++) {
    for (int j = 0; j < (mask - 1) / 2; j++) {
      matrix[i].insert(matrix[i].begin(), channels);
      matrix[i].insert(matrix[i].end(), channels);
    }
  }

  vector<vector <unsigned char>> row ((ancho + mask - 1), channels);

  for (int i = 0; i < (mask - 1) / 2; i ++) {
    matrix.insert(matrix.begin(), row);
    matrix.insert(matrix.end(), row);
  }

  return matrix;
}

void blur_image(vector<vector<vector<unsigned char>>> matrix, int mask, long alto, long ancho, FILE *outputImage) {
  long firstRow = (matrix.size() - alto) / 2;
  long lastRow = firstRow + alto - 1;
  long lastColumn = firstRow + ancho - 1;
  int outerPixels = (mask - 1) / 2;

  for (long i = firstRow; i <= lastRow; i++) {
    for (long j = firstRow; j <= lastColumn; j++) {
      long colorWeight = 0;
      for (long k = i - outerPixels; k <= i + outerPixels; k++) {
        for (long l = j - outerPixels; l <= j + outerPixels; l++) {
          colorWeight += matrix[k][l][0];
        }
      }
      colorWeight /= mask * mask;
      fputc(colorWeight, outputImage);
      fputc(colorWeight, outputImage);
      fputc(colorWeight, outputImage);
    }
  }
  fclose(outputImage);
}

int main(){

  // omp_set_num_threads(NUM_THREADS);
  FILE *image, *outputImage;
  image = fopen("TestingImages/f4.bmp","rb"); // Imagen original a transformar
  outputImage = fopen("f3_blur.bmp","wb"); // Imagen transformada

  long ancho, alto;
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

  for(int i = 0; i < alto; i++){

    vector<vector<unsigned char>> temporal;
    unsigned char b, g, r;

    for(int j = 0; j < ancho; j++){
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
    
    matrix.push_back(temporal);

  }

  fclose(image);
  vector<vector<vector<unsigned char>>> matrixAddPixels = add_pixels(matrix, 100, alto, ancho);
  blur_image(matrixAddPixels, 11, alto, ancho, outputImage);

  return 0;

}