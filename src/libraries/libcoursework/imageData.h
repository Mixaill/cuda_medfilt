#pragma once 

#include "libcoursework_api.h"

#include <cstdint>
#include <cstring>

//—труктура, описывающа€ пиксель изображени€
typedef union
{
    unsigned char arr[4]; //-V112
    struct
    {
        uint8_t r; //r-red-красный
        uint8_t g; //g-green-зеленый
        uint8_t b; //b-blue-синий
        uint8_t a; //a-alpha-альфа канал или €ркость дл€ монохромного изображени€
    };
}cw_uchar4;

typedef struct
{
    double r; //r-red-красный
    double g; //g-green-зеленый
    double b; //b-blue-синий
    double a; //a-alpha-альфа канал или €ркость дл€ монохромного изображени€
} cw_double4;

// ласс, описывающий изображение
class LIBCOURSEWORK_API ImageData{ //-V690
public:
    static const unsigned int CHANNEL_COUNT_RGB = 3;
    static const unsigned int CHANNEL_COUNT_RGBA = 4; //-V112

    unsigned int width;   //ширина
    unsigned int height;  //высота
    bool isColor = true;  //цветное или монохромное

    uint8_t**       data_uint8;    //пиксели (uint8_t**)
    cw_uchar4*      data_cwuchar4; //пиксели  (cw_uchar4*)

	ImageData(unsigned int w, unsigned int h);
    ImageData(unsigned int w, unsigned int h,bool color);

	~ImageData();

	//ѕеревод изображени€ из цветного в монохромное
    void toGrayscale();

    //√енераци€ unsigned char** из cw_uchar4
    void GenerateUchar();

    //генераци€ cw_uchar4 из unsigned char**
    void GenerateCWUchar4();
	
    //
    void toOneChannel(unsigned int remaining_channel);
};