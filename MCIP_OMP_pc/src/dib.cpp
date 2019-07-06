/****************
*
* Bitmap utility 
* http://fengyh.cn/
*
*****************/

#include "dib.h"
#include "time.h"

DIBitmap::DIBitmap()
{
	pBitmapHeader = (BITMAPHEADER*)malloc(CSZ_BMH);
	InitBitmapHeader(pBitmapHeader);

	pDataBuffer = NULL;
	palette = NULL;
	nBytesOfBuffer = 0;
}

DIBitmap::~DIBitmap()
{
	if(pBitmapHeader) free(pBitmapHeader);
	if(palette) free(palette);
	if(pDataBuffer) free(pDataBuffer);
}

void DIBitmap::InitBitmapHeader(BITMAPHEADER* pbmh)
{
	pbmh->A1_bfType = BM_WORD;
	pbmh->A2_bfSize = 0;
	pbmh->A3_bfReserved1 = 0;
	pbmh->A4_bfReserved2 = 0;
	pbmh->A5_bfOffBits = 0;
	pbmh->B1_biSize = 0;
	pbmh->B2_biWidth = 0;
	pbmh->B3_biHeight = 0;
	pbmh->B4_biPlanes = 1;
	pbmh->B5_biBitCount = 0;
	pbmh->B6_biCompression = 0;
	pbmh->B7_biSizeImage = 0;
	pbmh->B8_biXPelsPerMeter = 0;
	pbmh->B9_biYPelsPerMeter = 0;
	pbmh->BA_biClrUsed = 0;
	pbmh->BB_biClrImportant = 0;
}

BOOL DIBitmap::Open(const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (fp == NULL)
	{
		return FALSE;
	}

	WORD word;
	fread(&word, 2, 1, fp);
	if (word != BM_WORD)
	{
		fclose(fp);
		return FALSE;
	}

	fread(pBitmapHeader, CSZ_BMH, 1, fp);
	LONG pos = 2 + CSZ_BMH;
	int n = sizeof(RGBQ);
	if (pBitmapHeader->B5_biBitCount == 8)
	{
		palette = (RGBQ*)malloc(n * 256);
		fread(palette, n, 256, fp);
		pos += n * 256;
	}

	LONG w=pBitmapHeader->B2_biWidth;
	LONG h=pBitmapHeader->B3_biHeight;
	LONG c=(pBitmapHeader->B5_biBitCount)>>3;
	LONG nL=c*w;
	int nr=nL%4;

	nBytesOfBuffer=nL*h;
	pDataBuffer = (BYTE*)malloc(nBytesOfBuffer);
	if (pDataBuffer == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	BYTE tail[4];
	BYTE* buffer=pDataBuffer;
	int i;
	if(nr>0)
	{
		nr=4-nr;
		for(i=0;i<h;++i)
		{
			fread(buffer,nL,1,fp);
			buffer+=nL;
			fread(tail,1,nr,fp);
		}
	}
	else
	{
		fread(buffer, nBytesOfBuffer, 1, fp);
	}

	fclose(fp);
	return TRUE;
}

BOOL DIBitmap::Save(const char* fileName)
{
	if(pDataBuffer==NULL)
	{
		return FALSE;
	}

	FILE* fp = fopen(fileName, "wb");
	if (fp == NULL)
	{
		return FALSE;
	}

	fwrite(pBitmapHeader, CSZ_BMH, 1, fp);
	if (palette)
	{
		fwrite(palette, CSZ_PAL, 1, fp);
	}

	LONG w = pBitmapHeader->B2_biWidth;
	LONG h = pBitmapHeader->B3_biHeight;
	LONG c = (pBitmapHeader->B5_biBitCount) >> 3;
	LONG nLineBytes = (pBitmapHeader->B1_biSize) / h;
	LONG ncw=c*w;
	int r= nLineBytes - ncw;

	BYTE tail=0;
	BYTE* buffer=pDataBuffer;
	if(r>0)
	{
		for(int i=0;i<h;++i)
		{
			fwrite(buffer,ncw,1,fp);
			buffer+=ncw;
			fwrite(&tail,1,r,fp);
		}
	}
	else
	{
		fwrite(buffer, nBytesOfBuffer, 1, fp);
	}

	fflush(fp);
	fclose(fp);
	return TRUE;
}

BOOL DIBitmap::Create(BYTE* data,int w, int h,int c)
{
	if ((data == NULL) || (c != 1) || (c != 3))
	{
		return FALSE;
	}

	pBitmapHeader->B2_biWidth = w;
	pBitmapHeader->B3_biHeight = h;
	pBitmapHeader->B5_biBitCount = c * 8;
	LONG nLineBytes = ((c*w + 3) >> 2) << 2;
	pBitmapHeader->B1_biSize = nLineBytes*h;

	pBitmapHeader->A2_bfSize = CSZ_BMH + pBitmapHeader->B1_biSize;

	int i;
	if (c == 1)
	{
		pBitmapHeader->A2_bfSize += CSZ_PAL;
		if (palette == NULL)
		{
			palette = (RGBQ*)malloc(CSZ_PAL);
			for (i = 0; i < 256; ++i)
			{
				palette[i].A1_rgbBlue = i;
				palette[i].A2_rgbGreen = i;
				palette[i].A3_rgbRed = i;
				palette[i].A4_rgbReserved = 0;
			}
		}
	}
	else
	{
		if (palette)
		{
			free(palette);
			palette = NULL;
		}
	}

	nBytesOfBuffer = c*w*h;
	
	BYTE* oldBuffer = pDataBuffer;
	pDataBuffer = (BYTE*)malloc(nBytesOfBuffer);
	if (pDataBuffer == NULL)
	{
		printf("Failed to allocate memory!\n");
		return FALSE;
	}
	for (i = 0; i < nBytesOfBuffer; ++i)
	{
		pDataBuffer[i] = data[i];
	}
	free(oldBuffer);

	return TRUE;
}

BOOL DIBitmap::ToGrayscale(BOOL useMean)
{
	if(pDataBuffer==NULL)
	{
		return FALSE;
	}
	if (pBitmapHeader->B5_biBitCount != 24)
	{
		return FALSE;
	}

	pBitmapHeader->B5_biBitCount = 8;

	int i, j;
	if (palette == NULL)
	{
		palette = (RGBQ*)malloc(CSZ_PAL);
		for (i = 0; i < 256; ++i)
		{
			palette[i].A1_rgbBlue = i;
			palette[i].A2_rgbGreen = i;
			palette[i].A3_rgbRed = i;
			palette[i].A4_rgbReserved = 0;
		}
	}

	LONG w = pBitmapHeader->B2_biWidth;
	LONG h = pBitmapHeader->B3_biHeight;

	BYTE* oldBuffer = pDataBuffer;
	nBytesOfBuffer = w*h;
	pDataBuffer = (BYTE*)malloc(nBytesOfBuffer);
	BYTE r, g, b;
	BYTE* src = oldBuffer;
	BYTE* dst = pDataBuffer;

	if (useMean)
	{
		for (i = 0; i < h; ++i)
		{
			for (j = 0; j < w; ++j)
			{
				b = src[0];
				g = src[1];
				r = src[2];
				src+=3;
				*dst = (r + g + b) / 3;
				++dst;
			}
		}
	}
	else
	{
		for (i = 0; i < h; ++i)
		{
			for (j = 0; j < w; ++j)
			{
				b = src[0];
				g = src[1];
				r = src[2];
				src+=3;
				*dst = (r*76 + g*150 + b*30) >> 8;
				++dst;
			}
		}
	}


	free(oldBuffer);
	return TRUE;
}

