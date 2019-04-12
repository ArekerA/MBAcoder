# MBAcoder

[English](#english)
[Polski](#polski)

[Screenshots](#screenshots)

## English
Student's project. An application that uses its own format to save graphic files. Includes encoder and decoder for BMP files.

#### General description:

MBA is an image compression format. It uses Rice's compression algorithm. It supports two work modes:
* 32-degree gray scale
* 32 colors

One pixel of the image uses 5 data bits. The format saves the lines of the image from the bottom up, just like it was done for files with the extension .bmp. The MBA format files have the .mba extension.

#### File header:
* 3 bytes
File identification:
MBA for color files,
ABM for monochrome files,
* 5 bytes
File size in bytes.
* 4 bytes
The height of the image.
* 4 bytes
The width of the image.
* 1 byte
The number of redundant data.
* 20 bytes
Sorted alphabet of colors.

#### Data:
After the header, data is stored on 5 bits per pixel and coded with Rice code.
Rice coding specification:
* message alphabet: range of numbers [0,31];
* code base: M = 4;
* The input data is sorted by the frequency of occurrence. Sorted colors are sent with the file.

#### 32-degree gray scale

In the case of the gray scale, the color is converted using the formula:

<dd> BW = 0.299 * R + 0.587 * G + 0.114 * B </dd>

In this way, we get 8 bits of data, which we transform using the method of searching for the nearest neighbor to 5 bits.
Dithering is also included using the Floyd-Steinberg algorithm.

Color palette

value | RGB
--- | ---
0 | 000000
1 | 080808
2 | 101010
3 | 191919
4 | 212121
5 | 292929
6 | 313131
7 | 3a3a3a
8 | 424242
9 | 4a4a4a
10 | 525252
11 | 5a5a5a
12 | 636363
13 | 6b6b6b
14 | 737373
15 | 7b7b7b
16 | 848484
17 | 8c8c8c
18 | 949494
19 | 9c9c9c
20 | a5a5a5
21 | adadad
22 | b5b5b5
23 | bdbdbd
24 | c5c5c5
25 | cecece
26 | d6d6d6
27 | dedede
28 | e6e6e6
29 | efefef
30 | f7f7f7
31 | ffffff

#### 32 colors

The color image uses the method of searching for the nearest neighbor to receive the color in the RRGGB format.
In this case, as well as before, dithering was included using the Floyd-Steinberg algorithm.

Color palette

value | RGB
--- | ---
0 | 000000
1 | 0000ff
2 | 005500
3 | 0055ff
4 | 00aa00
5 | 00aaff
6 | 00ff00
7 | 00ffff
8 | 550000
9 | 5500ff
10 | 555500
11 | 5555ff
12 | 55aa00
13 | 55aaff
14 | 55ff00
15 | 55ffff
16 | aa0000
17 | aa00ff
18 | aa5500
19 | aa55ff
20 | aaaa00
21 | aaaaff
22 | aaff00
23 | aaffff
24 | ff0000
25 | ff00ff
26 | ff5500
27 | ff55ff
28 | ffaa00
29 | ffaaff
30 | ffff00
31 | ffffff

## Polski
Projekt studencki. Aplikacja wykorzystująca własny format do zapisów plików graficznych. Zawiera koder i dekoder do plków BMP.

#### Opis ogólny:

MBA jest to format kompresji obrazu. Zastosowano w nim algorytm kompresji Rice’a. Obsługuje dwa tryby pracy:
* 32-stopniowa skala szarości
* 32 kolory

Jeden pixel obrazu wykorzystuje 5 bitów danych. Format zapisuje linie obrazu od dołu do góry, podobnie jak zostało to zrealizowane w przypadku plików z rozszerzeniem .bmp. Pliki formatu MBA posiadają rozszerzenie .mba.

#### Nagłówek pliku:
* 3 bajty
Identyfikacja pliku:
MBA dla plików kolorowych,
ABM dla plików monochromatycznych,
BAM dla plików z paletą.
* 5 bajtów
Rozmiar pliku w bajtach.
* 4 bajty
Wysokość obrazu.
* 4 bajty
Szerokość obrazu.
* 1 bajt
Liczba danych nadmiarowych.
* 20 bajtów
Posortowany alfabet kolorów.

#### Dane:
Po nagłówku występują dane zapisane są na 5 bitach na pixel i kodowane kodem Rice’a.
Specyfikacja kodowanie Rice’a:
* alfabet wiadomości: zakres liczb [0,31];
* baza kodu: M=4;
* Dane wejściowe sortowane są nierosnąco po częstotliwości występowania. Posortowane barwy przesyłane są wraz z plikiem.

#### 32-stopniowa skala szarości

W przypadku skali szarości barwa konwertowana jest przy użyciu wzoru:

<dd>BW = 0,299*R + 0,587*G + 0,114*B</dd>

W ten sposób otrzymamy 8 bitów danych, które przekształcamy przy pomocy metody poszukiwania najbliższego sąsiada na 5 bitów.
Zastosowany jest również dithering przy użyciu algorytmu Floyda-Steinberga.

Paleta kolorów

wartość | RGB
--- | ---
0 | 000000
1 | 080808
2 | 101010
3 | 191919
4 | 212121
5 | 292929
6 | 313131
7 | 3a3a3a
8 | 424242
9 | 4a4a4a
10 | 525252
11 | 5a5a5a
12 | 636363
13 | 6b6b6b
14 | 737373
15 | 7b7b7b
16 | 848484
17 | 8c8c8c
18 | 949494
19 | 9c9c9c
20 | a5a5a5
21 | adadad
22 | b5b5b5
23 | bdbdbd
24 | c5c5c5
25 | cecece
26 | d6d6d6
27 | dedede
28 | e6e6e6
29 | efefef
30 | f7f7f7
31 | ffffff

#### 32 kolory

Kolorowy obraz  wykorzystuje metodę poszukiwania najbliższego sąsiada do otrzymania barwy w formacie RRGGB.
W tym przypadku, tak jak również poprzednio, wykorzystano dithering przy użyciu algorytmu Floyda-Steinberga.

Paleta kolorów

wartość | RGB
--- | ---
0 | 000000
1 | 0000ff
2 | 005500
3 | 0055ff
4 | 00aa00
5 | 00aaff
6 | 00ff00
7 | 00ffff
8 | 550000
9 | 5500ff
10 | 555500
11 | 5555ff
12 | 55aa00
13 | 55aaff
14 | 55ff00
15 | 55ffff
16 | aa0000
17 | aa00ff
18 | aa5500
19 | aa55ff
20 | aaaa00
21 | aaaaff
22 | aaff00
23 | aaffff
24 | ff0000
25 | ff00ff
26 | ff5500
27 | ff55ff
28 | ffaa00
29 | ffaaff
30 | ffff00
31 | ffffff



## Screenshots

<img width="200" alt="image_1" src="/readme-images/1.png"> <img width="200" alt="image_1k" src="/readme-images/1k.png"> <img width="200" alt="image_1s" src="/readme-images/1s.png">

<img width="200" alt="image_2" src="/readme-images/2.png"> <img width="200" alt="image_2k" src="/readme-images/2k.png"> <img width="200" alt="image_2s" src="/readme-images/2s.png">

<img width="200" alt="image_3" src="/readme-images/3.png"> <img width="200" alt="image_3k" src="/readme-images/3k.png"> <img width="200" alt="image_3s" src="/readme-images/3s.png">

<img width="200" alt="image_4" src="/readme-images/4.png"> <img width="200" alt="image_4k" src="/readme-images/4ak.png"> <img width="200" alt="image_4s" src="/readme-images/4as.png">
