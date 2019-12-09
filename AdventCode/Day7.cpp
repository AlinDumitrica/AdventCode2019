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

#define COUNT 1000
#define DEPTH 5
#define CODE_LIMIT ((1 << DEPTH) - 1)
#define DEPTH_PART2 10
#define CODE_LIMIT_PART2 (((1 << DEPTH_PART2) - 1) - CODE_LIMIT)

struct Program
{
public:
	int opcode[COUNT];
	int index = 0;

	int GetResultBloking(int input, int &output)
	{
		int inputRead = 0;
		int outputGenerated = 0;
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
				if (inputRead > 0)
					return 0;
				PARAM_REF(1) = input;
				inputRead++;
				index += 2;
				break;
			}

			case OPCODE_OUTPUT:
			{
				assert(outputGenerated == 0);
				int p1 = RVALUE(1);
				output = p1;
				outputGenerated++;
				index += 2;
				break;
			}
			}
		}
		return 1;
	}
};

void GetResult(int opcode[], int input[], int output[])
{
	int index = 0, indexInput = 0, indexOutput = 0;

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
				assert((parameterCode % 10) == 0);

				PARAM_REF(1) = input[indexInput++];
				index += 2;
				break;
			}

			case OPCODE_OUTPUT:
			{
				int p1 = RVALUE(1);
				output[indexOutput++] = p1;
				index += 2;
				break;
			}
		}
	}
}

int GenerateSequence(int opcode[], int code, int previousOutput)
{
	if (code == CODE_LIMIT)
	{
		return previousOutput;
	}

	int opcodeClean[COUNT];
	int maxResult = -INT_MAX;
	for (int index = 0; index < DEPTH; ++index)
	{
		if ((code & (1 << index)) == 0)
		{
			memcpy(&opcodeClean, opcode, COUNT * sizeof(int));
			int input[] = { index, previousOutput };
			int output;
			GetResult(opcodeClean, input, &output);
			maxResult = max(maxResult, GenerateSequence(opcode, code | (1 << index), output));
		}
	}

	return maxResult;
}

int GenerateSequencePart2(int opcode[], int code, int sequence)
{
	if (code == CODE_LIMIT_PART2)
	{
		Program* programSequence[5];
		std::stack<int> inputs;
		int index = 0, result;

		//cout <<"Starting " << sequence;
		while (sequence % 10 > 0)
		{
			Program* program = new Program();
			programSequence[index] = program;
			memcpy(program->opcode, opcode, COUNT * sizeof(int));
			program->GetResultBloking(sequence % 10, result);

			index++; sequence /= 10;
		}

		programSequence[0]->GetResultBloking(0, result);
		inputs.push(result);

		index = 1;
		while (!inputs.empty() && programSequence[index])
		{
			if (programSequence[index]->GetResultBloking(inputs.top(), result) == 1)
			{
				delete programSequence[index];
				programSequence[index] = nullptr;
			}

			inputs.pop();
			inputs.push(result);
			index = (index + 1) % 5;
		}

		//cout << "   Size:" << inputs.size() <<"   Value:"<< inputs.top() << endl;
		return inputs.top();
	}

	int maximumValue;
	for (int index = DEPTH; index < DEPTH_PART2; ++index)
	{
		if ((code & (1 << index)) == 0)
		{
			maximumValue = max(maximumValue, GenerateSequencePart2(opcode, code | (1 << index), sequence * 10 + index));
		}
	}

	return maximumValue;
}

CREATE_TEST(7)
{
	FILE *file = fopen("day7.txt", "r");

	int number, count = 0;
	int opcode[COUNT];
	while (fscanf(file, "%i,", &number) == 1)
	{
		opcode[count++] = number;
	}

	int startInputPart1 = 0;
	cout << GenerateSequence(opcode, 0, startInputPart1) << endl;

	int sequence = 0;
	cout << GenerateSequencePart2(opcode, 0, sequence) << endl;
}
