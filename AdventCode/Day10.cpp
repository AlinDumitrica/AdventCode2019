#include "main.h"
#define ASTEROID '#'
#define SPACE '.'

#define WIDTH 23
#define MAP_SIZE 1024
#define PI 3.14159265359
float GetAngle(int index1, int index2, int width)
{
	int x1 = index1 % width; int y1 = index1 / width;
	int x2 = index2 % width; int y2 = index2 / width;

	float angle = (float)atan2(y2 - y1, x2 - x1) - 1.5707963;
	//float angle = (float)atan2(x2 - x1, y2 - y1);
	return angle < 0 ? angle + PI * 2 : angle;
}
float GetDistance(int index1, int index2, int width)
{
	int x = index1 % width - index2% width;
	int y = index1 / width - index2/ width;

	return x * x + y * y;
}
int GetSolutionPart2(float angles[], int sortedIndices[], int count)
{
	int destrotedAsteroids = 0, index = 0;
	float previousValue = -1;
	while (true)
	{
		float currentCheckAsteroid = angles[sortedIndices[index]];
		while (previousValue == currentCheckAsteroid || currentCheckAsteroid < 0)
		{
			index = (index + 1) % count;
			currentCheckAsteroid = angles[sortedIndices[index]];
		}

		previousValue = currentCheckAsteroid;
		angles[sortedIndices[index]] = -1; // destroy asteroid;
		
		destrotedAsteroids++;
		if (destrotedAsteroids == 200)
			return sortedIndices[index];
		index = (index + 1) % count;
	}

	return INT_MAX;
}

CREATE_TEST(10)
{
	FILE *file = fopen("day10.txt", "r");

	int count = 0, width = 0;
	char map[MAP_SIZE], character;
	float angles[MAP_SIZE];
	int sortedIndices[MAP_SIZE];

	while (fscanf(file, "%c", &character) == 1)
	{
		if (character != '\n')
			map[count++] = character;
		else if(width == 0)
			width = count;
	}

	int maxVisible = 0;
	int bestIndex = 0, asteroid200 = 0;
	for (int index1 = 0; index1 < count; ++index1)
	{
		memset(angles, 128, MAP_SIZE * sizeof(float));
		
		//if (map[index1] == ASTEROID)
		if (map[index1] == ASTEROID && index1 == 272)
		{
			for (int index2 = 0; index2 < count; ++index2)
			{
				if (map[index2] == ASTEROID && index1 != index2)
				{
					angles[index2] = GetAngle(index2, index1, width);
				}
			}

			//Part1
			//std::sort(angles, angles + count);
			//float* it = std::unique(angles, angles + count);
			//if (it - angles > maxVisible)
			//{
			//	maxVisible = it - angles;
			//	bestIndex = index1;
			//}

			//Part2
			bestIndex = index1;
			iota(sortedIndices, sortedIndices + count, 0);
			sort(sortedIndices, sortedIndices + count, [&angles, &index1, &width](int i1, int i2)
			{
				if (angles[i1] == angles[i2])
				{
					return GetDistance(index1, i1, width) < GetDistance(index1, i2, width);
				}
				else return angles[i1] < angles[i2]; 
			});
			asteroid200 = GetSolutionPart2(angles, sortedIndices, count);
		}
	}
	cout << "BestIndex:" << bestIndex <<"("<< bestIndex %width << "," << bestIndex / width << ") MaxVisible:" << maxVisible -1 << endl;
	cout << "Asteroid200:" << asteroid200 << "(" << asteroid200 % width << "," << asteroid200 / width << ")";
}
