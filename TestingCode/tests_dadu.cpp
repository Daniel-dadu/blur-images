// Código que permite conocer las características del archivo BMP
// Sobretodo, la información del header

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
    bitmapData = LoadBitmapFile("TestingImages/f4.bmp",&bitmapInfoHeader);
}
