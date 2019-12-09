#include "main.h"

#define LONGLONG "%I64d"

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

#define COUNT 50000

struct Program
{
public:
	int64 opcode[COUNT];
	int64 index = 0;
	int64 relativeBase = 0;

	Program(int64 _opcode[], int64 count)
	{
		memset(&opcode, 0, COUNT * sizeof(int64_t));
		memcpy(&opcode, _opcode, count * sizeof(int64_t));
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

CREATE_TEST(9)
{
	FILE *file = fopen("day9.txt", "r");

	int64 number, count = 0;
	int64 opcode[COUNT];
	while (fscanf(file, LONGLONG",", &number) == 1)
	{
		opcode[count++] = number;
	}

	Program program = Program(opcode, count);
	int64 output[10];
	program.GetResultBloking(2, output);

	for(int index = 0; index < 10;++index)
		cout << output[index]<< " ";
}
