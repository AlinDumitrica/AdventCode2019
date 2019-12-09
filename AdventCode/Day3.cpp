#include "main.h"

struct Vector2
{
    int x = 0;
    int y = 0;
    Vector2() : x(0), y(0) {};
    Vector2(int _x, int _y) : x(_x), y(_y) {};
};

struct Line 
{
    Vector2 origin;
    Vector2 destination;
    char direction;
    int length;
};

Vector2 GetDestination(Line &line)
{
    switch (line.direction)
    {
    case 'R': return Vector2(line.origin.x + line.length, line.origin.y);
    case 'L': return Vector2(line.origin.x - line.length, line.origin.y);
    case 'U': return Vector2(line.origin.x, line.origin.y - line.length);
    case 'D': return Vector2(line.origin.x, line.origin.y + line.length);
    }

    return Vector2();
}

void ReadCircuit(FILE *file, std::vector<Line> &circuit)
{
    int length, index = 0; char direction;
    Vector2 position(0,0);
    while (fscanf(file, "%c%i,", &direction, &length) == 2)
    {
        Line& line = circuit[index++];
        line.origin = position;
        line.direction = direction;
        line.length = length;

        position = GetDestination(line);
        line.destination = position;
    }
}

bool IsIntersection(int x1, int x2, int y1, int y2, Vector2 &interval)
{
    int a1 = std::min(x1, x2);
    int a2 = std::max(x1, x2);
    int b1 = std::min(y1, y2);
    int b2 = std::max(y1, y2);

    if (b1 > a2 || a1 > b2)
    {
        return false;
    }
    else
    {
        interval.x = std::max(a1, b1);
        interval.y = std::min(a2, b2);
        return true;
    }
}

bool IsIntersection(Line& line1, Line& line2, Vector2& intersection)
{
    Vector2 intersectionX, intersectionY;
    if (IsIntersection(line1.origin.x, line1.destination.x, line2.origin.x, line2.destination.x, intersectionX) &&
        IsIntersection(line1.origin.y, line1.destination.y, line2.origin.y, line2.destination.y, intersectionY))
    {
        intersection.x = intersectionX.x < 0 ? intersectionX.y < 0 ? intersectionX.y : 0 : intersectionX.y > 0 ? intersectionX.x : 0;
        intersection.y = intersectionY.x < 0 ? intersectionY.y < 0 ? intersectionY.y : 0 : intersectionY.y > 0 ? intersectionY.x : 0;
        return true;
    }

    return false;
}

int Distance(Vector2& point1, Vector2& point2)
{
    return abs(abs(point1.x) - abs(point2.x)) + abs(abs(point1.y) - abs(point2.y));
}

int GetIntersection(std::vector<Line> &circuit1, std::vector<Line> &circuit2, Vector2 &minIntersection)
{
    int Count = 301;
    int minDistance = INT_MAX, minLength = INT_MAX;
    int length1 = 0; int length2 = 0;

    Vector2 intersection;

    for (int i = 0; i < Count; i++)
    {
        Line& line1 = circuit1[i];
        
        length2 = 0;

        for (int j = 0; j < Count; j++)
        {
            Line& line2 = circuit2[j];
            if (i == 0 && j == 0)
            {
                length2 += line2.length;
                continue;
            }
            //closest to start        
            //if (IsIntersection(line1, line2, intersection))
            //{
            //    if (minDistance > abs(intersection.x) + abs(intersection.y))
            //    {
            //        minDistance = abs(intersection.x) + abs(intersection.y);
            //        minIntersection = intersection;
            //    }
            //}

            //shortest length
            if(minLength < length1 + length2)
                break;
            if (IsIntersection(line1, line2, intersection))
            {
                int dist1 = Distance(line1.origin, intersection);
                int dist2 = Distance(line2.origin, intersection);

                std::cout << i << " " << j << " " << length1 + length2 + dist1 + dist2 << endl;
                if (minLength > length1 + length2 + dist1 + dist2)
                    minLength = length1 + length2 + dist1 + dist2;
            }

            length2 += line2.length;
        }

        length1 += line1.length;
    }

    //return minDistance;
    return minLength;
}

CREATE_TEST(3)
{
    FILE *file = fopen("day3.txt", "r");

    std::vector<Line> circuit1(500), circuit2(500);
    ReadCircuit(file, circuit1);
    ReadCircuit(file, circuit2);

    Vector2 intersection;
    int minDistance = GetIntersection(circuit1, circuit2, intersection);
    cout << minDistance;
}
