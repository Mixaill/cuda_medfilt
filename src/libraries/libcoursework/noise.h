#pragma once 

#include "imageData.h"
#include "libcoursework_api.h"
#include "config.h"

class LIBCOURSEWORK_API Noise{
public:
    static void AddGaussian(ImageData* a, unsigned int stddev = DEFAULT_GAUSSIAN_STDDEV);
    static void AddSaltAndPepper(ImageData* a, double probability_low = DEFAULT_SALTPEPPER_PROBABILITY_LO, double probability_high = DEFAULT_SALTPEPPER_PROBABILITY_HI);

private:
    static uint8_t addGaussianLimitPixel(double t);
    static uint8_t addSaltAndPepperForPixel(uint8_t, double probability, double probability_low = DEFAULT_SALTPEPPER_PROBABILITY_LO, double probability_high = DEFAULT_SALTPEPPER_PROBABILITY_HI);
};
