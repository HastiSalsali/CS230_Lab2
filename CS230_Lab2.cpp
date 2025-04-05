//Salsali, Hasti CS230 Section 11091 4/6
//Lab 2 - Bresenham's Algorithm
#include <iostream>
#include <fstream>
#include "windows.h"
using namespace std;
//bool addFontChar(char* charMap, char* BMP, int x, int y, int h, int w);
// The following defines the size of the square image in pixels.
#define IMAGE_SIZE 256
int main(int argc, char* argv[])
{
	// refer to the Wikipedia entry for BMP file format:
	// https://en.wikipedia.org/wiki/BMP_file_format
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;
	char colorTable[1024];
	// The following defines the array which holds the image.  
	char bits[IMAGE_SIZE][IMAGE_SIZE] = { 0 };
	int i, j;
	//char* fontPtr;
	//char* bmpPtr;
	// Define and open the output file. 
	ofstream bmpOut("foo.bmp", ios::out + ios::binary);
	if (!bmpOut) {
		cout << "...could not open file, ending.";
		return -1;
	}
	// Initialize the bit map file header with static values.
	bmfh.bfType = 0x4d42;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(bmfh) + sizeof(bmih) + sizeof(colorTable);
	bmfh.bfSize = bmfh.bfOffBits + sizeof(bits);
	// Initialize the bit map information header with static values.
	bmih.biSize = 40;
	bmih.biWidth = IMAGE_SIZE;
	bmih.biHeight = IMAGE_SIZE;
	bmih.biPlanes = 1;
	bmih.biBitCount = 8;
	bmih.biCompression = 0;
	bmih.biSizeImage = IMAGE_SIZE * IMAGE_SIZE;
	bmih.biXPelsPerMeter = 2835;  // magic number, see Wikipedia entry
	bmih.biYPelsPerMeter = 2835;
	bmih.biClrUsed = 256;
	bmih.biClrImportant = 0;
	// Build color table.
	for (i = 0; i < 256; i++) {
		j = i * 4;
		colorTable[j] = colorTable[j + 1] = colorTable[j + 2] = colorTable[j + 3] = i;
		//colorTable[j] = colorTable[j + 1] = colorTable[j + 2] = colorTable[j +3] = 0;
		//colorTable[j + 2] = 255;
		//colorTable[j+2] = j;
		// for your edification, try setting one of the first three values to 255 instead of i
			// and see what it does to the generated bit map.
	}
	// Build gray scale array of bits in image, 
	for (i = 0; i < IMAGE_SIZE; i++) {
		for (j = 0; j < IMAGE_SIZE; j++) {
			bits[i][j] = j;
		}
	}

	int x1, y1, x2, y2, dx, dy, p, increment;
	cout << "Pierce College CS230 Spring 2025 Lab Assignment 2 - Salsali, Hasti\n"
		"Enter two pairs of point coordinates in the range of 0 to " << IMAGE_SIZE << ".\n";
	cin >> x1 >> y1 >> x2 >> y2;

	bool endProgram = 0;
	if (x1 < 0 || x1 > IMAGE_SIZE) {
		cout << "Value " << x1 << " out of range, ending.\n";
		endProgram = 1;
	}
	else if (x2 < 0 || x2 > IMAGE_SIZE) {
		cout << "Value " << x2 << " out of range, ending.\n";
		endProgram = 1;
	}
	else if (y1 < 0 || y1 > IMAGE_SIZE) {
		cout << "Value " << y1 << " out of range, ending.\n";
		endProgram = 1;
	}
	else if (y2 < 0 || y2 > IMAGE_SIZE) {
		cout << "Value " << y2 << " out of range, ending.\n";
		endProgram = 1;
	}
	if (endProgram) {
		bmpOut.close();
		return 1;
	}

	
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	incrementY = dy < 0 ? 1 : -1;
	if (abs(dx) > abs(dy)) {

		p = 2 * dy - dx;
		j = y1;
		i = x1;
		bits[i][j] = IMAGE_SIZE - j;
		for (i = x1; i <= x2; i ++) {
			if (p < 0) {
				p = p + (2 * dy);
			}
			else{
				j += increment;
				p = 2 * dy - (2 * dx);
			}
			bits[i][j] = IMAGE_SIZE - j;
		}
	}

	
	
	// Write out the bit map.  
	char* workPtr;
	workPtr = (char*)&bmfh;
	bmpOut.write(workPtr, 14);
	workPtr = (char*)&bmih;
	bmpOut.write(workPtr, 40);
	workPtr = &colorTable[0];
	bmpOut.write(workPtr, sizeof(colorTable));
	workPtr = &bits[0][0];
	bmpOut.write(workPtr, IMAGE_SIZE * IMAGE_SIZE);
	bmpOut.close();
	// Now let's look at our creation.
	system("mspaint foo.bmp");
	// Done.
	return 0;
}