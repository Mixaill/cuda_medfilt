#pragma once 

#include "libcoursework_api.h"

#include <cstdint>
#include <cstring>

//���������, ����������� ������� �����������
typedef union
{
    unsigned char arr[4]; //-V112
    struct
    {
        uint8_t r; //r-red-�������
        uint8_t g; //g-green-�������
        uint8_t b; //b-blue-�����
        uint8_t a; //a-alpha-����� ����� ��� ������� ��� ������������ �����������
    };
}cw_uchar4;

typedef struct
{
    double r; //r-red-�������
    double g; //g-green-�������
    double b; //b-blue-�����
    double a; //a-alpha-����� ����� ��� ������� ��� ������������ �����������
} cw_double4;

//�����, ����������� �����������
class LIBCOURSEWORK_API ImageData{ //-V690
public:
    static const unsigned int CHANNEL_COUNT_RGB = 3;
    static const unsigned int CHANNEL_COUNT_RGBA = 4; //-V112

    unsigned int width;   //������
    unsigned int height;  //������
    bool isColor = true;  //������� ��� �����������

    uint8_t**       data_uint8;    //������� (uint8_t**)
    cw_uchar4*      data_cwuchar4; //�������  (cw_uchar4*)

	ImageData(unsigned int w, unsigned int h);
    ImageData(unsigned int w, unsigned int h,bool color);

	~ImageData();

	//������� ����������� �� �������� � �����������
    void toGrayscale();

    //��������� unsigned char** �� cw_uchar4
    void GenerateUchar();

    //��������� cw_uchar4 �� unsigned char**
    void GenerateCWUchar4();
	
    //
    void toOneChannel(unsigned int remaining_channel);
};