// MBAcoder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Encoder.h"
#include "Decoder.h"
#include <iostream>
#include <fstream>
#include <clocale>
#include <bitset>
#include <algorithm>
#include <bitset> 
#include <vector>

using namespace std;

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
	char data[1];
};
#pragma pack(pop)

int main(int argc, char *argv[])
{
	std::setlocale(LC_ALL, "");
	if (argc == 2 && (!strcmp(argv[1], "help") || !strcmp(argv[1], "?")))
	{
		cout << "wpisz"<<endl<< "\ttyp plik_Ÿród³owy plik_docelowy" << endl<<"gdzie"<<endl<<"\ttyp"<<endl<<"\t\tenkoder_kolor | enkoder_szary | dekoder"<<endl;
		return 0;
	}
	if (argc != 4)
	{
		cout << "Nieporawna liczba argumetów" << endl << "wpisz help aby uzyskaæ pomoc"<<endl;
		return 0;
	}
	if (!strcmp(argv[1], "ek") || !strcmp(argv[1], "enkoder_kolor"))
	{
		cout << "Enkoder" << endl << endl;
		Encoder e(argv[2], argv[3]);
		e.kodujKolor();
	}
	if (!strcmp(argv[1], "es") || !strcmp(argv[1], "enkoder_szary"))
	{
		cout << "Enkoder" << endl << endl;
		Encoder e(argv[2], argv[3]);
		e.kodujSzary();
	}
	if (!strcmp(argv[1], "d") || !strcmp(argv[1], "dekoder"))
	{
		cout << "Dekoder" << endl<<endl;
		Decoder e(argv[2], argv[3]);
	}
	cout << endl << "Koniec" << endl << endl;
    return 0;
}

