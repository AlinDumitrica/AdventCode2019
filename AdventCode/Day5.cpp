#include "main.h"

#define OPCODE_SUM			1
#define OPCODE_MUL			2
#define OPCODE_INPUT		3
#define OPCODE_OUTPUT		4
#define OPCODE_JUMP_TRUE	5
#define OPCODE_JUMP_FALSE	6
#define OPCODE_LESS			7
#define OPCODE_EQUALS		8
#define OPCODE_END 99

#define INPUT 5

#define PARAM_REF(n) opcode[opcode[index + n]]
#define RVALUE(n) ((int)(parameterCode / pow(10, n-1)) % 10) ? opcode[index + n] : PARAM_REF(n)

CREATE_TEST(5)
{
	FILE *file = fopen("day5.txt", "r");

	int number, count = 0;
	int opcode[1000];
	while (fscanf(file, "%i,", &number) == 1)
	{
		opcode[count++] = number;
	}

	int index = 0;
	while (opcode[index] != OPCODE_END)
	{
		int code = opcode[index];
		int instruction = code > 100 ? code % 100 : code;
		int parameterCode = code / 100;
		switch (instruction)
		{
			case OPCODE_SUM:
			{
				int p1 = RVALUE(1);
				int p2 = RVALUE(2);
				PARAM_REF(3) = p1 + p2;
				index += 4;
				break;
			}

			case OPCODE_MUL:
			{
				int p1 = RVALUE(1);
				int p2 = RVALUE(2);
				PARAM_REF(3) = p1 * p2;
				index += 4;
				break;
			}

			case OPCODE_JUMP_TRUE:
			{
				int p1 = RVALUE(1);
				int p2 = RVALUE(2);
				index = p1 == 0 ? index + 3 : p2;
				break;
			}

			case OPCODE_JUMP_FALSE:
			{
				int p1 = RVALUE(1);
				int p2 = RVALUE(2);
				index = p1 == 0 ? p2 : index + 3;
				break;
			}

			case OPCODE_LESS:
			{
				int p1 = RVALUE(1);
				int p2 = RVALUE(2);
				PARAM_REF(3) = p1 < p2 ? 1 : 0;
				index += 4;
				break;
			}

			case OPCODE_EQUALS:
			{
				int p1 = RVALUE(1);
				int p2 = RVALUE(2);
				PARAM_REF(3) = p1 == p2 ? 1 : 0;
				index += 4;
				break;
			}

			case OPCODE_INPUT:
			{
				opcode[opcode[index + 1]] = INPUT;
				index += 2;
				break;
			}

			case OPCODE_OUTPUT:
			{
				int p1 = RVALUE(1);
				cout << p1 << endl;
				index += 2;
				break;
			}
		}
	}
}
