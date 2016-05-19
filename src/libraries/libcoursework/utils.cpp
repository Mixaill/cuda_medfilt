#include "utils.h"

ImageData* image_extend(ImageData* image,const int windowSize)
{
    const int w_edge = windowSize / 2;
    const int w_extended = image->width+ 2 * w_edge;

    const int h_edge = windowSize / 2;
    const int h_extended = image->height + 2 * h_edge;

    ImageData* image_new = new ImageData(w_extended,h_extended);
    image_new->isColor=image->isColor;

    //detect number of channels to process
    int c=0;
    int c_max=0;
    if(image->isColor)
    {
        c=0;
        c_max=ImageData::CHANNEL_COUNT_RGB;

    }else{
        c=3;
        c_max=ImageData::CHANNEL_COUNT_RGBA;
    }

    for(c;c<c_max;c++) {
        //place existing data
        for (unsigned int y = 0; y < image->height; y++) {
            memcpy(image_new->data_uint8[c] + w_extended * (h_edge + y) + w_edge,
                   image->data_uint8[c] + image->width * y, image->width * sizeof(uint8_t));

        }

        //fill with mirrored data: horizontal lines
        for (unsigned int y = 0; y < h_edge; y++) {
            memcpy(image_new->data_uint8[c] + h_edge * w_extended - (y + 1) * w_extended + w_edge,
                   image->data_uint8[c] + (y * image->width), image->width * sizeof(uint8_t));

            memcpy(image_new->data_uint8[c] + (image->height + h_edge + y) * w_extended  + w_edge,
                   image->data_uint8[c] + image->height * image->width - (y + 1) * image->width,
                   image->width * sizeof(uint8_t));

        }

        //fill with mirrored data: vertical lines
        for (unsigned int y = 0; y < h_extended; y++)
        {
            //vertical lines
            for (unsigned int x = 0; x < h_edge; x++)
            {
                image_new->data_uint8[c][y*w_extended + h_edge - (x+1)]= image_new->data_uint8[c][y*w_extended + h_edge +x];
                image_new->data_uint8[c][y*w_extended + (w_extended-h_edge) + x]= image_new->data_uint8[c][y*w_extended + (w_extended-h_edge)-1 - x ];
            }

        }

    }

    return image_new;
}

void image_placeExtendedBack(ImageData* image, ImageData* image_extended, const int windowSize)
{
    //detect edge
    const int w_edge = windowSize / 2;
    const int h_edge = windowSize / 2;

    //detect number of channels to process
    int c=0;
    int c_max=0;
    if(image->isColor)
    {
        c=0;
        c_max=ImageData::CHANNEL_COUNT_RGB;

    }else{
        c=3;
        c_max=ImageData::CHANNEL_COUNT_RGBA;
    }

    for (c; c < c_max; c++)
    {
        for (unsigned int y = 0; y < image->height; y++) {
            memcpy(image->data_uint8[c] + image->width * y, image_extended->data_uint8[c] + image_extended->width * (h_edge + y) + w_edge, image->width * sizeof(uint8_t));
        }
    }
}

ImageData* image_difference(ImageData* a, ImageData* b)
{
    if ( (a->width != b->width) || (a->height != b->height) )
    {
        return nullptr;
    }

    ImageData* c = new ImageData(a->width,a->height,a->isColor);

    for (unsigned int y = 0; y < c->height; y++)
    {
        for (unsigned int x = 0; x < c->width; x++)
        {
            if(a->isColor &&  b->isColor) {
                for (unsigned int k = 0; k < ImageData::CHANNEL_COUNT_RGB; k++) {
                    c->data_uint8[k][y * c->width + x] = (uint8_t) (abs(a->data_uint8[k][y * a->width + x] - b->data_uint8[k][y * b->width + x]));
                }
            }
            else{
                c->data_uint8[3][y * c->width + x] = (uint8_t) (abs(a->data_uint8[3][y * a->width + x] - b->data_uint8[3][y * b->width + x]));
            }
        }
    }

    return c;
}

bool string_ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) 
    { 
        return false; 
    }

    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string string_get_extension(std::string const & str)
{
    std::string::size_type idx;
    std::string extension;

    idx = str.rfind('.');

    if (idx != std::string::npos)
    {
        extension = str.substr(idx + 1);
    }

    return extension;
}

