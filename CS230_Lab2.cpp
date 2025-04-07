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

	int x1, y1, x2, y2, dx, dy, p, sx, sy;
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

	/*
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	sx = (x2 >= x1) ? 1 : -1;
	sy = (y2 >= y1) ? 1 : -1;
	j = y1;
	i = x1;
	if (dx > dy) {
		
		p = 2 * dy - dx;
		for (int count = 0; count <= dx;  count ++) {
			bits[i][j] = bits[i - 1][j] = bits[i + 1][j] = IMAGE_SIZE - j;
			bits[i][j - 1] = IMAGE_SIZE - (j - 1);
			bits[i][j + 1] = IMAGE_SIZE - (j + 1);
			i += sx;
			if (p < 0) {
				p += (2 * dy);
			}
			else{
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
    __asm {
        PUSH EAX
        PUSH EBX
        PUSH ECX
        PUSH EDX
        PUSH ESI
        PUSH EDI
        PUSH EBP
        PUSH ESP


00DCAC00  mov         eax,dword ptr [x2]
00DCAC06  sub         eax,dword ptr [x1]
00DCAC0C  push        eax
00DCAC0D  call        _abs (0DC125Dh)
00DCAC12  add         esp,4
00DCAC15  mov         dword ptr [dx],eax
	//dy = abs(y2 - y1);
00DCAC1B  mov         eax,dword ptr [y2]
00DCAC21  sub         eax,dword ptr [y1]
00DCAC27  push        eax
00DCAC28  call        _abs (0DC125Dh)
00DCAC2D  add         esp,4
00DCAC30  mov         dword ptr [dy],eax
	//sx = (x2 >= x1) ? 1 : -1;
00DCAC36  mov         eax,dword ptr [x2]
00DCAC3C  cmp         eax,dword ptr [x1]
00DCAC42  jl          __$EncStackInitStart+4E2h (0DCAC50h)
00DCAC44  mov         dword ptr [ebp-10BFCh],1
00DCAC4E  jmp         __$EncStackInitStart+4ECh (0DCAC5Ah)
00DCAC50  mov         dword ptr [ebp-10BFCh],0FFFFFFFFh
00DCAC5A  mov         ecx,dword ptr [ebp-10BFCh]
00DCAC60  mov         dword ptr [sx],ecx
	//sy = (y2 >= y1) ? 1 : -1;
00DCAC66  mov         eax,dword ptr [y2]
00DCAC6C  cmp         eax,dword ptr [y1]
00DCAC72  jl          __$EncStackInitStart+512h (0DCAC80h)
00DCAC74  mov         dword ptr [ebp-10BFCh],1
00DCAC7E  jmp         __$EncStackInitStart+51Ch (0DCAC8Ah)
00DCAC80  mov         dword ptr [ebp-10BFCh],0FFFFFFFFh
00DCAC8A  mov         ecx,dword ptr [ebp-10BFCh]
00DCAC90  mov         dword ptr [sy],ecx
	//j = y1;
00DCAC96  mov         eax,dword ptr [y1]
00DCAC9C  mov         dword ptr [j],eax
	//i = x1;
00DCACA2  mov         eax,dword ptr [x1]
00DCACA8  mov         dword ptr [i],eax
	//if (dx > dy) {
00DCACAE  mov         eax,dword ptr [dx]
00DCACB4  cmp         eax,dword ptr [dy]
00DCACBA  jle         __$EncStackInitStart+6C7h (0DCAE35h)

	//p = 2 * dy - dx;
00DCACC0  mov         eax,dword ptr [dy]
00DCACC6  shl         eax,1
00DCACC8  sub         eax,dword ptr [dx]
00DCACCE  mov         dword ptr [p],eax
	//for (int count = 0; count <= dx;  count ++) {
00DCACD4  mov         dword ptr [ebp-105B8h],0
00DCACDE  jmp         __$EncStackInitStart+581h (0DCACEFh)
00DCACE0  mov         eax,dword ptr [ebp-105B8h]
00DCACE6  add         eax,1
00DCACE9  mov         dword ptr [ebp-105B8h],eax
00DCACEF  mov         eax,dword ptr [ebp-105B8h]
00DCACF5  cmp         eax,dword ptr [dx]
00DCACFB  jg          __$EncStackInitStart+6C2h (0DCAE30h)
	//bits[i][j] = bits[i - 1][j] = bits[i + 1][j] = IMAGE_SIZE - j;
00DCAD01  mov         eax,100h
00DCAD06  sub         eax,dword ptr [j]
00DCAD0C  mov         dword ptr [ebp-10BFCh],eax
00DCAD12  mov         ecx,dword ptr [i]
00DCAD18  add         ecx,1
00DCAD1B  shl         ecx,8
00DCAD1E  lea         edx,bits[ecx]
00DCAD25  mov         eax,dword ptr [j]
00DCAD2B  mov         cl,byte ptr [ebp-10BFCh]
00DCAD31  mov         byte ptr [edx+eax],cl
00DCAD34  mov         edx,dword ptr [i]
00DCAD3A  sub         edx,1
00DCAD3D  shl         edx,8
00DCAD40  lea         eax,bits[edx]
00DCAD47  mov         ecx,dword ptr [j]
00DCAD4D  mov         dl,byte ptr [ebp-10BFCh]
00DCAD53  mov         byte ptr [eax+ecx],dl
00DCAD56  mov         eax,dword ptr [i]
00DCAD5C  shl         eax,8
00DCAD5F  lea         ecx,bits[eax]
00DCAD66  mov         edx,dword ptr [j]
00DCAD6C  mov         al,byte ptr [ebp-10BFCh]
00DCAD72  mov         byte ptr [ecx+edx],al
	//bits[i][j - 1] = IMAGE_SIZE - (j - 1);
00DCAD75  mov         eax,dword ptr [j]
00DCAD7B  sub         eax,1
00DCAD7E  mov         ecx,100h
00DCAD83  sub         ecx,eax
00DCAD85  mov         edx,dword ptr [i]
00DCAD8B  shl         edx,8
00DCAD8E  lea         eax,bits[edx]
00DCAD95  mov         edx,dword ptr [j]
00DCAD9B  mov         byte ptr [eax+edx-1],cl
	//bits[i][j + 1] = IMAGE_SIZE - (j + 1);
00DCAD9F  mov         eax,dword ptr [j]
00DCADA5  add         eax,1
00DCADA8  mov         ecx,100h
00DCADAD  sub         ecx,eax
00DCADAF  mov         edx,dword ptr [i]
00DCADB5  shl         edx,8
00DCADB8  lea         eax,bits[edx]
00DCADBF  mov         edx,dword ptr [j]
00DCADC5  mov         byte ptr [eax+edx+1],cl
	//i += sx;
00DCADC9  mov         eax,dword ptr [i]
00DCADCF  add         eax,dword ptr [sx]
00DCADD5  mov         dword ptr [i],eax
	//if (p < 0) {
00DCADDB  cmp         dword ptr [p],0
00DCADE2  jge         __$EncStackInitStart+68Dh (0DCADFBh)
	//p += (2 * dy);
00DCADE4  mov         eax,dword ptr [dy]
00DCADEA  mov         ecx,dword ptr [p]
00DCADF0  lea         edx,[ecx+eax*2]
00DCADF3  mov         dword ptr [p],edx
	//}
00DCADF9  jmp         __$EncStackInitStart+6BDh (0DCAE2Bh)
	//else{j += sy;
00DCADFB  mov         eax,dword ptr [j]
00DCAE01  add         eax,dword ptr [sy]
00DCAE07  mov         dword ptr [j],eax
	//p += (2 * dy) - (2 * dx);
00DCAE0D  mov         eax,dword ptr [dy]
00DCAE13  shl         eax,1
00DCAE15  mov         ecx,dword ptr [dx]
00DCAE1B  shl         ecx,1
00DCAE1D  sub         eax,ecx
00DCAE1F  add         eax,dword ptr [p]
00DCAE25  mov         dword ptr [p],eax
			//}

		//}
00DCAE2B  jmp         __$EncStackInitStart+572h (0DCACE0h)
	//}
00DCAE30  jmp         __$EncStackInitStart+837h (0DCAFA5h)
	//else {
		//p = (2 * dx) - dy;
00DCAE35  mov         eax,dword ptr [dx]
00DCAE3B  shl         eax,1
00DCAE3D  sub         eax,dword ptr [dy]
00DCAE43  mov         dword ptr [p],eax
		//for (int count = 0; count <= dy; count ++) {
00DCAE49  mov         dword ptr [ebp-105C4h],0
00DCAE53  jmp         __$EncStackInitStart+6F6h (0DCAE64h)
00DCAE55  mov         eax,dword ptr [ebp-105C4h]
00DCAE5B  add         eax,1
00DCAE5E  mov         dword ptr [ebp-105C4h],eax
00DCAE64  mov         eax,dword ptr [ebp-105C4h]
00DCAE6A  cmp         eax,dword ptr [dy]
00DCAE70  jg          __$EncStackInitStart+837h (0DCAFA5h)
			//bits[i][j] = bits[i - 1][j] = bits[i + 1][j] = IMAGE_SIZE - j;
00DCAE76  mov         eax,100h
00DCAE7B  sub         eax,dword ptr [j]
00DCAE81  mov         dword ptr [ebp-10BFCh],eax
00DCAE87  mov         ecx,dword ptr [i]
00DCAE8D  add         ecx,1
00DCAE90  shl         ecx,8
00DCAE93  lea         edx,bits[ecx]
00DCAE9A  mov         eax,dword ptr [j]
00DCAEA0  mov         cl,byte ptr [ebp-10BFCh]
00DCAEA6  mov         byte ptr [edx+eax],cl
00DCAEA9  mov         edx,dword ptr [i]
00DCAEAF  sub         edx,1
00DCAEB2  shl         edx,8
00DCAEB5  lea         eax,bits[edx]
00DCAEBC  mov         ecx,dword ptr [j]
00DCAEC2  mov         dl,byte ptr [ebp-10BFCh]
00DCAEC8  mov         byte ptr [eax+ecx],dl
00DCAECB  mov         eax,dword ptr [i]
00DCAED1  shl         eax,8
00DCAED4  lea         ecx,bits[eax]
00DCAEDB  mov         edx,dword ptr [j]
00DCAEE1  mov         al,byte ptr [ebp-10BFCh]
00DCAEE7  mov         byte ptr [ecx+edx],al
			//bits[i][j - 1] = IMAGE_SIZE - (j - 1);
00DCAEEA  mov         eax,dword ptr [j]
00DCAEF0  sub         eax,1
00DCAEF3  mov         ecx,100h
00DCAEF8  sub         ecx,eax
00DCAEFA  mov         edx,dword ptr [i]
00DCAF00  shl         edx,8
00DCAF03  lea         eax,bits[edx]
00DCAF0A  mov         edx,dword ptr [j]
00DCAF10  mov         byte ptr [eax+edx-1],cl
			//bits[i][j + 1] = IMAGE_SIZE - (j + 1);
00DCAF14  mov         eax,dword ptr [j]
00DCAF1A  add         eax,1
00DCAF1D  mov         ecx,100h
00DCAF22  sub         ecx,eax
00DCAF24  mov         edx,dword ptr [i]
00DCAF2A  shl         edx,8
00DCAF2D  lea         eax,bits[edx]
00DCAF34  mov         edx,dword ptr [j]
00DCAF3A  mov         byte ptr [eax+edx+1],cl
			//j += sy;
00DCAF3E  mov         eax,dword ptr [j]
00DCAF44  add         eax,dword ptr [sy]
00DCAF4A  mov         dword ptr [j],eax
			//if (p < 0) {
00DCAF50  cmp         dword ptr [p],0
00DCAF57  jge         __$EncStackInitStart+802h (0DCAF70h)
				//p += (2 * dx);
00DCAF59  mov         eax,dword ptr [dx]
00DCAF5F  mov         ecx,dword ptr [p]
00DCAF65  lea         edx,[ecx+eax*2]
00DCAF68  mov         dword ptr [p],edx
			//}
00DCAF6E  jmp         __$EncStackInitStart+832h (0DCAFA0h)
			//else {
				//i += sx;
00DCAF70  mov         eax,dword ptr [i]
00DCAF76  add         eax,dword ptr [sx]
00DCAF7C  mov         dword ptr [i],eax
				//p += (2 * dx) - (2 * dy);
00DCAF82  mov         eax,dword ptr [dx]
00DCAF88  shl         eax,1
00DCAF8A  mov         ecx,dword ptr [dy]
00DCAF90  shl         ecx,1
00DCAF92  sub         eax,ecx
00DCAF94  add         eax,dword ptr [p]
00DCAF9A  mov         dword ptr [p],eax
			//}
		//}
00DCAFA0  jmp         __$EncStackInitStart+6E7h (0DCAE55h)
	//}
	

        POP ESP
        POP EBP
        POP EDI
        POP ESI
        POP EDX
        POP ECX
        POP EBX
        POP EAX
    }*/

	
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