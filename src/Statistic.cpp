#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <vector>


using namespace std;

class Statistic {

public:
  // Constructor: initialize private data
  // Statistic();

  // add an item to the statistics
  void add(double x) {
    // append x to the end of values
    values.push_back(x);
    // increment the number of elements variable since v is now one element
    // longer
    numberOfElements++;
    std::cout << "Value " << x << " inserted" << std::endl;
  }

  // append a list of input to the end of the values vector
  void addList(vector<double> input) {
    values.insert(values.end(), input.begin(), input.end());
    std::cout
        << "Appending all values to the end of the vector. New vector length: "
        << values.size() << "." << std::endl;
    numberOfElements += values.size();
  }

  // computes the average value of all the values in the vector
  double getAverage() const {
    long double sum = 0;
    double average = 0.0;

    for (double element : values) {
      sum = sum + element;
    }
    // Average = the sum of all elements in values divided by its length
    average = sum / numberOfElements;
    // std::cout << "The average is: " << average << std::endl;
    return average;
  }

  // computes the standard deviation of the values in the vector
  double getStandardDeviation() const {
    double standardDeviation = 0.0;
    double mean = getAverage();

    for (double element : values) {
      standardDeviation += pow(element - mean, 2);
    }

    double STD = sqrt(standardDeviation / numberOfElements);

    // std::cout << "The STD is: " << STD << std::endl;

    return STD;
  }

private:
  // values holds the collection of doubles over which we will compute the
  // average and std deviation
  vector<double> values;
  // the number of elements in v
  long double numberOfElements = 0;
};
