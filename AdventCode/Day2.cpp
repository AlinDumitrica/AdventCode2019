#include "main.h"

#define OPCODE_SUM 1
#define OPCODE_MUL 2
#define OPCODE_END 99

#define FIND_NUMBER 19690720

CREATE_TEST(2)
{
    FILE *file = fopen("day2.txt", "r");

    int number, count = 0;
    int opcode[1000];
    int opcode2[1000];

    while (fscanf(file, "%i,", &number) == 1)
    {
        opcode2[count++] = number;
    }

    opcode[1] = 12;
    opcode[2] = 2;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            for (int k = 0; k < count; k++)
            {
                opcode[k] = opcode2[k];
            }

            int index = 0;
            opcode[1] = i;
            opcode[2] = j;

            while (opcode[index] != OPCODE_END)
            {
                switch (opcode[index])
                {
                case OPCODE_SUM:
                {
                    opcode[opcode[index + 3]] = opcode[opcode[index + 1]] + opcode[opcode[index + 2]];
                    index += 4;
                    break;
                }

                case OPCODE_MUL:
                {
                    opcode[opcode[index + 3]] = opcode[opcode[index + 1]] * opcode[opcode[index + 2]];
                    index += 4;
                    break;
                }
                }
            }

            if (opcode[0] == FIND_NUMBER)
            {
                std::cout << i << " " << j;
                return;
            }
        }
    }
    //std::cout << opcode[0];
}
