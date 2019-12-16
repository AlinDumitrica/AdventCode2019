#include "main.h"

struct Moon
{
public:
	int x, y, z;
	int vX = 0, vY = 0, vZ = 0;
};
struct History
{
	int value0; int velocity0;
	int value1; int velocity1;
	int value2; int velocity2;
	int value3; int velocity3;
}history[10000];

#define UPDATE_VELOCITY(i, j, axis) (moons[i].##axis < moons[j].##axis ? 1 : moons[i].##axis == moons[j].##axis ? 0 : -1)

#define UPDATE_ASTEROID_VELOCITY(n) \
moons[n].vX +=UPDATE_VELOCITY(n, 0, x) + UPDATE_VELOCITY(n, 1, x) + UPDATE_VELOCITY(n, 2, x) + UPDATE_VELOCITY(n, 3, x); \
moons[n].vY +=UPDATE_VELOCITY(n, 0, y) + UPDATE_VELOCITY(n, 1, y) + UPDATE_VELOCITY(n, 2, y) + UPDATE_VELOCITY(n, 3, y); \
moons[n].vZ +=UPDATE_VELOCITY(n, 0, z) + UPDATE_VELOCITY(n, 1, z) + UPDATE_VELOCITY(n, 2, z) + UPDATE_VELOCITY(n, 3, z);

#define UPDATE_POSITION(n)\
moons[n].x += moons[n].vX;\
moons[n].y += moons[n].vY;\
moons[n].z += moons[n].vZ;

/**/
#define SOLVE_AXIS(n, N)\
do {\
	moons[0].v##N += UPDATE_VELOCITY(0, 0, n) + UPDATE_VELOCITY(0, 1, n) + UPDATE_VELOCITY(0, 2, n) + UPDATE_VELOCITY(0, 3, n);\
	moons[1].v##N += UPDATE_VELOCITY(1, 0, n) + UPDATE_VELOCITY(1, 1, n) + UPDATE_VELOCITY(1, 2, n) + UPDATE_VELOCITY(1, 3, n);\
	moons[2].v##N += UPDATE_VELOCITY(2, 0, n) + UPDATE_VELOCITY(2, 1, n) + UPDATE_VELOCITY(2, 2, n) + UPDATE_VELOCITY(2, 3, n);\
	moons[3].v##N += UPDATE_VELOCITY(3, 0, n) + UPDATE_VELOCITY(3, 1, n) + UPDATE_VELOCITY(3, 2, n) + UPDATE_VELOCITY(3, 3, n);\
	UPDATE_POSITION(0); UPDATE_POSITION(1); UPDATE_POSITION(2); UPDATE_POSITION(3);\
	History& history = history[index##N];\
	if(find_if(history, history + index, [&history](const History &arg) { \
		return	moons[0].##n != arg.value0 && moons[0].v##n != arg.velocity0 && \
				moons[1].##n != arg.value1 && moons[1].v##n != arg.velocity1 && \ 
				moons[2].##n != arg.value2 && moons[2].v##n != arg.velocity2 && \
				moons[3].##n != arg.value3 && moons[3].v##n != arg.velocity3}))\
		break;\
	index##N++;\
} while (true);\
cout << "index##N:" << index##N << endl;


CREATE_TEST(12)
{
    FILE *file = fopen("day12.txt", "r");

    int valX, valY, valZ, index=0;
	Moon moons[4], moonsCopy[4];

    while (fscanf(file, "<x=%i, y=%i, z=%i>", &valX, &valY, &valZ) == 3)
    {
		Moon& moon = moons[index++];
		moon.x = valX;
		moon.y = valY;
		moon.z = valZ;
    }

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
	int indexX = 0, indexY = 0, indexZ = 0;
	memcpy(moons, moonsCopy, 4 * sizeof(Moon));
	SOLVE_AXIS(x, X);
	memcpy(moons, moonsCopy, 4 * sizeof(Moon));
	SOLVE_AXIS(y, Y);
	memcpy(moons, moonsCopy, 4 * sizeof(Moon));
	SOLVE_AXIS(z, Z);

	cout << indexX * indexY * indexZ;
}
