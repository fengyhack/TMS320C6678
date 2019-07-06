#include "mcip_common.h"
#include "stdlib.h"
#include "string.h"

const int NGRAY = 256;
const byte b0 = (byte)0;
const byte bx = (byte)255;

//八邻域
static int OD[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

void ObjectDetection(byte* dst,byte* src,int width,int height)
{
	ImageBW(dst,src,width,height);
	ImageClose(dst,width,height,3,3);
	int size=width*height;
	int min=10,max=300; //!!!!!!!!!
	short* map = (short*)malloc(size*sizeof(short));
	int n = CCLabeling(map,dst,width,height);
	CCAreaThresholding(map,size,n,min,max);
	ReMapping(dst,map,size);
	free(map);
}

void CCAreaThresholding(short* map,int size,int n,int min,int max)
{
	int M=(n+1)*sizeof(int);
	int* areas = (int*)malloc(M);
	memset(areas,0,M);

	int i,L;
	for(i=0;i<size;++i)
	{
		L = map[i];
		if(L)
		{
			++areas[L];
		}
	}

	int cnt = 0;

	for(i=1;i<=n;++i)
	{
		if(areas[i]<min||areas[i]>max)
		{
			areas[i]=0;
			++cnt;
		}
	}

	for(i=0;i<size;++i)
	{
		if(areas[map[i]]==0)
		{
			map[i]=0;
		}
	}

	free(areas);
}

// CC = ConnectedComponent
int CCLabeling(short* map, byte *img, int width, int height)
{
	Queue* Q = (Queue*)malloc(sizeof(Queue));
	Q->first = NULL;
	Q->last  = NULL;

	memset(map, 0, width * height*sizeof(short));

	int r, c, k, S;
	short L = 0;
	for(r = 1; r < height - 1; ++r)
	{
		for(c = 1; c < width - 1; ++c)
		{
			k = r * width + c;
			if(img[k] == bx && map[k] == (short)0)
			{
				++L;
				SearchNeighbor(map, img, width, height, L, k, Q);
				S = DeQueue(Q);
				while(S > -1)
				{
					SearchNeighbor(map, img, width, height, L, S, Q);
					S = DeQueue(Q);
				}
			}
		}
	}

	free(Q);

	return L;
}

void SearchNeighbor(short* map, byte *img, int width, int height, short L, int S, Queue *Q)
{
	map[S] = L;
	int i,k,N = width * height;
	for(i = 0; i < 8; ++i)
	{
		k = S + OD[i][0] * width + OD[i][1];
		if(k > 0 && k < N && img[k] == bx && map[k] == (short)0)
		{
			map[k] = L;
			EnQueue(Q, k);
		}
	}
}

int DeQueue(Queue *Q)
{
	QNode *p = Q->first;
	if(p == NULL)
	{
		return -1;
	}

	int data = p->data;

	if(p->next == NULL)
	{
		Q->first = NULL;
		Q->last  = NULL;
	}
	else
	{
		Q->first = p->next;
	}

	free(p);
	return data;
}

void EnQueue(Queue *Q, int data)
{
	QNode *p = (QNode*)malloc(sizeof(QNode));
	p->data = data;

	if(Q->first == NULL)
	{
		Q->first = p;
		Q->last  = p;
		p->next = NULL;
	}
	else
	{
		p->next = NULL;
		Q->last->next = p;
		Q->last = p;
	}
}

void ReMapping(byte* dst,short* src,int size)
{
	short min=src[0];
	short max=src[0];

	int i;
	for(i=1;i<size;++i)
	{
		if(src[i]>max) max=src[i];
		else if(src[i]<min) min=src[i];
		else continue;
	}

	float f = 255.0f/(max-min);

	for(i=0;i<size;++i)
	{
		dst[i]=(byte)((src[i]-min)*f);
	}
}

void ImageBW(byte* dst, byte* src,int width,int height)
{
	int size = width*height;
	byte thresh = 32;
	int window = 3;

	byte* grad=(byte*)malloc(size);

	CalcGradient(grad, src, width, height);
	GradientFilter2(src, grad, size);

	free(grad);

	int* hist = (int*)malloc(sizeof(int)*NGRAY);
	int* hist_s = (int*)malloc(sizeof(int)*NGRAY);

	CalcHist(hist, src, size);

	HistSmoothing(hist_s, hist, window);

	free(hist);

	thresh = (byte)FindBestThresh(hist_s, window);

	free(hist_s);

	ImageThresholding(dst, src, size, thresh);
}

// calc gradient
// gradient filtering
// calc histgram
// histgram smoothing
// find best threshold

void CalcHist(int* hist, byte* imgData, int imgSize)
{
	memset(hist, 0, sizeof(int)*NGRAY);

	int i;

	for (i = 0; i < imgSize; ++i)
	{
		++hist[imgData[i]];
	}
}

void HistSmoothing(int* dst_hist, int* src_hist, int window)
{
	if (window < 1 || window>5)
	{
		window = 3;
	}

	int* tmp = (int*)malloc(sizeof(int)*(window + NGRAY + window));

	int i;

	for (i = 0; i < window; ++i)
	{
		tmp[i] = src_hist[0];
	}
	for (i = window; i < window + NGRAY; ++i)
	{
		tmp[i] = src_hist[i - window];
	}
	for (i = window + NGRAY; i < window + NGRAY + window; ++i)
	{
		tmp[i] = src_hist[NGRAY - 1];
	}

	int j,sum;
	int w_ext = window + 1 + window;

	for (i = 0; i < NGRAY; ++i)
	{
		sum = window;
		for (j = -window; j <= window; ++j)
		{
			sum += tmp[window + i + j];
		}
		dst_hist[i] = sum / w_ext;
	}

	free(tmp);
}

void ImageThresholding(byte* dst, byte* src, int imgSize, byte thresh)
{
	byte bv;

	int i;
	for (i = 0; i < imgSize; ++i)
	{
		bv = src[i];
		dst[i] = bv > thresh ? bx : b0;
	}
}

int FindFirstPeak(int* smooth_hist, int search_window)
{
	if (search_window < 2 || search_window>10)
	{
		search_window = 2;
	}

	int max = smooth_hist[0];
	int index = search_window;
	int i, next;
	for (i = search_window + 1; i < NGRAY; ++i)
	{
		if (smooth_hist[i] > max)
		{
			max = smooth_hist[i];
			index = i;
		}
		else
		{
			next = IndexOfNextMax(smooth_hist, max, i + 1, search_window);
			if (next > 0)
			{
				i = next;
				max = smooth_hist[i];
				index = i;
			}
			else
			{
				break;
			}
		}
	}

	return index;
}

int FindFirstNadir(int* smooth_hist, int i_first_peak, int search_window)
{
	if (search_window < 2 || search_window>10)
	{
		search_window = 2;
	}

	int min = smooth_hist[i_first_peak];
	int index = i_first_peak;
	int i, next;
	for (i = i_first_peak + 1; i < NGRAY; ++i)
	{
		if (smooth_hist[i] < min)
		{
			min = smooth_hist[i];
			index = i;
		}
		else
		{
			next = IndexOfNextMin(smooth_hist, min, i + 1, search_window);
			if (next > 0)
			{
				i = next;
				min = smooth_hist[i];
				index = i;
			}
			else
			{
				break;
			}
		}
	}

	return index;
}

int FindLastPeak(int* smooth_hist, int search_window)
{
	if (search_window < 2 || search_window>10)
	{
		search_window = 2;
	}

	int max = smooth_hist[NGRAY - 1];
	int index = NGRAY - 1;
	int i, prev;
	for (i = NGRAY - 2; i > search_window; --i)
	{
		if (smooth_hist[i] >= max)
		{
			max = smooth_hist[i];
			index = i;
		}
		else
		{
			prev = IndexOfPrevMax(smooth_hist, max, i - 1, search_window);
			if (prev > 0)
			{
				i = prev;
				max = smooth_hist[i];
				index = i;
			}
			else
			{
				break;
			}
		}
	}

	return index;
}

int FindLastNadir(int* smooth_hist, int i_last_peak, int search_window)
{
	if (search_window < 2 || search_window>10)
	{
		search_window = 2;
	}

	int min = smooth_hist[i_last_peak];
	int index = i_last_peak;
	int i, prev;
	for (i = i_last_peak - 1; i > 0; --i)
	{
		if (smooth_hist[i] <= min)
		{
			min = smooth_hist[i];
			index = i;
		}
		else
		{
			prev = IndexOfPrevMin(smooth_hist, min, i - 1, search_window);
			if (prev > 0)
			{
				i = prev;
				min = smooth_hist[i];
				index = i;
			}
			else
			{
				break;
			}
		}
	}

	return index;
}

int IndexOfNextMax(int* smooth_hist, int curr_max, int index, int search_window)
{
	int i;

	for (i = index; i < index + search_window && i < NGRAY; ++i)
	{
		if (smooth_hist[i] >= curr_max) return i;
	}

	return -1;
}

int IndexOfNextMin(int* smooth_hist, int curr_min, int index, int search_window)
{
	int i;

	for (i = index; i < index + search_window && i < NGRAY; ++i)
	{
		if (smooth_hist[i] <= curr_min) return i;
	}

	return -1;
}

int IndexOfPrevMax(int* smooth_hist, int curr_max, int index, int search_window)
{
	int i;

	for (i = index; i > index - search_window && i > 0; --i)
	{
		if (smooth_hist[i] >= curr_max) return i;
	}

	return -1;
}

int IndexOfPrevMin(int* smooth_hist, int curr_min, int index, int search_window)
{
	int i;

	for (i = index; i > index - search_window && i > 0; --i)
	{
		if (smooth_hist[i] <= curr_min) return i;
	}

	return -1;
}

void ImageTopHat(byte* imgData, int width, int height, int window)
{
	int N = width*height;

	byte* tmp = (byte*)malloc(N);
	memcpy(tmp, imgData, N);

	ImageErosion(tmp, width, height, window);
	ImageDilation(tmp, width, height, window);

	int i;
	for (i = 0; i < N; ++i)
	{
		imgData[i] -= tmp[i];
	}

	free(tmp);
}

void ImageOpen(byte* imgData, int width, int height, int window_erosion, int window_dilation)
{
	ImageErosion(imgData, width, height, window_erosion);
	ImageDilation(imgData, width, height, window_dilation);
}

void ImageClose(byte* imgData, int width, int height, int window_erosion, int window_dilation)
{
	ImageDilation(imgData, width, height, window_dilation);
	ImageErosion(imgData, width, height, window_erosion);
}

void ImageErosion(byte* imgData, int width, int height, int window)
{
	//int k = 0;
	int i,j;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			//imgData[k + j] = GetCellMin(imgData, width, height, i, j, window);
			CellErosion(imgData, width, height, i, j, window);
		}
		//k += width;
	}
}

void ImageDilation(byte* imgData, int width, int height, int window)
{
	//int k = 0;
	int i,j;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			//imgData[k + j] = GetCellMax(imgData, width, height, i, j, window);
			CellDilation(imgData, width, height, i, j, window);
		}
		//k += width;
	}
}

byte GetCellMax(byte* imgData, int width, int height, int row, int col, int window)
{
	int row_start = row - window;
	int row_end = row + window;
	int col_start = col - window;
	int col_end = col + window;

	if (row_start < 0) row_start = 0;
	if (row_end > height) row_end = height;
	if (col_start < 0) col_start = 0;
	if (col_end > width) col_end = width;

	int n = (row_end - row_start + 1)*(col_end - col_start + 1);
	byte* tmp = (byte*)malloc(n);

	int i,j,k = 0;
	int p = row_start*width + col_start;
	for (i = row_start; i < row_end; ++i)
	{
		for (j = col_start; j < col_end; ++j)
		{
			tmp[k] = imgData[p + j];
			++k;
		}
		p += width;
	}

	byte max = tmp[0];
	for (k = 1; k< n; ++k)
	{
		if (tmp[k] > max) max = tmp[k];
	}

	free(tmp);

	return max;
}

byte GetCellMin(byte* imgData, int width, int height, int row, int col, int window)
{
	int row_start = row - window;
	int row_end = row + window;
	int col_start = col - window;
	int col_end = col + window;

	if (row_start < 0) row_start = 0;
	if (row_end > height - 1) row_end = height - 1;
	if (col_start < 0) col_start = 0;
	if (col_end > width - 1) col_end = width - 1;

	int n = (row_end - row_start + 1)*(col_end - col_start + 1);
	byte* tmp = (byte*)malloc(n);

	int i,j,k = 0;
	int p = row_start*width + col_start;
	for (i = row_start; i <= row_end; ++i)
	{
		for (j = col_start; j <= col_end; ++j)
		{
			tmp[k] = imgData[p + j];
			++k;
		}
		p += width;
	}

	byte min = tmp[0];
	for (k = 1; k < n; ++k)
	{
		if (tmp[k] < min) min = tmp[k];
	}

	free(tmp);

	return min;
}

// 如果cell的中心像素值center_pixel_value = 0 则不作任何操作
// 如果cell里面值为0的像素超过半数，则置center_pixel_value = 0
void CellErosion(byte* imgData, int width, int height, int row, int col, int window)
{
	int p = row*width + col;
	if (imgData[p] == b0) return;

	int row_start = row - window;
	int row_end = row + window;
	int col_start = col - window;
	int col_end = col + window;

	if (row_start < 0) row_start = 0;
	if (row_end > height - 1) row_end = height - 1;
	if (col_start < 0) col_start = 0;
	if (col_end > width - 1) col_end = width - 1;

	int count_black = 0;

	int i,j,k;
	for (i = row_start; i <= row_end; ++i)
	{
		for (j = col_start; j <= col_end; ++j)
		{
			k = i*width + j;
			if (imgData[k] == (byte)0) ++count_black;
		}
	}

	int n_half = (row_end - row_start + 1)*(col_end - col_start + 1) / 2;
	if (count_black > n_half)
	{
		imgData[p] = b0;
	}
}

// 如果cell的中心像素值center_pixel_value = 255 则不作任何操作
// 如果cell里面值为255的像素超过半数，则置center_pixel_value = 255
void CellDilation(byte* imgData, int width, int height, int row, int col, int window)
{
	int p = row*width + col;
	if (imgData[p] == bx) return;

	int row_start = row - window;
	int row_end = row + window;
	int col_start = col - window;
	int col_end = col + window;

	if (row_start < 0) row_start = 0;
	if (row_end > height - 1) row_end = height - 1;
	if (col_start < 0) col_start = 0;
	if (col_end > width - 1) col_end = width - 1;

	int count_white = 0;

	int i, j, k;
	for (i = row_start; i <= row_end; ++i)
	{
		for (j = col_start; j <= col_end; ++j)
		{
			k = i*width + j;
			if (imgData[k] == bx) ++count_white;
		}
	}

	int n_half = (row_end - row_start + 1)*(col_end - col_start + 1) / 2;
	if (count_white > n_half)
	{
		imgData[p] = bx;
	}
}

void ImageMeanFilter(byte* imgData, int width, int height, int window)
{
	int i, j, k = 0;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			imgData[k + j] = GetCellMean(imgData, width, height, i, j, window);
		}
		k += width;
	}
}

byte GetCellMean(byte* imgData, int width, int height, int row, int col, int window)
{
	if (window == 0)
	{
		return imgData[row*width + col];
	}

	int row_start = row - window;
	int row_end = row + window;
	int col_start = col - window;
	int col_end = col + window;

	if (row_start < 0) row_start = 0;
	if (row_end > height - 1) row_end = height - 1;
	if (col_start < 0) col_start = 0;
	if (col_end > width - 1) col_end = width - 1;

	int n = (row_end - row_start + 1)*(col_end - col_start + 1);

	int i, j, k;
	int sum = 0;
	for (i = row_start; i <= row_end; ++i)
	{
		k = i*width;
		for (j = col_start; j <= col_end; ++j)
		{
			sum += imgData[k + j];
		}
	}

	sum /= n;

	return (byte)sum;
}

void EdgeDetection(byte* edge, byte* bwSrc, int width, int height)
{
	int row, col, k, m;
	int dx, dy;

	memset(edge, 0, width*height);

	for (row = 1; row < height; ++row)
	{
		m = row*width;
		for (col = 1; col < width; ++col)
		{
			k = m + col;
			dx = bwSrc[k] - bwSrc[k - 1];
			dy = bwSrc[k] - bwSrc[k - width];
			if ((dx != 0) || (dy != 0))
			{
				edge[k] = bx;
			}
		}
	}
}

void CalcGradient(byte* grad, byte* img, int width, int height)
{
	int row, col, k, m;
	int g, dx, dy, ax, ay;

	memset(grad, 0, width*height);

	for (row = 1; row < height; ++row)
	{
		m = row*width;
		for (col = 1; col < width; ++col)
		{
			k = m + col;
			dx = img[k] - img[k - 1];
			dy = img[k] - img[k - width];
			ax = dx>0 ? dx : -dx;
			ay = dy>0 ? dy : -dy;
			g = ax + ay + 1;
			grad[k] = g>1 ? (byte)(g >> 1) : b0;
		}
	}
}

void GradientFilter(byte* img, byte* grad, int width, int height, byte thresh, int window)
{
	int i, j, k = 0, g, v;
	//int imgSize = width*height;

	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			g = grad[k];
			if (g > 1)
			{
				v = GetCellMean(img, width, height, i, j, window);
				if (v > thresh)
				{
					v = img[k] * g;
					img[k] = v > 255 ? bx : (byte)v;
				}
			}
			++k;
		}
	}
}

void GradientFilter2(byte* img, byte* grad, int imgSize)
{
	int i, v, g;

	for (i = 0; i < imgSize; ++i)
	{
		g = grad[i];
		if (g > 1)
		{
			v = g * img[i];
			img[i] = v > 255 ? bx : (byte)v;
		}
	}
}

int FindBestThresh(int* smooth_hist, int serach_window)
{
	int x1 = FindFirstPeak(smooth_hist, serach_window);
	int x2 = FindLastPeak(smooth_hist, serach_window);

	if (x1 < 1) x1 = 1;
	if (x2 < 1) x2 = NGRAY - 2;

	int x_m = x1 + 1;
	int y_m = smooth_hist[x_m];

	int i;

	for (i = x_m; i < x2; ++i)
	{
		if (smooth_hist[i] < y_m)
		{
			x_m = i;
			y_m = smooth_hist[i];
		}
	}

	int y1 = smooth_hist[x1];
	double k = 1.0*(y1 - y_m) / (x1 - x_m);

	int dmax = 0, delta;

	int thresh = x1 + 1;

	int x;

	for (x = x1 + 1; x <= x_m; ++x)
	{
		delta = (int)((y1 - k*(x1 - x)) - smooth_hist[x]);
		if (delta > dmax)
		{
			dmax = delta;
			thresh = x;
		}
	}

	return thresh;
}

// EOF
