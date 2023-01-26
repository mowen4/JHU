//hello world for the git repo
// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class InputOutput
{
    //TODO: Checking for errors, for negative values and potentially too large values?

    public:
        void getInputs(int inputs[3])
        {

            std::cout << "Triangle Tester" << std::endl;
            std::cout << "Enter 1st Side Length: ";
            std::cin >> inputs[0];
            std::cout << "Enter 2nd Side Length: ";
            std::cin >> inputs[1];
            std::cout << "Enter 3rd Side Length: ";
            std::cin >> inputs[2];

        }
};

class Triangle
{
    private:
        int a, b ,c;
    public:

        Triangle(int x, int y, int z)
        {
            a = x;
            b = y;
            c = z;
        }


        void determineTriangleType()
        {

            std::cout << a << b << c << std::endl;
            
            bool noType = true;
            
            if(!isValidTriangle())
            {
                std::cout << "You have entered invalid lengths for the sides of a real triangle." << endl;
                noType = false;
            }
            if (a == b && b == c && a == c)
            {
                std::cout << "It is Equilateral" << std::endl;
                noType = false;
            }
            if (a == b || a == c || b == c)
            {
                std::cout << "It is Isosceles" << std::endl;
                noType = false;
            }
            if ((a*a + b*b == c*c) || (b*b + c*c == a*a))
            {
                std::cout << "It is a Right Triangle" << std::endl;
                noType = false;
            }
            if (noType)
            {
                std::cout << "It is none of the standard types" << std::endl;
            }
        }

        bool isValidTriangle()
        {
            //the sum of any two sides must be greater than the length of the third side
            if(a + b <= c || a + c <= b || b + c <= a){
                return false;
            }
            //no side can have a negative or zero length
            else if (a <= 0 || b <= 0 || c <= 0){
                return false;
            }

            return true;
        }

};


int main()
{
    int sides[3] = {0,0,0};

    InputOutput io;
    io.getInputs(sides);

    Triangle t(sides[0],sides[1],sides[2]);
    t.determineTriangleType();

    return 0;
}