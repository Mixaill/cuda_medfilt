#include "noise.h"
#include "utils.h"

#include <chrono>
#include <cmath>
#include <random>

void Noise::AddGaussian(ImageData* a, unsigned int stddev)
{
    #define MEAN 0

    std::default_random_engine generator((unsigned int)(std::chrono::system_clock::now().time_since_epoch().count()));
    std::normal_distribution<double> distr(MEAN, stddev);
    
    for (unsigned int m = 0; m < a->height; m++)
    {
        for (unsigned int n = 0; n < a->width; n++)
        {
            if (a->isColor)
            {
                for (unsigned int k = 0; k < 3; k++)
                {
                    a->data_uint8[k][m*a->width + n] = addGaussianLimitPixel(a->data_uint8[k][m*a->width + n] + round(distr(generator)));
                }
            }
            else
            {
                a->data_uint8[3][m*a->width + n] = addGaussianLimitPixel(a->data_uint8[3][m*a->width + n] + round(distr(generator)));
            }
        }      
    }
}

uint8_t Noise::addGaussianLimitPixel(double t)
{
    if (t > 255)
    {
        t = 255;
    }
    else if (t < 0)
    {
        t = 0;
    }
    return (uint8_t)t;
}

void Noise::AddSaltAndPepper(ImageData* a, double probability_low, double probability_high)
{
    std::default_random_engine generator((unsigned int)(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<double> distr(0,1);
    double temp = 0;

    for (unsigned int y = 0; y < a->height; y++)
    {
        for (unsigned int x = 0; x < a->width; x++)
        {
            if (a->isColor)
            {
                for (int k = 0; k < 3; k++)
                {
                    a->data_uint8[k][y*a->width + x] = addSaltAndPepperForPixel(a->data_uint8[k][y*a->width + x], distr(generator), probability_low, probability_high);
                }
            }
            else
            {
                a->data_uint8[3][y*a->width + x] = addSaltAndPepperForPixel(a->data_uint8[3][y*a->width + x], distr(generator), probability_low, probability_high);
            }
        }
    }
}

uint8_t Noise::addSaltAndPepperForPixel(uint8_t pixel, double probability, double probability_low, double probability_high)
{
    if (probability > probability_high)
    {
        return 255;
    }
    else if (probability < probability_low)
    {
        return 0;
    }
    else
    {
        return pixel;
    }
}