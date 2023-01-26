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
            //temporary local variables
            int temp = 0;
            bool input_ok;

            std::cout << "Triangle Tester" << std::endl;
            
            //for each side repeat the inner loop until a valid integer input is detected
            for ( int i = 0; i < 3; ++i )
            {
                do
                {
                    cout << "Enter side " << i +1 <<":";
                    cin >> inputs[i];

                    if ( cin.fail() )
                    {
                        cout << "Error: Conversion to integer failed!\n";

                        input_ok = false;
                    }
                    else if ( inputs[i] > 2147483647 || inputs[i] <= 0 )
                    {
                        cout << "Error: Number should in range of (1 to 2147483647) only!\n";
                        input_ok = false;
                    }
                    else
                    {
                        input_ok = true;
                    }

                    cin.clear();

                    cin.ignore(2147483647, '\n' );
                } while ( !input_ok );
            
            }
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

            //std::cout << a << b << c << std::endl;
            
            bool noType = true;
            
            //check if trianle is valid size
            if(!isValidTriangle())
            {
                std::cout << "You have entered invalid lengths for the sides of a real triangle." << endl;
                noType = false;
            }
            //if a==b and b==c then a==b==c
            if (a == b && b == c)
            {
                std::cout << "It is Equilateral" << std::endl;
                noType = false;
            }
            //if any pair of sides is equal then it is also Isosceles
            if (a == b || a == c || b == c)
            {
                std::cout << "It is Isosceles" << std::endl;
                noType = false;
            }
            //if any combination of legs meet pythagorean theorem it is a right triangle
            if ((a*a + b*b == c*c) || (b*b + c*c == a*a) || (c*c + a*a == b*b))
            {
                std::cout << "It is a Right Triangle" << std::endl;
                noType = false;
            }
            //if none are met then it is a nonstandard type
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