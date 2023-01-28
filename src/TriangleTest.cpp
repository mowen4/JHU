
// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "InputOutput.cpp"
#include "Triangle.cpp"

using namespace std;

//program entry point
int main()
{
    //array to hold the user input values
    int sides[3] = {0,0,0};

    InputOutput io;
    //gets user input and saves into sides array
    io.getInputs(sides);

    //creates triangle object using sides array
    Triangle t(sides[0],sides[1],sides[2]);

    //check the triangle type and output it to the console
    t.determineTriangleType();

    //return zero for successful completion
    return 0;
}



