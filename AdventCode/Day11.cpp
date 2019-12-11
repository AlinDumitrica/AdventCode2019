#include "main.h"

#define LONGLONG "%I64d"
#define COUNT 50000

#define OPCODE_SUM			1
#define OPCODE_MUL			2
#define OPCODE_INPUT		3
#define OPCODE_OUTPUT		4
#define OPCODE_JUMP_TRUE	5
#define OPCODE_JUMP_FALSE	6
#define OPCODE_LESS			7
#define OPCODE_EQUALS		8
#define OPCODE_BASE_CHANGE  9
#define OPCODE_END 99

#define EXTRACT_TYPE(n)		((int64)(parameterCode / pow(10, n-1)) % 10) 
#define PARAM_VALUE(n)		opcode[index + n]
#define PARAM_REF(n)		opcode[opcode[index + n]]
#define PARAM_BASE(n)		opcode[relativeBase + PARAM_VALUE(n)]

#define RVALUE(n) \
EXTRACT_TYPE(n) == 0 ? PARAM_REF(n) :\
EXTRACT_TYPE(n) == 1 ? PARAM_VALUE(n) :\
EXTRACT_TYPE(n) == 2 ? PARAM_BASE(n) :\
0; assert(EXTRACT_TYPE(n) < 3);

#define LVALUE_ASSIGN(n, value)\
EXTRACT_TYPE(n) == 0 ? PARAM_REF(n) = value : \
EXTRACT_TYPE(n) == 1 ? PARAM_VALUE(n) = value: \
EXTRACT_TYPE(n) == 2 ? PARAM_BASE(n) = value: \
0; assert(EXTRACT_TYPE(n) < 3);

#define SHIP_SIZE 80
#define BLACK 0
#define WHILE 1
#define ROTATE(direction, angle) ((direction + (angle * 2 - 1) + 4) % 4)
#define ADVANCE(x, y, direction) x += -(direction-2) * (direction%2); y+= (direction-1)* ((direction+1)%2);

struct PaintCell
{
	bool color = BLACK;
	int hitCount = 0;
};

struct Program
{
public:
	int64 opcode[COUNT];
	int64 index = 0;
	int64 relativeBase = 0;

	Program(int64 _opcode[], int64 count)
	{
		memset(&opcode, 0, COUNT * sizeof(int64));
		memcpy(&opcode, _opcode, count * sizeof(int64));
	}

	int GetResultBloking(int64 input, int64 output[])
	{
		int inputRead = 0;
		int outputGenerated = 0;
		while (true)
		{
			int64 code = opcode[index];
			int64 instruction = code > 100 ? code % 100 : code;
			int64 parameterCode = code / 100;
			switch (instruction)
			{
			case OPCODE_SUM:
			{
				int64 p1 = RVALUE(1);
				int64 p2 = RVALUE(2);
				LVALUE_ASSIGN(3, p1 + p2);
				index += 4;
				break;
			}

			case OPCODE_MUL:
			{
				int64 p1 = RVALUE(1);
				int64 p2 = RVALUE(2);
				LVALUE_ASSIGN(3, p1 * p2);
				index += 4;
				break;
			}

			case OPCODE_BASE_CHANGE:
			{
				int64 p1 = RVALUE(1);
				relativeBase += p1;
				index += 2;
				break;
			}
			
			case OPCODE_JUMP_TRUE:
			{
				int64 p1 = RVALUE(1);
				int64 p2 = RVALUE(2);
				index = p1 == 0 ? index + 3 : p2;
				break;
			}

			case OPCODE_JUMP_FALSE:
			{
				int64 p1 = RVALUE(1);
				int64 p2 = RVALUE(2);
				index = p1 == 0 ? p2 : index + 3;
				break;
			}

			case OPCODE_LESS:
			{
				int64 p1 = RVALUE(1);
				int64 p2 = RVALUE(2);
				LVALUE_ASSIGN(3, p1 < p2 ? 1 : 0);
				index += 4;
				break;
			}

			case OPCODE_EQUALS:
			{
				int64 p1 = RVALUE(1);
				int64 p2 = RVALUE(2);
				LVALUE_ASSIGN(3, p1 == p2 ? 1 : 0);
				index += 4;
				break;
			}

			case OPCODE_INPUT:
			{
				if (inputRead > 0)
					return OPCODE_INPUT;
				LVALUE_ASSIGN(1, input);
				inputRead++;
				index += 2;
				break;
			}

			case OPCODE_OUTPUT:
			{
				int64 p1 = RVALUE(1);
				output[outputGenerated++] = p1;
				index += 2;
				break;
			}

			case  OPCODE_END:
			{
				return  OPCODE_END;
			}
			default:
			{
				assert(false);
			}
			}
		}
	}
};

CREATE_TEST(11)
{
	FILE *file = fopen("day11.txt", "r");

	int64 number, count = 0;
	int64 opcode[COUNT];
	PaintCell ship[SHIP_SIZE][SHIP_SIZE];

	while (fscanf(file, LONGLONG",", &number) == 1)
	{
		opcode[count++] = number;
	}

	Program program = Program(opcode, count);
	int64 x = SHIP_SIZE / 2, y = x, result[2], direction = 0;
	ship[x][y].color = 1;

	while (program.GetResultBloking(ship[x][y].color, result) != OPCODE_END)
	{
		ship[x][y].hitCount++;
		ship[x][y].color = result[0];
		direction = ROTATE(direction, result[1]);
		ADVANCE(x, y, direction);
	}

	int hitCount = 0;
	for (int index = 0; index < SHIP_SIZE; index++)
	{
		for (int index2 = 0; index2 < SHIP_SIZE; index2++)
		{
			if(ship[index2][index].hitCount > 0)
				hitCount++;
			cout << (ship[index2][index].color ? "*" : " ");
		}
		cout << endl;
	}

	cout << hitCount;
}
