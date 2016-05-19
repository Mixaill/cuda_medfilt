#include <fstream>
#include <iostream>

#include "config.h"
#include "image.h"
#include "noise.h"
#include "utils.h"

void help()
{
    std::cout << "coursework_addnoise: adds noise to image" << std::endl << std::endl
        << "| usage: coursework_addnoise <input.bmp> [noise_type] [param1] [param2]" << std::endl
        << "|   input.bmp  : input filename" << std::endl
        << "|   noise_type : type of noise" << std::endl
        << "|     gaussian    : gaussian noise " << std::endl
        << "|     salt_pepper : salt and pepper noise " << std::endl << std::endl
        << "| parameters for gaussian noise:" << std::endl
        << "|     param1: sigma parametr of gaussian noise" << std::endl << std::endl
        << "| parameters for salt and pepper noise" << std::endl
        << "|     param1: low probability" << std::endl
        << "|     param2: high probability" << std::endl;

}

void add_noise(int argc, char* argv[])
{
    //load image
    std::string filename = std::string(argv[1]);
    ImageData* imageData = Image::Load(filename);
    if (imageData == nullptr)
    {
        std::cout << "Could not load image" << std::endl;
        exit(1);
    }

    //add some noise
    std::string noisetype = std::string(argv[2]);
    if (noisetype == "gaussian")
    {
        unsigned int stddev = DEFAULT_GAUSSIAN_STDDEV;

        if (argc > 3)
        {
            stddev = atoi(argv[3]);
        }
        Noise::AddGaussian(imageData, stddev);
    }
    else if (noisetype == "salt_pepper")
    {
        double probability_low = DEFAULT_SALTPEPPER_PROBABILITY_LO;
        double probability_high= DEFAULT_SALTPEPPER_PROBABILITY_HI;
       
        if (argc > 4)
        {
            probability_high = std::stod(argv[4]);
        }

        if (argc > 3)
        {
            probability_low = std::stod(argv[3]);
        }

        Noise::AddSaltAndPepper(imageData, probability_low, probability_high);
    }
    else
    {
        std::cout << "Unknown noise type!" << std::endl << std::endl;
        help();
        delete imageData;
        exit(2);
    }
  

    //save
	std::string ext = string_get_extension(filename);
	int len = ext.length();
	if (len == 0)
		ext = "png";
	else
		len++;
    Image::Save(imageData,filename.substr(0,filename.size()-len)+"_noise."+ext); //-V112
 
    delete imageData;
}

int main(int argc, char* argv[])
{
    if(argc<3)
    {
        help();
    }
    else
    {
        add_noise(argc,argv);
    }

    std::cout << "Press any key to exit..." << std::endl;
    std::cin.get();
    return 0;
}