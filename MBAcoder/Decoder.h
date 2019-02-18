#include <bitset> 
using namespace std;
#pragma once
class Decoder
{
#pragma pack(push, 1)
	struct BMPFileHeader
	{
		short bfType;
		int bfSize;
		short bfReserved1;
		short bfReserved2;
		int bfOffBits;
	};

	struct BMPInfoHeader
	{
		int biSize;
		int biWidth;
		int biHeight;
		short biPlanes;
		short biBitCount;
		int biCompression;
		int biSizeImage;
		int biXPelsPerMeter;
		int biYPelsPerMeter;
		int biClrUsed;
		int biClrImportant;
	};
	struct Pixel
	{
		unsigned char b, g, r;
	};
	struct MBAHeader
	{
		char type[3];
		char size[5];
		int width;
		int height;
		char data;
	};
#pragma pack(pop)
public:
	Decoder();
	Decoder(string plik, string plikDocelowy);
	~Decoder();
	void kolor();
	void szary();

	MBAHeader *header;
	char paleta[32];
	string plik, plikDocelowy;
	char** pixels;
	Pixel** pixs;
};

