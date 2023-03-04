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
  Probability(double p);
  void setProbability(double p);
  double getProbability() const;
  Probability &operator=(double p);
  Probability operator&(const Probability &other) const;
  Probability operator|(const Probability &other) const;
  Probability operator^(const Probability &other) const;
  Probability operator-(const Probability &other) const;
  Probability operator~() const;

  friend std::ostream& operator<<(std::ostream& os, const Probability& prob);

private:
  double p;
};

Probability::Probability(double p) {
  if (p < 0 || p > 1.0) {
    cerr << "Invalid probability value: " << p
         << " Setting probability to default value of 0.0" << endl;
    p = 0.0;
  }
  setProbability(p);
}

void Probability::setProbability(double p) {
  if (p < 0 || p > 1) {
    cerr << "Invalid probability value: " << p << endl;
    exit(1);
  }
  this->p = p;
}

double Probability::getProbability() const { return p; }

Probability Probability::operator&(const Probability &other) const {
  return Probability(p * other.p);
}

Probability Probability::operator|(const Probability &other) const {
  return Probability(p + other.p - (p * other.p));
}

Probability Probability::operator^(const Probability &other) const {
  return Probability((p * (1 - other.p)) + ((1 - p) * other.p));
}

Probability Probability::operator-(const Probability &other) const {
  return Probability(p * (1 - other.p));
}

Probability Probability::operator~() const { return Probability(1 - p); }

Probability &Probability::operator=(double p) {
  setProbability(p);
  return *this;
}

std::ostream& operator<<(std::ostream &os, const Probability &probability) {
  os << probability.p;
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
