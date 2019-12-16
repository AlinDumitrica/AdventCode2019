#include "main.h"

char input[1000];
int phase[] = { 0, 1, 0, -1 };
#define PHASE_VALUE(digit, i) phase[(((i+1) % ((digit+1) * 4)) / (digit+1)) % 4]
#define REPEAT 10000

CREATE_TEST(16)
{
    FILE *file = fopen("day16.txt", "r");

    char number;
	int count = 0;

    while (fscanf(file, "%c,", &number) == 1)
    {
		input[count++] = number - '0';
    }

	int index = 0;
	char phaseValue[1000];
	memcpy(phaseValue, input, count * sizeof(char));

	int digitGenerated = (count + 1) * 4* count;// std::lcm((count + 1) * 4, count);

	while (index < 100)
	{
		char phaseValueAux[1000];
		for (int digit = 0; digit < count; digit++)
		{
			int phaseLenght = (digit + 1) * 4;
			int neededLength = std::min(std::lcm(phaseLenght, count), REPEAT * count);
			int cyclesRepeat = (count * REPEAT / neededLength);

			int digitSum = 0;
			for (int i = 0; i < neededLength; i++)
			{
				//cout << (PHASE_VALUE(digit, i) < 0 ? 3 : PHASE_VALUE(digit, i));
				digitSum += PHASE_VALUE(digit, i) * phaseValue[i%count];
			}
			digitSum *= cyclesRepeat; //cout << endl;
			for (int i = cyclesRepeat * neededLength; i < REPEAT * count; i++)
			{
				digitSum += PHASE_VALUE(digit, i) * phaseValue[i%count];
			}

			//cout << "index" << digit << " "<<abs(digitSum % 10)<<endl;
			cout << abs(digitSum % 10);
			//phaseValueAux[digit] = abs(digitSum % 10);
		}

		memcpy(phaseValue, phaseValueAux, count * sizeof(char));
		index++;
	}
	
	int offsetNumber = 0;
	for (int i = 0; i < 7; i++)
		offsetNumber = offsetNumber * 10 + input[i];

	cout << offsetNumber << endl;
	for (int i = 0; i < count; i++)
	{
		cout << int(phaseValue[i]);
	}
	cout << endl;
	for (int i = offsetNumber; i < offsetNumber + 8; i++)
	{
		cout << int(phaseValue[i%count]);
	}
}
