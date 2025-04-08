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

	int x1, y1, x2, y2, deltaX, deltaY, incrementX, incrementY; // for assembnly code
	//int x1, y1, x2, y2, dx, dy, sx, sy, p; //for the c++ code

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

	
	deltaX = (x2 >= x1) ? (x2 - x1) : (x1 - x2) ;
	deltaY = (y2 >= y1) ? (y2 - y1) : (y1 - y2);
	incrementX = (x2 >= x1) ? 1 : -1;
	incrementY = (y2 >= y1) ? 1 : -1;
	j = y1;
	i = x1;
	__asm{
		push eax;
		push ebx;
		push ecx;
		push edx;
		push esi;
		push edi;
		push ebp;
		
		mov eax, deltaX; //eax = deltaX
		mov ebx, deltaY; //ebx = deltaY
		cmp eax, ebx;
		jg dxGreater;

	dyGreater:
		mov edx, deltaX;
		shl edx, 1;
		sub edx, deltaY; // edx = p;

		mov eax, i; // eax = i
		mov ebx, j; // ebx = j
		mov ecx, 0; //ecx = count
	forLoop_Y:
		//load address of array[i][j]
		mov esi, eax; //esi = i
		imul esi, IMAGE_SIZE;
		add esi, ebx; //esi += j
		lea edi, [bits + esi]; //edi =  &bits[i][j]

		//set bit[i][j] = imageSize - j
		mov esi, IMAGE_SIZE;
		sub esi, ebx;
		and [edi], 0xFF00; //zero out the upper bits
		or [edi], esi; //bits[i[]j] = IMAGE_SIZE - j


		add ebx, incrementY; //j += incrementY

		mov esi, deltaY; //esi = dy
		shl esi, 1; //esi =( 2 * dy) 

		cmp edx, 0; // p < 0
		jl negP_Y //jimp to else
			add eax, incrementX; //i += incrementX
			sub edx, esi // p -= 2 * dy

			negP_Y :
		mov esi, deltaX; //esi = dx
		shl esi, 1; //esi =( 2 * dx) 

		add edx, esi; // p += 2* dx

		add ecx, 1;
		cmp ecx, deltaY;
		jg endForLoop_Y;
		jmp forLoop_Y


//////////////////////////////////////////
	dxGreater:
		mov edx, deltaY;
		shl edx, 1;
		sub edx, deltaX; // edx = p;

		mov eax, i; // eax = i
		mov ebx, j; // ebx = j
		mov ecx, 0; //ecx = count
	forLoop_X:
		//load address of array[i][j]
		mov esi, eax; //esi = i
		imul esi, IMAGE_SIZE;
		add esi, ebx; //esi += j
		lea edi, [bits + esi]; //edi =  &bits[i][j]

		//set bit[i][j] = imageSize - j
		mov esi, IMAGE_SIZE;
		sub esi, ebx;
		and [edi], 0xFF00; //zero out the upper bits
		or [edi], esi; //bits[i[]j] = IMAGE_SIZE - j

		
		add eax, incrementX; //i += incrementX

		mov esi, deltaX; //esi = dx
		shl esi, 1; //esi =( 2 * dx) 

		cmp edx, 0; // p < 0
		jl negP_X //jimp to else
			add ebx, incrementY; //j += sy
			sub edx, esi // p -= 2 * dx

		negP_x:
			mov esi, deltaY; //esi = dy
			shl esi, 1; //esi =( 2 * dy) 

			add edx, esi; // p += 2* dy

		add ecx, 1;
		cmp ecx, deltaX;
		jg endForLoop_x;
		jmp forLoop_x
		
	endForLoop_x:
	endForLoop_y:


		

		pop ebp;
		pop edi;
		pop esi;
		pop edx;
		pop ecx;
		pop ebx;
		pop eax;
	}
	
	
	/*
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	sx = (x2 >= x1) ? 1 : -1;
	sy = (y2 >= y1) ? 1 : -1;
	j = y1;
	i = x1;
	if (dx > dy) {
		
		p = 2 * dy - dx;
		for (int count = 0; count <= dx; count++) {
			bits[i][j] = bits[i - 1][j] = bits[i + 1][j] = IMAGE_SIZE - j;
			bits[i][j - 1] = IMAGE_SIZE - (j - 1);
			bits[i][j + 1] = IMAGE_SIZE - (j + 1);
			i += sx;
			if (p < 0) {
				p += (2 * dy);
			}
			else {
				j += sy;
				p += (2 * dy) - (2 * dx);
			}
		}
	}

	else {
		p = (2 * dx) - dy;
		for (int count = 0; count <= dy; count ++) {
			bits[i][j] = bits[i - 1][j] = bits[i + 1][j] = IMAGE_SIZE - j;
			bits[i][j - 1] = IMAGE_SIZE - (j - 1);
			bits[i][j + 1] = IMAGE_SIZE - (j + 1);
			j += sy;
			if (p < 0) {
				p += (2 * dx);
			}
			else {
				i += sx;
				p += (2 * dx) - (2 * dy);
			}
		}
	}
	
	
	*/
	    


	
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