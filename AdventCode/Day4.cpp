#include "main.h"

#define OPCODE_SUM 1
#define OPCODE_MUL 2
#define OPCODE_END 99

#define FIND_NUMBER 19690720

bool CheckNumber(int number)
{
	int consecutive = 1;
	int maxConsecutive = 1;
	bool hasPair = false;

	int lastDigit = number % 10;
	number = number / 10;
	
	do
	{
		int digit = number % 10;
		if (digit > lastDigit)
		{
			return false;
		}
		else if(digit == lastDigit)
		{
			consecutive++;
		}
		else
		{
			maxConsecutive = std::max(consecutive, maxConsecutive);
			hasPair = hasPair || consecutive == 2;
			consecutive = 1;
		}

		lastDigit = digit;
		number = number / 10;
	} while (number > 0);

	maxConsecutive = std::max(consecutive, maxConsecutive);
	hasPair = hasPair || consecutive == 2;

	//return maxConsecutive >= 2; // first part
	return hasPair; // second part
}

CREATE_TEST(4)
{
	FILE *file = fopen("day4.txt", "r");

	int start, end, count = 0;
	fscanf(file, "%i-%i", &start, &end);

	//start = 100000;
	//end = 999999;
	for (int index = start; index <= end; ++index)
	{
		if (CheckNumber(index))
		{
			count++;
			std::cout << index << endl;
		}
	}

	std::cout << count;
}
