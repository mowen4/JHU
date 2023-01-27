#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Triangle
{
    private:
        int a, b ,c;   

    public:
         
        //Triangle types, for unit testing
        enum TriangleType {equilateral, isosceles, scalene, invalid};
        TriangleType type;   
     

        Triangle(int x, int y, int z)
        {
            a = x;
            b = y;
            c = z;
        }


        void determineTriangleType()
        {

            //std::cout << a << b << c << std::endl;
            
            //check if trianle has valid side lengths
            if(isValidTriangle())
            {
                 //if a==b and b==c then a==b==c
                if (a == b && b == c)
                {
                    std::cout << "It is Equilateral" << std::endl;
                    type = equilateral;
                }
                //if any pair of sides is equal then it is also Isosceles
                else if (a == b || a == c || b == c)
                {
                    std::cout << "It is Isosceles" << std::endl;
                    type = isosceles;
                }
                //otherwise it is scalene
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

