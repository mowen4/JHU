#include "jsoncpp.cpp"
#include "json/json.h"
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <vector>

using namespace std;

class Probability {
public:
  Probability(double probability);
  void setProbability(double probability);
  double getProbability() const;
  Probability &operator=(double probability);
  // Probability operator&(const Probability &other) const;
  // Probability operator|(const Probability &other) const;
  // Probability operator^(const Probability &other) const;
  // Probability operator-(const Probability &other) const;
  // Probability operator~() const;

  friend std::ostream &operator<<(std::ostream &os, const Probability &prob);

private:
  double probability;
};

Probability::Probability(double probability) {
  if (probability < 0 || probability > 1.0) {
    cerr << "Invalid probability value: " << probability
         << " Setting probability to default value of 0.0" << endl;
    probability = 0.0;
  }
  setProbability(probability);
}

void Probability::setProbability(double probability) {
  if (probability < 0 || probability > 1) {
    cerr << "Invalid probability value: " << probability << endl;
    exit(1);
  }
  this->probability = probability;
}

double Probability::getProbability() const { return probability; }

Probability operator&(const Probability &left, const Probability &right){
  return Probability(left.getProbability() * right.getProbability());
}

Probability operator|(const Probability &left, const Probability &right){
  return Probability((left.getProbability() + right.getProbability())
                    - (left & right).getProbability());
}

Probability operator^(const Probability &left, const Probability &right){
  return Probability((left.getProbability() * (1 - right.getProbability())) 
                  + (right.getProbability() * (1 - left.getProbability())));
}

Probability operator-(const Probability& left, const Probability& right){
  return Probability(left.getProbability() * (1 - right.getProbability()));
}

Probability operator~(const Probability& probability){
  return Probability(1 - probability.getProbability());
}

Probability& Probability::operator=(double probability) {
  setProbability(probability);
  return *this;
}

std::ostream &operator<<(std::ostream &os, const Probability &probability) {
  os << probability.probability;
  return os;
}

/// @brief Runs tests on a collection of hands stored in json format
///
/// Hands are stored in a file in json format. This class will read
/// in each pair of hands anddetermine the winner and print it to the screen
class JsonProbabilityTests {
public:
  JsonProbabilityTests(string jsonFile);
  void RunTests();

private:
  string fileLocation;
  Json::Value jsonData;
};

JsonProbabilityTests::JsonProbabilityTests(string fileName) {
  fileLocation = fileName;
  std::ifstream file(fileLocation);
  file >> jsonData;
};

void JsonProbabilityTests::RunTests() {
  for (Json::ValueIterator itr = jsonData.begin(); itr != jsonData.end();
       itr++) {
    std::string propertyName = itr.name();

    cout << "\nInitiating tests of the following type: " << propertyName
         << "\n";
    Json::Value currentCategory = jsonData[propertyName];

    for (const auto &probPair : currentCategory) {
      double pValueA = 0.0, pValueB = 0.0;
      for (const auto &val : probPair["p1"]) {
        pValueA = val.asDouble();
      }
      for (const auto &val : probPair["p2"]) {
        pValueB = val.asDouble();
      }
      Probability a(pValueA);
      Probability b(pValueB);

      cout << "a: " << a << endl;
      cout << "b: " << b << endl;
      cout << "a & b: " << (a & b) << endl;
      cout << "a | b: " << (a | b) << endl;
      cout << "a ^ b: " << (a ^ b) << endl;
      cout << "a - b: " << (a - b) << endl;
      cout << "~a: " << (~a) << endl;
      cout << "~b: " << (~b) << "\n" << endl;
    }
  }
}

int main() {

  JsonProbabilityTests jsonTestHarness("probabilityTests.json");
  jsonTestHarness.RunTests();

  Probability a(0.5);
  Probability b(0.25);

  cout << "a: " << a << endl;
  cout << "b: " << b << endl;
  cout << "a & b: " << (a & b) << endl;
  cout << "a | b: " << (a | b) << endl;
  cout << "a ^ b: " << (a ^ b) << endl;
  cout << "a - b: " << (a - b) << endl;
  cout << "~a: " << (~a) << endl;
  cout << "~b: " << (~b) << endl;

  cout << "Demonstrating invalid probability case: Probability c(1.5); \n";
  Probability c(1.5);
  cout << "c: " << c << endl;
  cout << "a | c: " << (a | c) << endl;

  // int i = 1;
  // int j = i++;
  // cout << i;
  // cout << j;

  return 0;
}