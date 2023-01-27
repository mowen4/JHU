//hello world for the git repo
// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "InputOutput.cpp"
#include "Triangle.cpp"

using namespace std;

int main()
{
    int sides[3] = {0,0,0};

    InputOutput io;
    io.getInputs(sides);

    Triangle t(sides[0],sides[1],sides[2]);
    t.determineTriangleType();

    return 0;
}



