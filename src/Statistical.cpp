#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Statistic
{
private:
    vector<double> v;
    long double numberOfElements = 0;
public:
    // Constructor: initialize private data
    //Statistic();
    // add an item to the statistics

    void add(double x)
    {
    // get average
        v.push_back(x);
        numberOfElements++;
        std::cout << "Value " << x << " inserted" <<std::endl;
        
    }

    float getAverage()
    {

        long double sum = 0;
        float average = 0.0;

        for(double element : v)
        {
            sum = sum + element;
        }
        average = sum / numberOfElements;

        //std::cout << "The average is: " << average << std::endl;

        return average;
    }

    float getStandardDeviation()
    {
        float standardDeviation = 0.0;
        float mean = getAverage();

        for(double element : v)
        {
            standardDeviation += pow(element - mean, 2);
        }

        float STD = sqrt(standardDeviation / numberOfElements);

        //std::cout << "The STD is: " << STD << std::endl;

        return STD;
    }
};

// program entry point
int main()
{

    Statistic s;
    s.add(1);
    s.add(2);
    s.add(3);
    s.add(4);
    s.add(5);
    s.add(6);
    s.add(7);
    s.add(8);
    s.add(9);
    s.add(10);

    std::cout << "The average of all values is: " << s.getAverage() << std::endl;
    std::cout << "The average standard deviation of all values is: " << s.getStandardDeviation() << std::endl;

    return 0;
};