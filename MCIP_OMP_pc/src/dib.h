/****************
*
* Bitmap utility 
* http://fengyh.cn/
*
*****************/

#ifndef DIB_H
#define DIB_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef int BOOL;

#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

const WORD BM_WORD = 0x4D42;

#pragma pack(push,1)
struct BITMAPHEADER
{
	WORD A1_bfType;  
	DWORD A2_bfSize;
	WORD A3_bfReserved1;
	WORD A4_bfReserved2;
	DWORD A5_bfOffBits;
	/***************************/
	DWORD B1_biSize;
	LONG B2_biWidth;
	LONG B3_biHeight;
	WORD B4_biPlanes;
	WORD B5_biBitCount;
	DWORD  B6_biCompression;
	DWORD  B7_biSizeImage;
	LONG  B8_biXPelsPerMeter;
	LONG  B9_biYPelsPerMeter;
	DWORD  BA_biClrUsed;
	DWORD  BB_biClrImportant;
};
#pragma pop()

struct RGBQ
{
	BYTE A1_rgbBlue;
	BYTE A2_rgbGreen;
	BYTE A3_rgbRed;
	BYTE A4_rgbReserved;
};

const int CSZ_BMH = sizeof(BITMAPHEADER);
const int CSZ_PAL = 256 * sizeof(RGBQ);

class DIBitmap
{
public:
	DIBitmap();
	~DIBitmap();
	BOOL Open(const char* fileName);
	BOOL Save(const char* fileName);
	BOOL Create(BYTE* data, int w, int h, int c = 1);

	LONG GetWidth() { return pBitmapHeader->B2_biWidth; }
	LONG GetHeight() { return pBitmapHeader->B3_biHeight; }
	LONG GetDepth() { return pBitmapHeader->B5_biBitCount; }
	LONG GetLineBytes() { return ((GetWidth()*GetDepth() + 31) >> 5) << 2; }
	BYTE* GetImageData() { return pDataBuffer; }

	BOOL ToGrayscale(BOOL useMean);

private:
	void InitBitmapHeader(BITMAPHEADER* pbmh);

	BITMAPHEADER* pBitmapHeader;
	RGBQ* palette;
	BYTE* pDataBuffer;
	LONG nBytesOfBuffer;
};

#endif
