/*
	Linguagens e Ambientes de Programação - Projeto de 2020/2021

	Imageomatic module body

largura maxima = 100 colunas
tab = 4 espaços
0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

	Este ficheiro constitui apenas um ponto de partida para o
	seu trabalho. Todo este ficheiro pode e deve ser alterado
	à vontade, a começar por este comentário.


 IDENTIFICAÇÃO DOS AUTORES -
	Aluno 1: numero, nome
	Aluno 2: numero, nome

Comentarios:

?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????

*/

#include "Imageomatic.h"

double scaleComponent01(int component){
	return ((double)component) / MAX_COLOR;
}

// convert ASCII character to 6 bit ASCII
char encodeChar(char c){
	// capitalize
	if(c >= 'a' && c <= 'z'){
			c += 'A' - 'a';
	}
	// replace out of bounds and special characters
	if (c == '\0'){
		c = '@'; // '@' = 0b100_0000
	}
	else if (c < 0x20 || c > 0x5F || c == '@'){
		c = '?';
	}
	// select the 6 less significant bits
	return c & 0x3F; // 0x3F = 0b011_1111
}

// convert ASCII string to 6 bit ASCII
void encode(String from, String to, int maxSize){
	int i = 0;
	while(from[i] != '\0'){ 
		if(i == maxSize - 1){
			break;
		}
		to[i] = encodeChar(from[i]);
		i++;
	}
	to[i] = 0;
}

Pixel hideInPixel(Pixel pixel, char c){
	pixel.red = (pixel.red & 0xFC) | (c >> 4);
	pixel.green = (pixel.green & 0xFC) | ((c & 0xC) >> 2);
	pixel.blue = (pixel.blue & 0xFC) | (c & 0x3);
	return pixel;
}

#define abs(a) a < 0 ? -a : a

/*** TYPE Int2 ***/

/* More Int2 functions, in case you need them */

Int2 int2Sub(Int2 a, Int2 b){
	return int2(a.x - b.x, a.y - b.y);
}


/*** TYPE Pixel ***/

/* More Pixel functions, in case you need them */

Pixel parseColor(String color) {
	unsigned int red, green, blue;
	char discard; //Required to catch a trailing character in the case of a string like fffffz
	// Check if the input color is a valid hexadecimal code
	if(sscanf(color, "%02x%02x%02x%c", &red, &green, &blue, &discard) != 3){
		// Not an hexadecimal code, look for the name in colorsFileName
		FILE * colors = fopen(colorsFileName, "r");
		String fcolor;
		bool found = false;
		while(!found && fscanf(colors, "%02x%02x%02x %s\n", &red, &green, &blue, fcolor) == 4){
			if(strcmp(color, fcolor) == 0){
				found = true;
			}
		}
		fclose(colors);
		// Not in colorsFileName, set to black
		if(!found){
			red = green = blue = 0;
		}
	}
	return pixel(red, green, blue);
}

Pixel blurPixel(Int2 target, int level, Image source, Int2 n){
	Int2 level2 = int2(level, level);
	Int2 start = int2Max(int2Sub(target, level2), int2Zero);
	Int2 end = int2Min(int2Add(target, level2), int2Sub(n, int2(1, 1)));
	Int2 i;
	int red = 0, green = 0, blue = 0;
	for(i.x = start.x; i.x <= end.x; i.x++)
	for(i.y = start.y; i.y <= end.y; i.y++) {
		Pixel pixel = source[i.x][i.y];
		red += pixel.red;
		green += pixel.green;
		blue += pixel.blue;
	}
	int count = (end.x - start.x + 1) * (end.y - start.y + 1);
	red = red / count;
	green = green / count;
	blue = blue / count;
	return pixel(red, green, blue);
}

void fill(Pixel color, Int2 n, Image res){
	Int2 i;
	for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
		res[i.x][i.y] = color;
	}
}

/*** TYPE Image ***/

void initialization(void)
{
	// This function is automatically called when the interpreter starts.
	// If you need to perform some initialization, this is the place
	// to write the initialization code.
}

Int2 imageCopy(Image img, Int2 n, Image res)
{
	Int2 i;
	for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
		res[i.x][i.y] = img[i.x][i.y];
	}
	return n;
}

Int2 imagePaint(String cor, Int2 n, Image res)
{
	Pixel color = parseColor(cor);
	fill(color, n, res);
	return n;
}

Int2 imageNegative(Image img, Int2 n, Image res)
{
	Int2 i;
	for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
		Pixel pixel = img[i.x][i.y];
		pixel.red = MAX_COLOR - pixel.red;
		pixel.green = MAX_COLOR - pixel.green;
		pixel.blue = MAX_COLOR - pixel.blue;
		res[i.x][i.y] = pixel;
	}
	return n;
}

Int2 imageDroplet(Int2 n, Image res)
{
	Int2 i;
	Int2 center = int2Half(n);
	for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
		Pixel pixel;
		double dist = int2Distance(i, center);
		pixel.red = pixel.green = pixel.blue = 0.7* MAX_COLOR + 0.3 * sin(dist / 20) * MAX_COLOR;
		res[i.x][i.y] = pixel;
	}
	return n;
}

Int2 imageMask(Image img1, Int2 n1, Image img2, Int2 n2, Image res) // pre: int2Equals(n1, n2)
{
	Int2 i;
	for(i.x = 0; i.x < n1.x; i.x++)
	for(i.y = 0; i.y < n1.y; i.y++) {
		Pixel pixel = img1[i.x][i.y];
		Pixel pixel2 = img2[i.x][i.y];
		pixel.red = pixel.red * scaleComponent01(pixel2.red);
		pixel.green = pixel.green * scaleComponent01(pixel2.green);
		pixel.blue = pixel.blue * scaleComponent01(pixel2.blue);
		res[i.x][i.y] = pixel;
	}
	return n1;
}

Int2 imageGrayscale(Image img, Int2 n, Image res)
{
	Int2 i;
	for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
		int value = pixelGrayAverage(img[i.x][i.y]);
		res[i.x][i.y] = pixelGray(value);
	}
	return n;
}

Int2 imageBlur(Image img, Int2 n, int nivel, Image res)
{
	Int2 i;
	for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
		res[i.x][i.y] = blurPixel(i, nivel, img, n);
	}
	return n;
}

Int2 imageRotation90(Image img, Int2 n, Image res)
{
	Int2 i;
	for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
		res[n.y - i.y - 1][i.x] = img[i.x][i.y];
	}
	return int2(n.y, n.x);
}

Int2 imagePosterize(Image img, Int2 n, int factor, Image res)
{
	int interval = 1 << (8 - factor);
	Int2 i;
	for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
		Pixel pixel = img[i.x][i.y];
		pixel.red = (pixel.red / interval) * interval;
		pixel.green = (pixel.green / interval) * interval;
		pixel.blue = (pixel.blue / interval) * interval;
		res[i.x][i.y] = pixel;
	}
	return n;
}

Int2 imageHalf(Image img, Int2 n, Image res)
{
	Int2 i;
	for(i.x = 0; i.x < n.x; i.x+=2)
	for(i.y = 0; i.y < n.y; i.y+=2) {
		res[i.x / 2][i.y / 2] = img[i.x][i.y];
	}
	return int2Half(n);
}

Int2 imageFunctionPlotting(DoubleFun fun, int scale, Int2 n, Image res)
{
	fill(white, n, res);
	Int2 center = int2Half(n);
	for(int x = 0; x < n.x; x++){
		int y = center.y - (fun((double)(x - center.x)/scale) * scale);
		//ensure within bounds
		if(y >= 0 && y < n.y)
			res[x][y] = black;
		res[x][center.y] = black;
	}
	for(int y = 0; y < n.y; y++) res[center.x][y] = black;
	return n;
}

Int2 imageOrderedDithering(Image img, Int2 n, Image res)
{
	#define INDEX_SIDE  8
	Byte indexMatrix[INDEX_SIDE][INDEX_SIDE] = {
					{ 0, 32,  8, 40,  2, 34, 10, 42}, 
					{48, 16, 56, 24, 50, 18, 58, 26},
					{12, 44,  4, 36, 14, 46,  6, 28},
					{60, 28, 52, 20, 62, 30, 54, 22},
					{ 3, 35, 11, 43,  1, 33,  9, 41},
					{51, 19, 59, 27, 49, 17, 57, 25},
					{15, 47,  7, 39, 13, 45,  5, 37},
					{63, 31, 55, 23, 61, 29, 53, 21}
			};
	Int2 i;
	for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
		Byte threshold = indexMatrix[i.x % INDEX_SIDE][i.y % INDEX_SIDE];
		double intensity = pixelGrayAverage(img[i.x][i.y]) / 4.0;
		if(intensity > threshold){
			res[i.x][i.y] = white;
		}
		else{
			res[i.x][i.y] = black;
		}
	}
	return n;
}

Int2 imageSteganography(Image img, Int2 n, String s, Image res)
{
	String encoded;
	char* pointer = encoded;
	bool finished = false;
	encode(s, encoded, n.x * n.y);
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		if(*pointer != '\0'){
			res[i.x][i.y] = hideInPixel(img[i.x][i.y], *pointer);
			pointer++;
		}
		else if (!finished){
			res[i.x][i.y] = hideInPixel(img[i.x][i.y], 0);
			finished = true;
		}
		else{
			res[i.x][i.y] = img[i.x][i.y];
		}
	}
	return n;
}


void imageTests(void)
{
	static Image img, img2, res;
	Int2 n;

	// Q
	n = imageLoad("img/frutos.png", img);
	n = imageGrayscale(img, n, res);
	imageStore("img/cinzento.png", res, n);

	// N
	n = imageLoad("img/frutos.png", img);
	n = imageNegative(img, n, res);
	imageStore("img/negativo.png", res, n);

	// H
	n = imageLoad("img/frutos.png", img);
	n = imageHalf(img, n, res);
	imageStore("img/metade.png", res, n);
	// P
	n = int2(512, 512);
	n = imagePaint("green", n, res);
	imageStore("img/pintar.png", res, n);

	// R
	n = imageLoad("img/frutos.png", img);
	n = imageRotation90(img, n, res);
	imageStore("img/rotacao_90.png", res, n);

	// O
	n = imageLoad("img/frutos.png", img);
	n = imagePosterize(img, n, 3, res);
	imageStore("img/poster.png", res, n);

	// G
	n = int2(512, 512);
	n = imageDroplet(n, res);
	imageStore("img/goticula.png", res, n);

	// D
	n = imageLoad("img/frutos.png", img);
	n = imageBlur(img, n, 5, res);
	imageStore("img/desfocado.png", res, n);

	// M
	n = imageLoad("img/frutos.png", img);
	n = imageDroplet(n, img2);
	n = imageMask(img, n, img2, n, res);
	imageStore("img/mascarar.png", res, n);

	// F
	n = int2(512, 512);
	n = imageFunctionPlotting(sin, 50, n, res);
	imageStore("img/funcao.png", res, n);

	// T
	n = imageLoad("img/frutos.png", img);
	n = imageOrderedDithering(img, n, res);
	imageStore("img/matizacao.png", res, n);

	// E
	n = imageLoad("img/frutos.png", img);
	n = imageSteganography(img, n, "atacamos ao amanhecer", res);
	imageStore("img/esteganografia.png", res, n);
}

