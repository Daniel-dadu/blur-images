#include <iostream>
#include <vector>
#include <windows.h> // Solo disponible en MinGW (Windows)

using namespace std;


unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    FILE *filePtr;  //our file pointer
    BITMAPFILEHEADER bitmapFileHeader;  //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    //open file in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    //verify that this is a .BMP file by checking bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); 

    //move file pointer to the beginning of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    cout << "Image Size (in Bytes): " << bitmapInfoHeader->biSizeImage << endl;
    cout << "Image Width (in Pixels): " << bitmapInfoHeader->biWidth << endl;
    cout << "Image Height (in Pixels): " << bitmapInfoHeader->biHeight << endl;
    cout << "Padding (in Bytes): " << ((bitmapInfoHeader->biWidth * 3) % 4) << endl;
    cout << "Total file size (in Bytes): " << bitmapFileHeader.bfSize << endl;

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,1,filePtr);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
        return NULL;
    }

    //swap the R and B values to get RGB (bitmap is BGR)
    for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3)
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    //close file and return bitmap image data
    fclose(filePtr);
    return bitmapImage;
}

int main() {
    BITMAPINFOHEADER bitmapInfoHeader;
    unsigned char *bitmapData;
    // ...
    bitmapData = LoadBitmapFile("TestingImages/f5.bmp",&bitmapInfoHeader);
}


/*
int main(){

  // omp_set_num_threads(NUM_THREADS);
  FILE *image, *outputImage;
  image = fopen("TestingImages/sample.bmp","rb"); // Imagen original a transformar
  outputImage = fopen("sample_blur_2.bmp","wb"); // Imagen transformada
  
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

  long padding = (ancho * 3) % 4;

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

    for(int p = 0; p < padding; p++) fgetc(image);

    matrix.push_back(temporal);

  }

  for(vector<vector<unsigned char>> v : matrix){
    for(vector<unsigned char> c : v){
        fputc(c[0], outputImage);
        fputc(c[1], outputImage);
        fputc(c[2], outputImage);
    }
    for(int p = 0; p < padding; p++) fputc(255, outputImage);
  }


  fclose(image);
  return 0;

}

*/
