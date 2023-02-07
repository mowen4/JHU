//Test scaffold for Statistic

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Statistic.cpp"
#include <vector>
#include <cmath>

//validate that GetAverage outputs the expected value
TEST_CASE("Average values added to vector", "[getAverage]")
{
    Statistic stat;
    stat.add(2.0);    
    stat.add(4.0);
    REQUIRE(stat.getAverage() == 3.0);    
}

//validate that getStandardDeviation outputs the expected value
TEST_CASE("Get standard deviation of values added to vector", "[getStandardDeviation]")
{
    Statistic stat;
    double a = 5.0;
    double b = 9.0;
    double c = 13.0;
    double numElements = 3;
    stat.add(a);    
    stat.add(b);
    stat.add(c);
    //pow(element - mean, 2);
    double mean = (a + b + c) / numElements;    
    double sumOfMeanDifferenceSquared = pow(a - mean, 2) + pow(b - mean, 2) + pow(c - mean, 2);
    double stdDev = sqrt(sumOfMeanDifferenceSquared/numElements);
    double result = stat.getStandardDeviation();
    REQUIRE(stat.getStandardDeviation() == stdDev);    
}

//validate that getAverage() outputs the expected value after adding a vector
TEST_CASE("Add a vector of items to statistic and compute average", "[addList]")
{
    Statistic stat;
    vector<double> elements {1.0, 2.0, 3.0, 4.0, 5.0};
    stat.addList(elements);            
    REQUIRE(stat.getAverage() == 3.0);    
}