#include "stdafx.h"
#include "Encoder.h"
#include <iostream>
#include <fstream>
#include <clocale>
#include <bitset>
#include <algorithm>
#include <bitset> 
#include <vector>
#include <functional>

using namespace std;
Encoder::Encoder()
{

}
Encoder::Encoder(string plik, string plikDocelowy)
{
	this->plik = plik;
	this->plikDocelowy = plikDocelowy;
	cout << "Czytanie Pliku" << endl;
	int licznik = 0;
	int licznikKrok = 0;
	ifstream ifs(plik, ios::binary);
	char* temp = new char[sizeof(BMPFileHeader)];//tablica dynamiczna(na wskaznikach) bo nie znam rozmaru
	ifs.read(temp, sizeof(BMPFileHeader));
	bfh = (Encoder::BMPFileHeader*)(temp);//do zmiennej klasowej zapisuje temp
	if (bfh->bfType != 19778)//BM jako short
	{
		cout << "B³¹d nag³ówka <Typ>" << endl;
	}
	temp = new char[sizeof(BMPInfoHeader)];//nowa tablica w rozmiarze rozmiaru nag³ównka pliku Ÿród³owego
	ifs.read(temp, sizeof(BMPInfoHeader));
	bih = (BMPInfoHeader*)(temp);//do zmiennej klasowej zapisuje temp tylko dla fh a nie dla ih
	ifs.seekg(bfh->bfOffBits, ios::beg);
	//czytanie pixeli
	int width = bih->biWidth;
	if (width % 4) width += 4 - (width % 4); // piksele w bitmapie s¹ wyrównywane do 4 bajtów
	pixs = new Pixel*[bih->biHeight];
	for (int i = 0; i < bih->biHeight; i++)
	{
		temp = new char[3 * width];
		ifs.read(temp, 3 * width);
		pixs[i] = (Pixel*)(temp);
	}
	ifs.close();
}


Encoder::~Encoder()
{
}

char * Encoder::najblizszy(Pixel * x)
{
	char p = 0;
	if (x->r > 211)
		p = 3;
	else if (x->r > 127)
		p = 2;
	else if (x->r > 43)
		p = 1;
	p = p << 2;
	if (x->g > 211)
		p += 3;
	else if (x->g > 127)
		p += 2;
	else if (x->g > 43)
		p += 1;
	p = p << 1;
	if (x->b > 128)
		p += 1;
	return &p;
}

char * Encoder::najblizszySzary(Pixel * x)
{
	char p;
	p = (x->r*.299 + x->g*.587 + x->b*.114) / 8;
	return &p;
}

unsigned char Encoder::najblizszyR(unsigned char *r)
{
	if (*r > 211)
		return 3;
	else if (*r > 127)
		return 2;
	else if (*r > 43)
		return 1;
	return 0;
}

unsigned char Encoder::najblizszyG(unsigned char *g)
{
	if (*g > 211)
		return 3;
	else if (*g > 127)
		return 2;
	else if (*g > 43)
		return 1;
	return 0;
}

unsigned char Encoder::najblizszyB(unsigned char *b)
{
	if (*b > 128)
		return 1;
	return 0;
}

short * Encoder::rice(char x)
{
	short c = 0;
	for (char i = 0; i < (x >> 2); i++)
	{
		c++;
		c = c << 1;
	}
	c = c << 2;
	c += x & 3;
	return &c;
}

void Encoder::sortuj(char ** pixels, int w, int h)
{
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			++licznik[(short)pixels[y][x]];
		}
	}
	long long tymlicznik[32] = {};
	for (int i = 0; i < 32; i++)
	{
		tymlicznik[i] = licznik[i];
	}
	short a = 0;
	long long max = 0;
	for (int j = 0; j < 32; j++)
	{
		a = 0;
		max = -1;
		for (int i = 0; i < 32; i++)
		{
			if (tymlicznik[i] > max)
			{
				max = tymlicznik[i];
				a = i;
			}
		}
		tymlicznik[a] = -1;
		nowyLicznik[j] = a;
	}
}

void Encoder::kodujKolor()
{
	if (true)//konwersja na 5 bitów - kolor
	{
		unsigned char r = 0, g = 0, b = 0;
		pixels = new char*[bih->biHeight];
		for (int y = 0; y < bih->biHeight; y++)
		{
			pixels[y] = new char[bih->biWidth];
			for (int x = 0; x < bih->biWidth; x++)
			{
				pixels[y][x] = *najblizszy(&pixs[y][x]);
				if (true)//dithering
				{
					r = pixs[y][x].r - najblizszyR(&pixs[y][x].r);
					g = pixs[y][x].g - najblizszyG(&pixs[y][x].g);
					b = pixs[y][x].b - najblizszyB(&pixs[y][x].b);
					if (x + 1 < bih->biWidth)
					{
						if(pixs[y][x + 1].r + (r * 7 / 16) < 255 && pixs[y][x + 1].r + (r * 7 / 16) > 0)
							pixs[y][x + 1].r += (r * 7 / 16);
						if (pixs[y][x + 1].g + (g * 7 / 16) < 255 && pixs[y][x + 1].g + (g * 7 / 16) > 0)
							pixs[y][x + 1].g += (g * 7 / 16);
						if (pixs[y][x + 1].b + (b * 7 / 16) < 255 && pixs[y][x + 1].b + (b * 7 / 16) > 0)
							pixs[y][x + 1].b += (b * 7 / 16);
					}
					if (x - 1 >= 0 && y - 1 >= 0)
					{
						if (pixs[y - 1][x - 1].r + (r * 3 / 16) < 255 && pixs[y - 1][x - 1].r + (r * 3 / 16) > 0)
							pixs[y - 1][x - 1].r += (r * 3 / 16);
						if (pixs[y - 1][x - 1].g + (g * 3 / 16) < 255 && pixs[y - 1][x - 1].g + (g * 3 / 16) > 0)
							pixs[y - 1][x - 1].g += (g * 3 / 16);
						if (pixs[y - 1][x - 1].b + (b * 3 / 16) < 255 && pixs[y - 1][x - 1].b + (b * 3 / 16) > 0)
							pixs[y - 1][x - 1].b += (b * 3 / 16);
					}
					if (y - 1 >= 0)
					{
						if (pixs[y - 1][x].r + (r * 5 / 16) < 255 && pixs[y - 1][x].r + (r * 5 / 16) > 0)
							pixs[y - 1][x].r += (r * 5 / 16);
						if (pixs[y - 1][x].g + (g * 5 / 16) < 255 && pixs[y - 1][x].g + (g * 5 / 16) > 0)
							pixs[y - 1][x].g += (g * 5 / 16);
						if (pixs[y - 1][x].b + (b * 5 / 16) < 255 && pixs[y - 1][x].b + (b * 5 / 16) > 0)
							pixs[y - 1][x].b += (b * 5 / 16);
					}
					if (x + 1 < bih->biWidth && y - 1 >= 0) 
					{
						if (pixs[y - 1][x + 1].r + (r * 1 / 16) < 255 && pixs[y - 1][x + 1].r + (r * 1 / 16) > 0)
							pixs[y - 1][x + 1].r += (r * 1 / 16);
						if (pixs[y - 1][x + 1].g + (g * 1 / 16) < 255 && pixs[y - 1][x + 1].g + (g * 1 / 16) > 0)
							pixs[y - 1][x + 1].g += (g * 1 / 16);
						if (pixs[y - 1][x + 1].b + (b * 1 / 16) < 255 && pixs[y - 1][x + 1].b + (b * 1 / 16) > 0)
							pixs[y - 1][x + 1].b += (b * 1 / 16);
					}
				}
			}
		}
	}
	sortuj(pixels, bih->biWidth, bih->biHeight);
	char nowyLicznikPrzeciwny[32] = {};
	for (char i = 0; i < 32; i++)
	{
		nowyLicznikPrzeciwny[nowyLicznik[i]] = i;
	}
	vector<bool> data;
	if (true)//kodowanie danych przy pomocy zakodowanej palety
	{
		cout << "Kodowanie danych" << endl;
		bitset<10> bit;
		bool flag;
		//for (int y = 0; y < bih->biHeight; ++y)
		for (int y = bih->biHeight - 1; y >= 0; --y)
		{
			//for (int x = 0; x < bih->biWidth;++x)
			for (int x = bih->biWidth - 1; x >= 0;--x)
			{
				pixels[y][x] = nowyLicznikPrzeciwny[pixels[y][x]];
				//stworzenie danych bitowych
				bit = bitset<10>(*rice(pixels[y][x]));
				flag = false;
				for (char i = 9; i >= 0; i--)
				{
					if (bit.test(i) == true || i <= 2)
						flag = true;
					if (flag)
						data.push_back(bit.test(i));
				}
				//koniec = stworzenie danych bitowych
			}
		}
	}
	MBAHeader header;
	header.type[0] = 'M';
	header.type[1] = 'B';
	header.type[2] = 'A';
	header.width = bih->biWidth;
	header.height = bih->biHeight;
	header.data = 8 - data.size() % 8;
	long long size = (data.size() / 8 + (data.size() % 8 ? 1 : 0) + 17 + 20);
	header.size[4] = size % 256;
	size >>= 8;
	header.size[3] = size % 256;
	size >>= 8;
	header.size[2] = size % 256;
	size >>= 8;
	header.size[1] = size % 256;
	size >>= 8;
	header.size[0] = size % 256;
	size >>= 8;
	ofstream ofs(plikDocelowy, ios::binary);
	ofs.write((char*)&header, sizeof(MBAHeader));
	cout << "Zapis do pliku" << endl;
	char c = 0;
	vector<bool> data2;
	for (char i = 0;i < 32;++i)
	{
		bitset<5> pal = bitset<5>(nowyLicznik[i]);
		for (char x = 4; x >= 0; --x)
		{
			data2.push_back(pal.test(x));

		}
	}
	while (data2.size() >= 8)
	{
		c = 0;
		for (char i = 0; i < 8;++i)
		{
			c = c << 1;
			c += (data2.back() ? 1 : 0);
			data2.pop_back();
		}
		ofs.write((char*)&c, sizeof(c));
	}
	cout << "size: "<< data.size() << endl;
	long long i = 0;
	long long ii = data.size();
	ii -= ii % 8;
	while (ii > i)
	{
		c = 0;
		for (char x = 0; x < 8;++x)
		{
			c = c << 1;
			c += (data.at(i) ? 1 : 0);
			++i;
		}
		ofs.write((char*)&c, sizeof(c));
	}
	c = 0;
	short z = 0;
	while (data.size() - i > 0)
	{
		c = c << 1;
		c += (data.at(i) ? 1 : 0);
		++i;
		++z;
	}
	c << 8 - z;
	ofs.write((char*)&c, sizeof(c));
	ofs.close();
}

void Encoder::kodujSzary()
{
	if (true)//konwersja na 5 bitów - skala szaroœci
	{
		unsigned char r = 0, g = 0, b = 0;
		pixels = new char*[bih->biHeight];
		for (int y = 0; y < bih->biHeight; y++)
		{
			pixels[y] = new char[bih->biWidth];
			for (int x = 0; x < bih->biWidth; x++)
			{
				pixels[y][x] = *najblizszySzary(&pixs[y][x]);
				if (true)//dithering
				{
					r = pixs[y][x].r - pixels[y][x];
					g = pixs[y][x].g - pixels[y][x];
					b = pixs[y][x].b - pixels[y][x];
					if (x + 1 < bih->biWidth)
					{
						if (pixs[y][x + 1].r + (r * 7 / 16) < 255 && pixs[y][x + 1].r + (r * 7 / 16) > 0)
							pixs[y][x + 1].r += (r * 7 / 16);
						if (pixs[y][x + 1].g + (g * 7 / 16) < 255 && pixs[y][x + 1].g + (g * 7 / 16) > 0)
							pixs[y][x + 1].g += (g * 7 / 16);
						if (pixs[y][x + 1].b + (b * 7 / 16) < 255 && pixs[y][x + 1].b + (b * 7 / 16) > 0)
							pixs[y][x + 1].b += (b * 7 / 16);
					}
					if (x - 1 >= 0 && y - 1 >= 0)
					{
						if (pixs[y - 1][x - 1].r + (r * 3 / 16) < 255 && pixs[y - 1][x - 1].r + (r * 3 / 16) > 0)
							pixs[y - 1][x - 1].r += (r * 3 / 16);
						if (pixs[y - 1][x - 1].g + (g * 3 / 16) < 255 && pixs[y - 1][x - 1].g + (g * 3 / 16) > 0)
							pixs[y - 1][x - 1].g += (g * 3 / 16);
						if (pixs[y - 1][x - 1].b + (b * 3 / 16) < 255 && pixs[y - 1][x - 1].b + (b * 3 / 16) > 0)
							pixs[y - 1][x - 1].b += (b * 3 / 16);
					}
					if (y - 1 >= 0)
					{
						if (pixs[y - 1][x].r + (r * 5 / 16) < 255 && pixs[y - 1][x].r + (r * 5 / 16) > 0)
							pixs[y - 1][x].r += (r * 5 / 16);
						if (pixs[y - 1][x].g + (g * 5 / 16) < 255 && pixs[y - 1][x].g + (g * 5 / 16) > 0)
							pixs[y - 1][x].g += (g * 5 / 16);
						if (pixs[y - 1][x].b + (b * 5 / 16) < 255 && pixs[y - 1][x].b + (b * 5 / 16) > 0)
							pixs[y - 1][x].b += (b * 5 / 16);
					}
					if (x + 1 < bih->biWidth && y - 1 >= 0)
					{
						if (pixs[y - 1][x + 1].r + (r * 1 / 16) < 255 && pixs[y - 1][x + 1].r + (r * 1 / 16) > 0)
							pixs[y - 1][x + 1].r += (r * 1 / 16);
						if (pixs[y - 1][x + 1].g + (g * 1 / 16) < 255 && pixs[y - 1][x + 1].g + (g * 1 / 16) > 0)
							pixs[y - 1][x + 1].g += (g * 1 / 16);
						if (pixs[y - 1][x + 1].b + (b * 1 / 16) < 255 && pixs[y - 1][x + 1].b + (b * 1 / 16) > 0)
							pixs[y - 1][x + 1].b += (b * 1 / 16);
					}
				}
			}
		}
	}
	sortuj(pixels, bih->biWidth, bih->biHeight);
	char nowyLicznikPrzeciwny[32] = {};
	for (char i = 0; i < 32; i++)
	{
		nowyLicznikPrzeciwny[nowyLicznik[i]] = i;
	}
	vector<bool> data;
	if (true)//kodowanie danych przy pomocy zakodowanej palety
	{
		cout << "Kodowanie danych" << endl;
		bitset<10> bit;
		bool flag;
		//for (int y = 0; y < bih->biHeight; ++y)
		for (int y = bih->biHeight - 1; y >= 0; --y)
		{
			//for (int x = 0; x < bih->biWidth;++x)
			for (int x = bih->biWidth - 1; x >= 0;--x)
			{
				pixels[y][x] = nowyLicznikPrzeciwny[pixels[y][x]];
				//stworzenie danych bitowych
				bit = bitset<10>(*rice(pixels[y][x]));
				flag = false;
				for (char i = 9; i >= 0; i--)
				{
					if (bit.test(i) == true || i <= 2)
						flag = true;
					if (flag)
						data.push_back(bit.test(i));
				}
				//koniec = stworzenie danych bitowych
			}
		}
	}
	MBAHeader header;
	header.type[0] = 'A';
	header.type[1] = 'B';
	header.type[2] = 'M';
	header.width = bih->biWidth;
	header.height = bih->biHeight;
	header.data = 8 - data.size() % 8;
	long long size = (data.size() / 8 + (data.size() % 8 ? 1 : 0) + 17 + 20);
	header.size[4] = size % 256;
	size >>= 8;
	header.size[3] = size % 256;
	size >>= 8;
	header.size[2] = size % 256;
	size >>= 8;
	header.size[1] = size % 256;
	size >>= 8;
	header.size[0] = size % 256;
	size >>= 8;
	ofstream ofs(plikDocelowy, ios::binary);
	ofs.write((char*)&header, sizeof(MBAHeader));
	cout << "Zapis do pliku" << endl;
	char c = 0;
	vector<bool> data2;
	for (char i = 0;i < 32;++i)
	{
		bitset<5> pal = bitset<5>(nowyLicznik[i]);
		for (char x = 4; x >= 0; --x)
		{
			data2.push_back(pal.test(x));

		}
	}
	while (data2.size() >= 8)
	{
		c = 0;
		for (char i = 0; i < 8;++i)
		{
			c = c << 1;
			c += (data2.back() ? 1 : 0);
			data2.pop_back();
		}
		ofs.write((char*)&c, sizeof(c));
	}
	cout << "size: " << data.size() << endl;
	long long i = 0;
	long long ii = data.size();
	ii -= ii % 8;
	while (ii > i)
	{
		c = 0;
		for (char x = 0; x < 8;++x)
		{
			c = c << 1;
			c += (data.at(i) ? 1 : 0);
			++i;
		}
		ofs.write((char*)&c, sizeof(c));
	}
	c = 0;
	short z = 0;
	while (data.size() - i > 0)
	{
		c = c << 1;
		c += (data.at(i) ? 1 : 0);
		++i;
		++z;
	}
	c << 8 - z;
	ofs.write((char*)&c, sizeof(c));
	ofs.close();
}

void Encoder::kodujPaleta()
{
	if (true)//konwersja na 5 bitów - paleta
	{
		oct_node *root = new oct_node;
		root->index = 0;
		root->liczba = 0;
		for (int i = 0; i < 8; ++i)//1
		{
			oct_node *child = new oct_node;
			child->poziom = 1;
			child->index = i;
			child->parent = root;
			child->liczba = 0;
			for (int j = 0; j < 8; ++j)//2
			{
				oct_node *child2 = new oct_node;
				child2->poziom = 2;
				child2->index = j;
				child2->parent = child;
				child2->liczba = 0;
				for (int k = 0; k < 8; ++k)//3
				{
					oct_node *child3 = new oct_node;
					child3->poziom = 3;
					child3->index = k;
					child3->parent = child2;
					child3->liczba = 0;
					for (int l = 0; l < 8; ++l)//4
					{
						oct_node *child4 = new oct_node;
						child4->poziom = 3;
						child4->index = k;
						child4->parent = child3;
						child4->liczba = 0;
						child3->kids[l] = child4;
					}
					child2->kids[k] = child3;
				}
				child->kids[j] = child2;
			}
			root->kids[i] = child;
		}
		unsigned char c = 0;
		for (int y = 0; y < bih->biHeight; y++)
		{
			for (int x = 0; x < bih->biWidth; x++)
			{
				c = pixs[y][x].r & 128 >> 5;
				c = c + (pixs[y][x].g & 128 >> 6);
				c = c + (pixs[y][x].b & 128 >> 7);
				for (int i = 0; i < 8; ++i)
				{ 
					if (root->kids[i]->index == c)
					{
						++(root->kids[i]->liczba);
						c = pixs[y][x].r & 64 >> 4;
						c = c + (pixs[y][x].g & 64 >> 5);
						c = c + (pixs[y][x].b & 64 >> 6);
						for (int j = 0; j < 8; ++j)
						{
							if (root->kids[i]->kids[j]->index == c)
							{
								++(root->kids[i]->kids[j]->liczba);
								c = pixs[y][x].r & 32 >> 3;
								c = c + (pixs[y][x].g & 32 >> 4);
								c = c + (pixs[y][x].b & 32 >> 5);
								for (int k = 0; k < 8; ++k)
								{
									if (root->kids[i]->kids[j]->kids[k]->index == c)
									{
										++(root->kids[i]->kids[j]->kids[k]->liczba);
										c = pixs[y][x].r & 16 >> 2;
										c = c + (pixs[y][x].g & 16 >> 3);
										c = c + (pixs[y][x].b & 16 >> 4);
										for (int l = 0; l < 8; ++l)
										{
											if (root->kids[i]->kids[j]->kids[k]->kids[l]->index == c)
											{
												++(root->kids[i]->kids[j]->kids[k]->kids[l]->liczba);
												l = 8;
											}
										}
										k = 8;
									}
								}
								j = 8;
							}
						}
						i = 8;
					}
				}
			}
		}
		vector<oct_node> pal;
		for (int i = 0; i < 8; ++i)//1
		{
			for (int j = 0; j < 8; ++j)//2
			{
				for (int k = 0; k < 8; ++k)//3
				{
					for (int l = 0; l < 8; ++l)//4
					{
						if (pal.size() < 32)
						{
							pal.push_back(*root->kids[i]->kids[j]->kids[k]->kids[l]);
							sort(pal.begin(), pal.end(), greater<oct_node>());
						}
						else if (pal.at(31).liczba < root->kids[i]->kids[j]->kids[k]->kids[l]->liczba)
						{
							pal.pop_back();
							pal.push_back(*root->kids[i]->kids[j]->kids[k]->kids[l]);
							sort(pal.begin(), pal.end(), greater<oct_node>());
						}
					}
				}
			}
		}
		for (int i = 0;i < 32;++i)
		{
			cout << (short)pal.at(i).index << ": " << pal.at(i).liczba << endl;
		}
	}
}

void Encoder::oct_node::usunKids()
{
	for (int i = 0; i < 8; ++i)
	{
		if (kids[i] != NULL)
		{
			kids[i]->usunKids();
			delete kids[i];
		}
		kids[i] = NULL;
	}
}
