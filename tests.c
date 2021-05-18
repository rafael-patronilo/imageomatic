#include "Imageomatic.h"

void assertImageEq(Image result, Image expected, Int2 nr, Int2 ne){
    if(!int2Equals(nr, ne)){
        printf("Different sizes:\n");
        printf("    result = (%d, %d)\n", nr.x, nr.y);
        printf("    expected = (%d, %d)\n", ne.x, ne.y);
    }
    Int2 i;

    int errorCount = 0;
	for(i.x = 0; i.x < ne.x; i.x++)
	for(i.y = 0; i.y < ne.y; i.y++) {
        Pixel pr = result[i.x][i.y];
        Pixel pe = expected[i.x][i.y];
		if (!pixelEquals(pr, pe)){
            printf("Different pixel at (%d, %d):\n", i.x, i.y);
            printf("    result = (%d, %d, %d)\n", pr.red, pr.green, pr.blue);
            printf("    expected = (%d, %d, %d)\n", pe.red, pe.green, pe.blue);
            errorCount++;
            if(errorCount >= 10){
                printf("Over 10 different pixels, stopping.");
                return;
            }
        }
	}
}

void printHexCodes(unsigned char* chars, int n){
    for(int i = 0; i < n; i++){
        printf("0x%02x ", chars[i]);
    }
}

char decFromPixel(Pixel pixel){
    char c = 0;
    c = c | ((pixel.red & 0b11) << 4);
    c = c | ((pixel.green & 0b11) << 2);
    c = c | (pixel.blue & 0b11);
    if(c == 0) return '\0';
    else if (c <= 0x1f) return c + 0x40;
    else return c;
}

void decode(Image img, Int2 n, String out){
    Int2 i;
    for(i.x = 0; i.x < n.x; i.x++)
	for(i.y = 0; i.y < n.y; i.y++) {
        char c = decFromPixel(img[i.x][i.y]);
        *out = c;
        if(c == '\0') {
            return;
        }
        out++;
    }
}

void myImageTests(void)
{
	static Image img, img2, res, expected; 
	Int2 n, n_expected;
    #define test(name) \
    n_expected = imageLoad("expected/" name ".png", expected); \
    assertImageEq(res, expected, n, n_expected);

	// Q
    printf("Q - Grayscale:\n");
    n = imageLoad("img/frutos.png", img);
	n = imageGrayscale(img, n, res);
	imageStore("img/cinzento.png", res, n);
    test("gray")
	printf("\n\n");

	// N
    printf("N - Negative\n");
    n = imageLoad("img/frutos.png", img);
	n = imageNegative(img, n, res);
	imageStore("img/negativo.png", res, n);
    test("negativo")
    printf("\n\n");

	// H
    printf("H - Half\n");
    n = imageLoad("img/frutos.png", img);
	n = imageHalf(img, n, res);
	imageStore("img/metade.png", res, n);
    test("metade")
    printf("\n\n");

	// P
    printf("P - Paint\n");
	n = int2(512, 512);
	n = imagePaint("lime", n, res);
    test("pintar")
	imageStore("img/pintar.png", res, n);
    printf("\n\n");

	// R
    printf("R - Rotation\n");
	n = imageLoad("img/frutos.png", img);
	n = imageRotation90(img, n, res);
	imageStore("img/rotacao_90.png", res, n);
    test("rotacao_90")
    printf("\n\n");

	// O
    printf("O - Posterize\n");
	n = imageLoad("img/frutos.png", img);
	n = imagePosterize(img, n, 3, res);
	imageStore("img/poster.png", res, n);
    test("poster")
    printf("\n\n");

	// G
    printf("G - Droplet\n");
	n = int2(512, 512);
	n = imageDroplet(n, res);
	imageStore("img/goticula.png", res, n);
    test("goticula")
    printf("\n\n");

	// D
    printf("D - Blur\n");
	n = imageLoad("img/frutos.png", img);
	n = imageBlur(img, n, 5, res);
	imageStore("img/desfocado.png", res, n);
    test("desfocado")
    printf("\n\n");

	// M
    printf("M - Mask\n");
	n = imageLoad("img/frutos.png", img);
	n = imageDroplet(n, img2);
	n = imageMask(img, n, img2, n, res);
	imageStore("img/mascarar.png", res, n);
    test("mascarar")
    printf("\n\n");

	// F
    printf("F - Function(sin)\n");
	n = int2(512, 512);
	n = imageFunctionPlotting(sin, 50, n, res);
	imageStore("img/funcao.png", res, n);
    test("funcao")
    printf("\n\n");

	// T
    printf("T - Dithering\n");
	n = imageLoad("img/frutos.png", img);
	n = imageOrderedDithering(img, n, res);
	imageStore("img/matizacao.png", res, n);
    test("matizacao")
    printf("\n\n");

    String test;
	// E
    printf("E - Steganography\n");
	n = imageLoad("img/frutos.png", img);
	n = imageSteganography(img, n, "atacamos ao amanhecer", res);
    decode(res, n, test);
    printf("%s\n", test);
	imageStore("img/esteganografia.png", res, n);

    n = imageLoad("img/esteganografia.png", img);
    decode(img, n, test);
    printf("%s\n", test);

    n = int2(25, 4);
    n = imagePaint("ff00ff", n, img);
    n = imageSteganography(img, n, "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz :;<>=+*-_.,&%$#'\"!^[]()/\\? 0123456789 @ `~|{} opweiwoieiwheiuw", res);
    decode(res, n, test);
    printf("%s\n", test);
    printf("%s\n", strcmp(test, "ABCDEFGHIJKLMNOPQRSTUVWXYZ ABCDEFGHIJKLMNOPQRSTUVWXYZ :;<>=+*-_.,&%$#'\"!^[]()/\\? 0123456789 ? ?????") == 0 ? "positive" : "negative");
}