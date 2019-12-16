#include "main.h"

struct Moon
{
public:
	//int x, y, z;
	//int vX = 0, vY = 0, vZ = 0;
	int p[3];
	int v[3];
};
struct History
{
	int value0; int velocity0;
	int value1; int velocity1;
	int value2; int velocity2;
	int value3; int velocity3;
}historyArray[500000];

#define UPDATE_VELOCITY(i, j, axis) (moons[i].p[axis] < moons[j].p[axis] ? 1 : moons[i].p[axis] == moons[j].p[axis] ? 0 : -1)

#define UPDATE_ASTEROID_VELOCITY(n) \
moons[n].vX +=UPDATE_VELOCITY(n, 0, x) + UPDATE_VELOCITY(n, 1, x) + UPDATE_VELOCITY(n, 2, x) + UPDATE_VELOCITY(n, 3, x); \
moons[n].vY +=UPDATE_VELOCITY(n, 0, y) + UPDATE_VELOCITY(n, 1, y) + UPDATE_VELOCITY(n, 2, y) + UPDATE_VELOCITY(n, 3, y); \
moons[n].vZ +=UPDATE_VELOCITY(n, 0, z) + UPDATE_VELOCITY(n, 1, z) + UPDATE_VELOCITY(n, 2, z) + UPDATE_VELOCITY(n, 3, z);

#define UPDATE_POSITION(n)\
moons[n].p[0] += moons[n].v[0];\
moons[n].p[1] += moons[n].v[1];\
moons[n].p[2] += moons[n].v[2];

void SolveAxis(Moon moonsClean[], int axis, int neededLoops[], int &count)
{
	Moon moons[4]; int index = 0;
	memset(historyArray, 0, 10000 * sizeof(History));
	memcpy(moons, moonsClean, 4 * sizeof(Moon));
	count = 0;
	do 
	{
		History& history = historyArray[index];
		history.value0 = moons[0].p[axis]; history.velocity0 = moons[0].v[axis];
		history.value1 = moons[1].p[axis]; history.velocity1 = moons[1].v[axis];
		history.value2 = moons[2].p[axis]; history.velocity2 = moons[2].v[axis];
		history.value3 = moons[3].p[axis]; history.velocity3 = moons[3].v[axis];

		moons[0].v[axis] += UPDATE_VELOCITY(0, 0, axis) + UPDATE_VELOCITY(0, 1, axis) + UPDATE_VELOCITY(0, 2, axis) + UPDATE_VELOCITY(0, 3, axis);
		moons[1].v[axis] += UPDATE_VELOCITY(1, 0, axis) + UPDATE_VELOCITY(1, 1, axis) + UPDATE_VELOCITY(1, 2, axis) + UPDATE_VELOCITY(1, 3, axis);
		moons[2].v[axis] += UPDATE_VELOCITY(2, 0, axis) + UPDATE_VELOCITY(2, 1, axis) + UPDATE_VELOCITY(2, 2, axis) + UPDATE_VELOCITY(2, 3, axis);
		moons[3].v[axis] += UPDATE_VELOCITY(3, 0, axis) + UPDATE_VELOCITY(3, 1, axis) + UPDATE_VELOCITY(3, 2, axis) + UPDATE_VELOCITY(3, 3, axis);
		UPDATE_POSITION(0); UPDATE_POSITION(1); UPDATE_POSITION(2); UPDATE_POSITION(3);

		for (int test = 0; test < index; ++test)
		{
			if (moons[0].p[axis] == historyArray[test].value0 && moons[0].v[axis] == historyArray[test].velocity0 &&
				moons[1].p[axis] == historyArray[test].value1 && moons[1].v[axis] == historyArray[test].velocity1 &&
				moons[2].p[axis] == historyArray[test].value2 && moons[2].v[axis] == historyArray[test].velocity2 &&
				moons[3].p[axis] == historyArray[test].value3 && moons[3].v[axis] == historyArray[test].velocity3)
				//return index;
			{
				neededLoops[count++] = index + 1;
				if (test == 0)
					return;
			}
		}

		index++;
	} while (true);
}

CREATE_TEST(12)
{
    FILE *file = fopen("day12.txt", "r");

    int valX, valY, valZ, index=0;
	Moon moons[4], moonsCopy[4];

    while (fscanf(file, "<x=%i, y=%i, z=%i>", &valX, &valY, &valZ) == 3)
    {
		Moon& moon = moons[index++];
		moon.p[0] = valX; moon.v[0] = 0;
		moon.p[1] = valY; moon.v[1] = 0;
		moon.p[2] = valZ; moon.v[2] = 0;
    }

	/*
	index = 0;
	memcpy(moonsCopy, moons, 4 * sizeof(Moon));
	while (index < 1000)
	{

		UPDATE_ASTEROID_VELOCITY(0);
		UPDATE_ASTEROID_VELOCITY(1);
		UPDATE_ASTEROID_VELOCITY(2);
		UPDATE_ASTEROID_VELOCITY(3);
		
		UPDATE_POSITION(0);
		UPDATE_POSITION(1);
		UPDATE_POSITION(2);
		UPDATE_POSITION(3);
		index++;
	}

	int sum = 0;
	for (int index = 0; index < 4; ++index)
		sum += (abs(moons[index].x) + abs(moons[index].y) + abs(moons[index].z)) * (abs(moons[index].vX) + abs(moons[index].vY) + abs(moons[index].vZ));
	cout << "Part1:" << sum <<endl;
*/
	//do{
	//	moons[0].vX += UPDATE_VELOCITY(0, 0, x) + UPDATE_VELOCITY(0, 1, x) + UPDATE_VELOCITY(0, 2, x) + UPDATE_VELOCITY(0, 3, x);
	//	moons[1].vX += UPDATE_VELOCITY(1, 0, x) + UPDATE_VELOCITY(1, 1, x) + UPDATE_VELOCITY(1, 2, x) + UPDATE_VELOCITY(1, 3, x);
	//	moons[2].vX += UPDATE_VELOCITY(2, 0, x) + UPDATE_VELOCITY(2, 1, x) + UPDATE_VELOCITY(2, 2, x) + UPDATE_VELOCITY(2, 3, x);
	//	moons[3].vX += UPDATE_VELOCITY(3, 0, x) + UPDATE_VELOCITY(3, 1, x) + UPDATE_VELOCITY(3, 2, x) + UPDATE_VELOCITY(3, 3, x);
	//
	//	UPDATE_POSITION(0);UPDATE_POSITION(1);UPDATE_POSITION(2);UPDATE_POSITION(3);
	//	indexX++;
	//} while (moons[0].x != moonsCopy[0].x || moons[1].x != moonsCopy[1].x || moons[2].x != moonsCopy[2].x || moons[3].x != moonsCopy[3].x);
	//cout << "indexX:" << indexX << endl;
	int indexX[10000], indexY[10000], indexZ[10000];
	int countX, countY, countZ;
	SolveAxis(moons, 0, indexX, countX);
	SolveAxis(moons, 1, indexY, countY);
	SolveAxis(moons, 2, indexZ, countZ);

	for (int iX = 0; iX < countX; iX++)
		cout << indexX[iX] << " ";
	cout << endl;

	for (int iY = 0; iY < countX; iY++)
		cout << indexY[iY] << " ";
	cout << endl;

	for (int iZ = 0; iZ < countX; iZ++)
		cout << indexZ[iZ] << " ";
	cout << endl;

	//cout << "indexX:" << indexX << endl;
	//cout << "indexY:" << indexY << endl;
	//cout << "indexZ:" << indexZ << endl;
	//cout << indexX << " " << indexY << " " << indexX << endl;
	//
	//cout << indexX * indexY * indexZ;
}
