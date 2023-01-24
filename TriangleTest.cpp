//hello world for the git repo
// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>

int main()
{
    std::cout << "Hello World" << std::endl;

    int number;

    srand(time(NULL));

    for (int i = 0; i <= 100; i++)
    {
        number = rand();
        std::cout << number << std::endl;
    }
}