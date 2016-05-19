/*
* ctmf.c - Constant-time median filtering
* Written by: Simon Perreault
*             P. Hébert
*             Mikhail Paulyshka
*/

#include "config.h"
#include "imageData.h"
#include "filter_cpu.h"

/* Заголовки С */
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdint>

typedef struct 
{
    uint16_t coarse[16];
    uint16_t fine[16][16];
} Histogram;

static inline void histogram_add(const uint16_t x[16], uint16_t y[16])
{
    for (int i = 0; i < 16; ++i)
    {
        y[i] += x[i];
    }
}

static inline void histogram_sub(const uint16_t x[16], uint16_t y[16])
{
    for (int i = 0; i < 16; ++i)
    {
        y[i] -= x[i];
    }
}

static inline void histogram_muladd(const uint16_t a, const uint16_t x[16], uint16_t y[16])
{
    for (int i = 0; i < 16; ++i)
    {
        y[i] += a * x[i];
    }
}

#define HOP(j,x,op) \
    h_coarse[ 16*j + (x>>4) ] op; \
    h_fine[ 16 * (width*(x>>4) + j) + (x & 0xF) ] op;


#ifndef MIN
#define MIN(a,b) ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif

void filter_cpu_ctmf_channel(uint8_t* src, unsigned int width, unsigned int height, int r)
{
    uint8_t* dst = new uint8_t[width*height];
    
    int b,k;
    const uint8_t *p, *q;

    Histogram H;
    uint16_t *h_coarse;
    uint16_t *h_fine;
    uint16_t luc[16];

    assert(r >= 0);
    assert(width >= 2 * r + 1);
    assert(height >= 2 * r + 1);

    h_coarse = (uint16_t*)calloc(1 * 16 * width, sizeof(uint16_t));
    h_fine = (uint16_t*)calloc(16 * 16 * width, sizeof(uint16_t));

    /* First row initialization */
    for (int j = 0; j < width; ++j)
    {
        HOP(j, src[j], += r + 1);
    }

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            HOP(j, src[width*i + j], ++);
        }
    }

    for (int i = 0; i < height; ++i) {

        /* Update column histograms for entire row. */
        p = src + width * MAX(0, i - r - 1);
        q = p + width;
        for (int j = 0; p != q; ++j, ++p)
        {
            HOP(j, *p, --);
        }

        p = src + width * MIN(height - 1, i + r);
        q = p + width;
        for (int j = 0; p != q; ++j, ++p)
        {
            HOP(j, *p, ++);
        }

        /* First column initialization */
        memset(&H, 0, sizeof(H));
        memset(&luc, 0, sizeof(luc));

        histogram_muladd(r, &h_coarse[0], H.coarse);


        for (int j = 0; j < r; ++j)
        {
            histogram_add(&h_coarse[16 * j], H.coarse);
        }

        for (k = 0; k < 16; ++k)
        {
            histogram_muladd(2 * r + 1, &h_fine[16 * width * k], &H.fine[k][0]);
        }

        for (int j = 0; j < width; ++j)
        {
            const uint16_t t = 2 * r*r + 2 * r;
            uint16_t sum = 0, *segment;

            histogram_add(&h_coarse[16 * MIN(j + r, width - 1)], H.coarse);

            /* Find median at coarse level */
            for (k = 0; k < 16; ++k)
            {
                sum += H.coarse[k];
                if (sum > t)
                {
                    sum -= H.coarse[k];
                    break;
                }
            }
            assert(k < 16);

            /* Update corresponding histogram segment */
            if (luc[k] <= j - r)
            {
                memset(&H.fine[k], 0, 16 * sizeof(uint16_t));
                for (luc[k] = j - r; luc[k] < MIN(j + r + 1, width); ++luc[k])
                {
                    histogram_add(&h_fine[16 * (width * k + luc[k])], H.fine[k]);
                }
                if (luc[k] < j + r + 1)
                {
                    histogram_muladd(j + r + 1 - width, &h_fine[16 * (width * k + (width - 1))], &H.fine[k][0]);
                    luc[k] = j + r + 1;
                }
            }
            else
            {
                for (; luc[k] < j + r + 1; ++luc[k])
                {
                    histogram_sub(&h_fine[16 * (width * k + MAX(luc[k] - 2 * r - 1, 0))], H.fine[k]);
                    histogram_add(&h_fine[16 * (width * k + MIN(luc[k], width - 1))], H.fine[k]);
                }
            }

            histogram_sub(&h_coarse[16 * MAX(j - r, 0)], H.coarse);

            /* Find median in segment */
            segment = H.fine[k];
            for (b = 0; b < 16; ++b)
            {
                sum += segment[b];
                if (sum > t)
                {
                    dst[width*i + j] = 16 * k + b;
                    break;
                }
            }
            assert(b < 16);
        }

    }

    free(h_coarse);
    free(h_fine);
    
    
    memcpy(src, dst, width*height*sizeof(uint8_t));
    delete[] dst;
}

/**
* \brief Constant-time median filtering
*
* \param imgData       Входное изображение.
* \param windowRadius  Радиус окна медианного фильтра. Ядро будет представлять собой квадрат 2*windowRadius+1 на 2*windowRadius+1.
*/
void filter_cpu_ctmf(ImageData* imgData, const unsigned int windowSize)
{
    unsigned int windowRadius = windowSize / 2;

    if (imgData->isColor)
    {
        #ifdef ENABLE_FEATURE_OPENMP

        #pragma omp parallel for num_threads(ImageData::CHANNEL_COUNT_RGB)
        for (int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {
            filter_cpu_ctmf_channel(imgData->data_uint8[i], imgData->width, imgData->height, windowRadius);
        }
       
        #else
        for (int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {
            filter_cpu_ctmf_channel(imgData->data_uint8[i], imgData->width, imgData->height, windowRadius);
        }

        #endif
    }
    else
    {
        filter_cpu_ctmf_channel(imgData->data_uint8[3], imgData->width, imgData->height, windowRadius);
    }
}