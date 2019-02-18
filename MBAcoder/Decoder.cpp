#include "stdafx.h"
#include "Decoder.h"
#include <iostream>
#include <fstream>
#include <clocale>
#include <bitset>
#include <algorithm>
#include <bitset> 
#include <vector>

using namespace std;
Decoder::Decoder()
{
}
Decoder::Decoder(string plik, string plikDocelowy)
{
	this->plik = plik;
	this->plikDocelowy = plikDocelowy;
	cout << "Czytanie Pliku: ";
	int licznik = 0;
	int licznikKrok = 0;
	ifstream ifs(plik, ios::binary);
	char* temp = new char[sizeof(MBAHeader)];
	ifs.read(temp, sizeof(MBAHeader));
	header = (MBAHeader*)(temp);
	if (header->type[0] == 'M' && header->type[1] == 'B' && header->type[2] == 'A')
	{
		cout << "Kolor" << endl;
		kolor();
	}
	else if (header->type[0] == 'A' && header->type[1] == 'B' && header->type[2] == 'M')
	{
		cout << "Plik w skali szaroœci" << endl;
		szary();
	}
	else if (header->type[0] == 'B' && header->type[1] == 'A' && header->type[2] == 'M')
	{
		cout << "Plik kolorowy z palet¹" << endl;
	}
	else
	{
		cout << "B³¹d nag³ówka <Typ>" << endl;
	}
	ifs.close();
}


Decoder::~Decoder()
{
}

void Decoder::kolor()
{
	ifstream ifs(plik, ios::binary);
	ifs.seekg(sizeof(MBAHeader));
	char c[20];
	ifs.read(c, sizeof(c));
	vector<bool> paletaTym;
	for (char x = 0;x < 20;++x)
	{
		bitset<8> v = bitset<8>(c[x]);
		for (char i = 7;i >= 0;--i)
		{
			paletaTym.push_back(v.test(i));
		}
	}
	char a = 0;
	for (char x = 0;x < 32;++x)
	{
		a = 0;
		for (char i = 0;i < 5;++i)
		{
			a = a << 1;
			a += paletaTym.back() ? 1 : 0;
			paletaTym.pop_back();
		}
		paleta[x] = a;
	}
	vector<bool> data;
	if (true)
	{
		cout << "Odczyt danych" << endl;
		int off = ifs.tellg();
		cout << off << endl;
		ifs.seekg(0, ifs.end);
		int length = (int)ifs.tellg() - off;
		ifs.seekg(off);
		char * buffer = new char[length];
		ifs.read(buffer, length);
		ifs.close();
		for (int i = 0;i < length;i++)
		{
			bitset<8> b = bitset<8>(buffer[i]);
			data.push_back(b.test(7));
			data.push_back(b.test(6));
			data.push_back(b.test(5));
			data.push_back(b.test(4));
			data.push_back(b.test(3));
			data.push_back(b.test(2));
			data.push_back(b.test(1));
			data.push_back(b.test(0));
		}
		delete[] buffer;

	}
	ifs.close();
	if (true)
	{
		cout << "Dekodowanie danych" << endl;
		int w = header->width - 1, h = header->height - 1;
		long long c = 0;
		char pixel = 0;
		bool b = false;
		pixels = new char*[header->height];
		for (int i = 0;i<header->height;++i)
			pixels[i] = new char[header->width];
		while (data.size() - (unsigned char)header->data > c)
		{
			b = data.at(c);
			pixel += b ? 1 : 0;
			if (b == 0)
			{
				pixel <<= 1;
				++c;
				b = data.at(c);
				pixel += (b ? 1 : 0);
				pixel <<= 1;
				++c;
				b = data.at(c);
				pixel += (b ? 1 : 0);
				pixels[h][w] = pixel;
				pixel = 0;
				--w;
				if (w < 0)
				{
					w = header->width - 1;
					--h;
				}

			}
			++c;
		}
	}
	if (true)
	{
		cout << "Przygotowanie bitmapy" << endl;
		pixs = new Pixel*[header->height];
		for (int i = 0;i<header->height;++i)
			pixs[i] = new Pixel[header->width];
		char c = 0;
		for (int y = 0; y < header->height; ++y)
		{
			for (int x = 0; x < header->width; ++x)
			{
				c = paleta[pixels[y][x]];
				pixs[y][x].b = (c % 2 == 1) ? 255 : 0;
				c >>= 1;
				pixs[y][x].g = (c % 4 == 3) ? 255 : ((c % 4 == 2) ? 170 : ((c % 4 == 1) ? 85 : 0));
				c >>= 2;
				pixs[y][x].r = (c % 4 == 3) ? 255 : ((c % 4 == 2) ? 170 : ((c % 4 == 1) ? 85 : 0));
			}
		}
	}
	BMPFileHeader bfh;
	bfh.bfType = 19778;
	bfh.bfSize = header->width * header->height * 3 + 54;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = 54;

	BMPInfoHeader bih;
	bih.biSize = 40;
	bih.biWidth = header->width;
	bih.biHeight = header->height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = 0;
	bih.biSizeImage = header->width * header->height * 3;
	bih.biXPelsPerMeter = 2850;
	bih.biYPelsPerMeter = 2850;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	ofstream ofs(plikDocelowy, ios::binary);
	ofs.write((char*)&bfh, sizeof(BMPFileHeader));
	ofs.write((char*)&bih, sizeof(BMPInfoHeader));
	int width = header->width;
	if (width % 4) width += 4 - (width % 4);
	cout << "Zapis danych" << endl;
	for (int i = 0; i < header->height; i++)
		//for (int i = header->height-1; i >= 0; --i)
	{
		ofs.write((char*)pixs[i], 3 * width);
	}
	ofs.close();
}void Decoder::szary()
{
	ifstream ifs(plik, ios::binary);
	ifs.seekg(sizeof(MBAHeader));
	char c[20];
	ifs.read(c, sizeof(c));
	vector<bool> paletaTym;
	for (char x = 0;x < 20;++x)
	{
		bitset<8> v = bitset<8>(c[x]);
		for (char i = 7;i >= 0;--i)
		{
			paletaTym.push_back(v.test(i));
		}
	}
	char a = 0;
	for (char x = 0;x < 32;++x)
	{
		a = 0;
		for (char i = 0;i < 5;++i)
		{
			a = a << 1;
			a += paletaTym.back() ? 1 : 0;
			paletaTym.pop_back();
		}
		paleta[x] = a;
	}
	vector<bool> data;
	if (true)
	{
		cout << "Odczyt danych" << endl;
		int off = ifs.tellg();
		cout << off << endl;
		ifs.seekg(0, ifs.end);
		int length = (int)ifs.tellg() - off;
		ifs.seekg(off);
		char * buffer = new char[length];
		ifs.read(buffer, length);
		ifs.close();
		for (int i = 0;i < length;i++)
		{
			bitset<8> b = bitset<8>(buffer[i]);
			data.push_back(b.test(7));
			data.push_back(b.test(6));
			data.push_back(b.test(5));
			data.push_back(b.test(4));
			data.push_back(b.test(3));
			data.push_back(b.test(2));
			data.push_back(b.test(1));
			data.push_back(b.test(0));
		}
		delete[] buffer;

	}
	ifs.close();
	if (true)
	{
		cout << "Dekodowanie danych" << endl;
		int w = header->width - 1, h = header->height - 1;
		long long c = 0;
		char pixel = 0;
		bool b = false;
		pixels = new char*[header->height];
		for (int i = 0;i<header->height;++i)
			pixels[i] = new char[header->width];
		while (data.size() - (unsigned char)header->data > c)
		{
			b = data.at(c);
			pixel += b ? 1 : 0;
			if (b == 0)
			{
				pixel <<= 1;
				++c;
				b = data.at(c);
				pixel += (b ? 1 : 0);
				pixel <<= 1;
				++c;
				b = data.at(c);
				pixel += (b ? 1 : 0);
				pixels[h][w] = pixel;
				pixel = 0;
				--w;
				if (w < 0)
				{
					w = header->width - 1;
					--h;
				}

			}
			++c;
		}
	}
	if (true)
	{
		cout << "Przygotowanie bitmapy" << endl;
		pixs = new Pixel*[header->height];
		for (int i = 0;i<header->height;++i)
			pixs[i] = new Pixel[header->width];
		char c = 0;
		for (int y = 0; y < header->height; ++y)
		{
			for (int x = 0; x < header->width; ++x)
			{
				c = paleta[pixels[y][x]];
				pixs[y][x].b = (unsigned char)c*8.23;
				pixs[y][x].g = (unsigned char)c*8.23;
				pixs[y][x].r = (unsigned char)c*8.23;
			}
		}
	}
	BMPFileHeader bfh;
	bfh.bfType = 19778;
	bfh.bfSize = header->width * header->height * 3 + 54;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = 54;

	BMPInfoHeader bih;
	bih.biSize = 40;
	bih.biWidth = header->width;
	bih.biHeight = header->height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = 0;
	bih.biSizeImage = header->width * header->height * 3;
	bih.biXPelsPerMeter = 2850;
	bih.biYPelsPerMeter = 2850;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	ofstream ofs(plikDocelowy, ios::binary);
	ofs.write((char*)&bfh, sizeof(BMPFileHeader));
	ofs.write((char*)&bih, sizeof(BMPInfoHeader));
	int width = header->width;
	if (width % 4) width += 4 - (width % 4); // piksele w bitmapie s¹ wyrównywane do 4 bajtów
	cout << "Zapis danych" << endl;
	for (int i = 0; i < header->height; i++)
		//for (int i = header->height-1; i >= 0; --i)
	{
		ofs.write((char*)pixs[i], 3 * width);
	}
	ofs.close();
}
