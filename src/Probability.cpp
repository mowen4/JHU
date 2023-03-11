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

/// @brief A class for handling probabilites 
///
/// Holds a probability value as a variable that indicates the liklihood of the
/// event represented by the probability object. Contains overloaded operators 
/// for performing certain probability computations.  
class Probability {
public:  
  Probability(double probability);
  void setProbability(double probability);
  double getProbability() const;
  Probability &operator=(double probability);  
  Probability &operator&=(const Probability& right);  
  Probability &operator|=(const Probability& right);  
  Probability &operator^=(const Probability& right);  
  Probability &operator-=(const Probability& right);  

  friend std::ostream &operator<<(std::ostream &os, const Probability &prob);

private:
  double probability;
};

/// @brief Constructs a new probability object with the passed in probability 
///        value.
/// Valid values are between 0.0 and 1.0. Values outside this range are set to
/// the default value of 0.0
/// @param probability 
Probability::Probability(double probability) {
  if (probability < 0 || probability > 1.0) {
    cerr << "Invalid probability value: " << probability
         << " Setting probability to default value of 0.0" << endl;
    probability = 0.0;
  }
  setProbability(probability);
}


// Member functions

/// @brief Sets the probability variable to the given probability value.
/// Sets the value of the probability private variable. Valid values are between
/// 0.0 and 1.0. If an invalid value is passed, a default value of 0.0 is set
/// instead.
/// @param probability 
void Probability::setProbability(double probability) {
  if (probability < 0 || probability > 1) {
    cerr << "Invalid probability value: " << probability
         << " Setting probability to default value of 0.0" << endl;
    probability = 0.0;
  }
  this->probability = probability;
}

/// @brief Returns the probability value of this Probability object.
/// @return 
double Probability::getProbability() const { return probability; }

/// @brief Assignment operator overload. Assigns the double parameter
///        to the object left of the =. This sets the private probability 
///        of that object to the value of the double or a default value if 
///        an invalid value is passed.
/// @param probability 
/// @return 
Probability& Probability::operator=(double probability) {
  setProbability(probability);
  return *this;
}

/// @brief Overload to the &= operator that assigns a new probability to the 
///        variable left of the operator equal to the probability of A & B
/// @param right The Probability object to the right of the &= operator
/// @return 
Probability& Probability::operator&=(const Probability& right) {
    setProbability(getProbability() * right.getProbability());
    return *this;
}

/// @brief Overload to the |= operator that assigns a new probability to the
///        variable left of the operator equal to the probability of A | B
/// @param right The Probability object to the right of the |= operator
/// @return 
Probability& Probability::operator|=(const Probability& right) {
    auto left = getProbability();
    setProbability((left + right.getProbability())
                    - (left * right.getProbability()));
    return *this;
}

/// @brief Overload to the ^= operator that assigns a new probability to the 
///        variable left of the operator equal to the probability of A ^ B
/// @param right The Probability object to the right of the ^= operator
/// @return 
Probability& Probability::operator^=(const Probability& right) {
    auto left = getProbability();
    setProbability((left * (1 - right.getProbability())) 
                  + (right.getProbability() * (1 - left)));
    return *this;
}

/// @brief  Overload to the -= operator that assigns a new probability to the 
///         variable left of the oeprator equal to the probability of A - B
/// @param right The Probability object to the right of the ^= operator
/// @return 
Probability& Probability::operator-=(const Probability& right) {
    auto left = getProbability();
    setProbability(left * (1 - right.getProbability()));
    return *this;
}

// Non-Member functions

/// @brief Binary overload of the & operator. Computes A & B by calling the &=
///        operator.
/// @param left Probability object to the left of the operator
/// @param right Probability object to the right of the operator
/// @return 
Probability operator&(Probability &left, const Probability &right){  
  Probability temp(left);
  return temp &= right;
}

/// @brief Binary overload of the | operator. Computes A | B by calling the |=
///        operator.
/// @param left Probability object to the left of the operator
/// @param right Probability object to the right of the operator
/// @return 
Probability operator|(Probability &left, const Probability &right){
  Probability temp(left);
  return temp |= right;
}

/// @brief Binary overload of the ^ operator. Computes A ^ B by calling the ^=
///        operator.
/// @param left Probability object to the left of the operator
/// @param right Probability object to the right of the operator
/// @return 
Probability operator^(Probability &left, const Probability &right){
  Probability temp(left);
  return temp ^= right;
}

/// @brief Binary overload of the - operator. Computes A - B by calling the -=
///        operator.
/// @param left Probability object to the left of the operator
/// @param right Probability object to the right of the operator
/// @return 
Probability operator-(Probability& left, const Probability& right){
  Probability temp(left);
  return temp -= right;
}

/// @brief Overload of the ~ operator. Returns a Probability object with a 
///        probability that is the complement of the passed in probability.
/// @param right Probability object to the right of the operator
/// @return 
Probability operator~(const Probability& right){
  return Probability(1 - right.getProbability());
}


/// @brief Overloads the << operator to allow Probability objects to be printed
///        to the output stream std:cout
/// @param os 
/// @param probability 
/// @return 
std::ostream &operator<<(std::ostream &os, const Probability &probability) {
  os << probability.probability;
  return os;
}

/// @brief Runs tests on a collection of probability values
///
/// Probability values are stored in a file in json format. This class will read
/// in pairs of probabilities, run various probability operations against each,
/// and print the results to the screen
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

      cout <<"Initial Values" << endl;
      cout << "a:  " << a << endl;
      cout << "b:  " << b << endl;

      cout <<"Bitwise operator tests" << endl;

      cout << "a & b:  " << (a & b) << endl;
      cout << "a | b:  " << (a | b) << endl;
      cout << "a ^ b:  " << (a ^ b) << endl;
      cout << "a - b:  " << (a - b) << endl;
      cout << "~a:  " << (~a) << endl;
      cout << "~b:  " << (~b) << "\n" << endl;

      cout <<"Compound assignment operator tests" << endl;

      a &= b;
      cout << "a &= b:  " << "a: " << a << "  b: " << b << endl;
      //reset a,b
      a = pValueA; b = pValueB;
      a |= b;
      cout << "a |= b:  " << "a: " << a << "  b: " << b << endl;
      //reset a,b
      a = pValueA; b = pValueB;
      a ^= b;
      cout << "a ^= b:  " << "a: " << a << "  b: " << b << endl;
      //reset a,b
      a = pValueA; b = pValueB;
      a -= b;
      cout << "a -= b:  " << "a: " << a << "  b: " << b << endl;
      cout << "\n" << endl;


    }
  }
}

/// @brief int main is the entry point to the program.
/// @return 
int main() {

  JsonProbabilityTests jsonTestHarness("probabilityTests.json");
  jsonTestHarness.RunTests();

  // Probability a(0.5);
  // Probability b(0.25);

  // cout << "a: " << a << endl;
  // cout << "b: " << b << endl;
  // cout << "a & b: " << (a & b) << endl;
  // cout << "a | b: " << (a | b) << endl;
  // cout << "a ^ b: " << (a ^ b) << endl;
  // cout << "a - b: " << (a - b) << endl;
  // cout << "~a: " << (~a) << endl;
  // cout << "~b: " << (~b) << endl;

  // cout << "Demonstrating invalid probability case: Probability c(1.5); \n";
  // Probability c(1.5);
  // cout << "c: " << c << endl;
  // cout << "a | c: " << (a | c) << endl;

  return 0;
}
