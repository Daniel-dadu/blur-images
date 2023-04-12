#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <omp.h>
#define NUM_THREADS 50

using namespace std;

void blur_image(vector<vector<vector<unsigned char>>> matrix, int mask, long alto, long ancho, FILE *outputImage);

int main(){

  omp_set_num_threads(NUM_THREADS);
  FILE *image = fopen("TestingImages/f4.bmp","rb"); // Imagen original a transformar

  vector<vector<vector<unsigned char>>> matrix;

  unsigned char xx[138];

  for(int j = 0; j < 138; j++){
    xx[j] = fgetc(image);
  }

  int ancho = (long)xx[20]*65536 + (long)xx[19]*256 + (long)xx[18];
  int alto = (long)xx[24]*65536 + (long)xx[23]*256 + (long)xx[22];
  // cout << "alto img " << alto << endl;
  // cout << "ancho img " << ancho << endl;

  int padding = ancho % 4;

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

    for(int j = 0; j < padding; j++){
      fgetc(image);
    }
    
    matrix.push_back(temporal);

  }

  fclose(image);

  int i, inicio = 11;
  #pragma omp parallel private(i)
  {

    #pragma omp for 
    for(i = inicio; i <= 15; i+=2) {
      FILE *outputImage;
      string title = "Mask_" + to_string(i) + ".bmp";
      outputImage = fopen(title.c_str(), "wb"); // Imagen transformada

      for(int j = 0; j < 138; j++){
        fputc(xx[j], outputImage); // Copia cabecera a nueva imagen
      }

      blur_image(matrix, i, alto, ancho, outputImage);
    }

  }

  return 0;

}

void blur_image(vector<vector<vector<unsigned char>>> matrix, int mask, long alto, long ancho, FILE *outputImage) {
  long firstRow = (matrix.size() - alto) / 2;
  long lastRow = firstRow + alto - 1;
  long lastColumn = firstRow + ancho - 1;
  int outerPixels = (mask - 1) / 2;
  unsigned char white_pixel = '0';

  for (long i = firstRow; i <= lastRow; i++) {
    for (long j = firstRow; j <= lastColumn; j++) {
      long colorWeight = 0;
      for (long k = i - outerPixels; k <= i + outerPixels; k++) {
        for (long l = j - outerPixels; l <= j + outerPixels; l++) {
          if (k < firstRow || k > lastRow || l < firstRow || l > lastColumn) {
            colorWeight += white_pixel;
          }
          else {
            colorWeight += matrix[k][l][0];
          }
        }
      }
      colorWeight /= mask * mask;
      fputc(colorWeight, outputImage);
      fputc(colorWeight, outputImage);
      fputc(colorWeight, outputImage);
    }

    for(int j = 0; j < (ancho % 4); j++){
      fputc('0', outputImage);
    }

  }
  fclose(outputImage);
}