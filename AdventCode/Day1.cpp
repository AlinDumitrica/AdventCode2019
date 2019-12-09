#include "main.h"

int calculateFuel(int mass)
{
    return std::max(mass / 3 - 2, 0);
}

CREATE_TEST(1)
{
    FILE *file = fopen("day1.txt", "r");

    int number, fuel = 0;

    while (fscanf(file, "%i,", &number) == 1)
    {
        int moduleFuel = calculateFuel(number);

        int fuelMass = moduleFuel;
        while (fuelMass > 0)
        {
            fuelMass = calculateFuel(fuelMass);
            moduleFuel += fuelMass;
        }

        fuel += moduleFuel;

    }

    std::cout << fuel;
}
