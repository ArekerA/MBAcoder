#include <bitset> 
using namespace std;
#pragma once
class Encoder
{
public:
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
	class oct_node {
	public:
		unsigned char index, poziom;
		uint64_t liczba;
		oct_node *kids[8], *parent;
		void usunKids();
		bool operator > (const oct_node& x) const
		{
			return (liczba > x.liczba);
		}
	};
#pragma pack(pop)
	long long licznik[32] = {};
	char nowyLicznik[32] = {};//posortowane po czêstotliwoœci wystêpowania
	BMPFileHeader* bfh;
	BMPInfoHeader* bih;
	Pixel** pixs;
	char** pixels;
	string plik, plikDocelowy;

	Encoder();
	Encoder(string plik, string plikDocelowy);
	~Encoder();
	char* najblizszy(Pixel *x);
	char* najblizszySzary(Pixel *x);
	unsigned char najblizszyR(unsigned char *r);
	unsigned char najblizszyG(unsigned char *g);
	unsigned char najblizszyB(unsigned char *b);
	short* rice(char x);
	void sortuj(char** pixels, int w, int h);
	void kodujKolor();
	void kodujSzary();
	void kodujPaleta();
};

