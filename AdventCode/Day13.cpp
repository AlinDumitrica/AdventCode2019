#ifdef DAY13
#include "main.h"
#include <windows.h>
#include <chrono>
#include <thread>

#define LONGLONG "%I64d"
#define COUNT 5000

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

#define SHIP_SIZE 200
#define BLACK 0
#define WHILE 1
#define ROTATE(direction, angle) ((direction + (angle * 2 - 1) + 4) % 4)
#define ADVANCE(x, y, direction) x += -(direction-2) * (direction%2); y+= (direction-1)* ((direction+1)%2);

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

	int GetResultBloking3(int64 input, int64 output[])
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

				if (outputGenerated == 3)
					return OPCODE_OUTPUT;
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

int64 game[50][50];

CREATE_TEST(13)
{
	FILE *file = fopen("day13.txt", "r");

	int64 number, count = 0;
	int64 opcode[COUNT];

	while (fscanf(file, LONGLONG",", &number) == 1)
	{
		opcode[count++] = number;
	}

	Program program = Program(opcode, count);
	int64 result[3], maxX = 0, maxY = 0;
	int64  ballX, ballY, paddleX, paddleY;
	while (program.GetResultBloking3(0, result) != OPCODE_END)
	{
		//cout << result[0] << " " << result[1] << " " << result[2] << endl;
		game[result[0]][result[1]] = result[2];
		maxX = max(maxX, result[0]);
		maxY = max(maxY, result[1]);
		if (result[2] == 4)
		{
			ballX = result[0]; ballY = result[1];
		}
		else if (result[2] == 3)
		{
			paddleX = result[0]; paddleY = result[1];
		}
	}
	
	opcode[0] = 2; Program program2 = Program(opcode, count);
	HANDLE hStdout;
	COORD destCoord; destCoord.X = 0; destCoord.Y = 0;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	
	int maxScore = 0;
	while (true)
	{
		SetConsoleCursorPosition(hStdout, destCoord);
		while (program2.GetResultBloking3(paddleX < ballX ? 1 : paddleX == ballX ? 0 : -1, result) != OPCODE_END)
		{
			game[result[0]][result[1]] = result[2];
			if (result[2] == 4)
			{
				ballX = result[0]; ballY = result[1];
			}
			else if (result[2] == 3)
			{
				paddleX = result[0]; paddleY = result[1];
			}
			if (result[0] == -1 && result[1] == 0)
			{
				maxScore = result[2];
				cout << result[2] << endl;
				break;
			}
		}

		for (int iY = 0; iY < maxY; iY++)
		{
			for (int iX = 0; iX < maxX; iX++)
				switch (game[iX][iY])
				{
				case 0: cout << " "; break;
				case 1: cout << "|"; break;
				case 2: cout << "#"; break;
				case 3: cout << "_"; break;
				case 4: cout << "o"; break;
				}
			cout << endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	cout << "END:" << maxScore;
}
#endif