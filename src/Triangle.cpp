//Triangle Class Object

#include <stdlib.h>

using namespace std;

class Triangle
{
public:
    // Triangle types, for unit testing
    enum TriangleType
    {
        equilateral,
        isosceles,
        scalene,
        invalid
    };
    TriangleType type;

    Triangle(int x, int y, int z)
    {
        side1 = x;
        side2 = y;
        side3 = z;
    }

    // determines triangle type from side lengths
    void determineTriangleType()
    {
        // check if trianle has valid side lengths
        if (isValidTriangle())
        {
            // Transitive property: if a==b and b==c then a==b==c
            if (side1 == side2 && side2 == side3)
            {
                std::cout << "It is Equilateral" << std::endl;
                type = equilateral;
            }
            // if any pair of sides is equal then it is also Isosceles
            else if (side1 == side2 || side1 == side3 || side2 == side3)
            {
                std::cout << "It is Isosceles" << std::endl;
                type = isosceles;
            }
            // otherwise it is scalene
            else
            {
                std::cout << "It is Scalene" << std::endl;
                type = scalene;
            }
        }
        else
        {
            std::cout << "You have entered invalid lengths for the sides of a real triangle." << endl;
            type = invalid;
        }
    }

private:
    int side1, side2, side3;

    // checks if side lengths can form a valid real triangle
    bool isValidTriangle()
    {
        // the sum of any two sides must be greater than the length of the third side
        if (side1 + side2 <= side3 || side1 + side3 <= side2 || side2 + side3 <= side1)
        {
            return false;
        }
        // no side can have a negative or zero length
        else if (side1 <= 0 || side2 <= 0 || side3 <= 0)
        {
            return false;
        }

        return true;
    }
};
