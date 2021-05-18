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

	// E
    printf("E - Steganography\n");
	n = imageLoad("img/frutos.png", img);
	n = imageSteganography(img, n, "atacamos ao amanhecer", res);
	imageStore("img/esteganografia.png", res, n);
}