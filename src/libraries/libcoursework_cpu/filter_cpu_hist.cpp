#include "config.h"
#include "imageData.h"
#include "filter_cpu.h"
#include "utils.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdint>


void filter_cpu_hist_channel(uint8_t* data, const unsigned int width, const unsigned int height, const int windowSize, const int edge_x, const int edge_y)
{
	int windowRadius = windowSize/2;
    int windowLength = windowSize*windowSize;

    short hist[256];
    int cpt = 0;
    uint8_t ic = 0;

	uint8_t* data_out = new uint8_t[width*height];

    for(int y=edge_y;y<height-edge_y;y++)
	{
		for(int x=edge_x;x<width-edge_y;x++)
		{
			//clear hist
			for (int i = 0; i < 256; i++)
			{
				hist[i] = 0;
			}

			//fill hist
		    for (int iy = y - windowRadius; iy <= y + windowRadius; iy++)
    		{
        		for (int ix = x - windowRadius; ix <= x + windowRadius; ix++)
        		{
            		hist[data[iy*width + ix]]++;
        		}
    		}

			cpt = 0;
			ic = 0;


		    //parse hist
    		for (ic = 0; ic < 256; ic++)
    		{
        		cpt += hist[ic];

				if (cpt > windowLength/2)
				{
				    break;
				}
   			 }

    		data_out[y*width + x] = ic;
		}
	}

	memcpy(data,data_out,width*height*sizeof(uint8_t));
	delete[] data_out;
}


void filter_cpu_hist(ImageData* imageData, const unsigned int windowSize)
{
    assert(windowSize % 2 != 0); //windowSize must be odd number

    ImageData* imageDataExtended = image_extend(imageData,windowSize);

    if (imageData->isColor)
    {
        #ifdef ENABLE_FEATURE_OPENMP

        #pragma omp parallel for num_threads(ImageData::CHANNEL_COUNT_RGB)
        for (int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {
            filter_cpu_hist_channel(imageDataExtended->data_uint8[i], imageDataExtended->width, imageDataExtended->height, windowSize,windowSize/2,windowSize/2);
        }

        #else
        for (int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {
            filter_cpu_hist_channel(imageDataExtended->data_uint8[i], imageDataExtended->width, imageDataExtended->height, windowSize,windowSize/2,windowSize/2);
        }
        #endif
    }
    else
    {
        filter_cpu_hist_channel(imageData->data_uint8[3], imageData->width, imageData->height, windowSize,windowSize/2,windowSize/2);
    }

    image_placeExtendedBack(imageData,imageDataExtended,windowSize);
    delete imageDataExtended;
}
