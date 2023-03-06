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
  Probability operator&(const Probability &other) const;
  Probability operator|(const Probability &other) const;
  Probability operator^(const Probability &other) const;
  Probability operator-(const Probability &other) const;
  Probability operator~() const;

  friend std::ostream& operator<<(std::ostream& os, const Probability& prob);

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

Probability Probability::operator&(const Probability &other) const {
  return Probability(probability * other.probability);
}

Probability Probability::operator|(const Probability &other) const {
  return Probability(probability + other.probability - (probability * other.probability));
}

Probability Probability::operator^(const Probability &other) const {
  return Probability((probability * (1 - other.probability)) + ((1 - probability) * other.probability));
}

Probability Probability::operator-(const Probability &other) const {
  return Probability(probability * (1 - other.probability));
}

Probability Probability::operator~() const { return Probability(1 - probability); }

Probability &Probability::operator=(double probability) {
  setProbability(probability);
  return *this;
}

std::ostream& operator<<(std::ostream &os, const Probability &probability) {
  os << probability.probability;
  return os;
}

int main() {
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

  int i = 1;
  int j = i++;
  cout << i;
  cout << j;

  return 0;
}
