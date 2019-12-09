#include "main.h"
#define LAYER_WIDTH 25
#define LAYER_HEIGTH 6
#define LAYER_SIZE (LAYER_WIDTH * LAYER_HEIGTH)

#define RemderedPixel(pixelIndex) (RenderedImage[pixelIndex / LAYER_WIDTH][pixelIndex % LAYER_WIDTH])

CREATE_TEST(8)
{
	FILE *file = fopen("day8.txt", "r");

	int digit, count = 0;
	int PixelCount[200][3]{};
	int RenderedImage[LAYER_HEIGTH][LAYER_WIDTH];
	std::fill(*PixelCount, *PixelCount + 200 * 3, 0);
	std::fill(*RenderedImage, *RenderedImage + LAYER_WIDTH * LAYER_HEIGTH, 2);

	while (fscanf(file, "%1d", &digit) == 1)
	{
		int layer = count / LAYER_SIZE;
		int pixelIndex = count % LAYER_SIZE;

		PixelCount[layer][digit]++;
		RemderedPixel(pixelIndex) = RemderedPixel(pixelIndex) == 2 ? digit : RemderedPixel(pixelIndex);

		count++;
	}

	int min0Digits = INT_MAX, part1Value = INT_MAX;
	for (int index = 0; index < count / LAYER_SIZE; ++index)
	{
		if (min0Digits > PixelCount[index][0])
		{
			min0Digits = PixelCount[index][0];
			part1Value = PixelCount[index][1] * PixelCount[index][2];
		}
	}
	cout << part1Value<<endl;

	for (int index1 = 0; index1 < LAYER_HEIGTH; ++index1)
	{
		for (int index2 = 0; index2 < LAYER_WIDTH; ++index2)
		{
			cout << (RenderedImage[index1][index2] == 1 ? "*" : " ");
		}
		cout << endl;
	}
}
