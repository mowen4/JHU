#include <iostream>
#include <memory>
#include <string>
#include <map>

using namespace std;


//Node.h
class Node {
public:
    virtual ~Node() {};
    virtual double evaluate() const = 0;
    virtual std::shared_ptr<Node> derivative(const std::string& variable) const = 0;
    virtual void print(std::ostream& os) const = 0;    
    friend std::ostream& operator<<(std::ostream& os, const Node& node);        
};

//Node.cpp
std::ostream& operator<<(std::ostream& os, const Node& node) {
    node.print(os);
    return os;
}

//Constant.h
class Constant : public Node {
public:
    Constant(double val);
    double evaluate() const override;
    std::shared_ptr<Node> derivative(const std::string& variable) const override;
    void print(std::ostream& os) const override;
    friend std::ostream& operator<<(std::ostream& os, const Constant& cons);
private:
    double value;
};

//Constant.cpp

Constant::Constant(double val) {
    value = val;
}

double Constant::evaluate() const {
    return value;
}

std::shared_ptr<Node> Constant::derivative(const std::string& variable) const {
    return std::make_shared<Constant>(0.0);
}

void Constant::print(std::ostream& os) const {
    os << value;
}

std::ostream& operator<<(std::ostream& os, const Constant& cons) {
    os << cons.value;
    return os;
}

//Variable.h

class Variable : public Node {
public:
    Variable(const std::string& varName);
    static std::map<std::string, double> variables;
    double evaluate() const override;
    std::shared_ptr<Node> derivative(const std::string& variable) const override;
    void print(std::ostream& os) const override;
    friend std::ostream& operator<<(std::ostream& os, const Variable& var);
private:
    std::string name;
};

//variable.cpp

std::map<std::string, double> Variable::variables = {
    {"Xray", 2.0}, {"Yellow", 3.0}, {"Zebra", 5.0}
};

Variable::Variable(const std::string& varName) {
    name = varName;
}

double Variable::evaluate() const {
    return variables[name];
}

std::shared_ptr<Node> Variable::derivative(const std::string& variable) const {
    if (name == variable) {
        return std::make_shared<Constant>(1.0);
    } else {
        return std::make_shared<Constant>(0.0);
    }
}

void Variable::print(std::ostream& os) const {
    os << name;
}

std::ostream& operator<<(std::ostream& os, const Variable& var) {
    os << var.name;
    return os;
}

//Add.h

class Add : public Node {
public:
    Add(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode);
    double evaluate() const override;
    std::shared_ptr<Node> derivative(const std::string& variable) const override;
    void print(std::ostream& os) const override;
    friend std::ostream& operator<<(std::ostream& os, const Add& add);
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

//Add.cpp

Add::Add(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode)
    : left(leftNode), right(rightNode) {}

double Add::evaluate() const {
    return left->evaluate() + right->evaluate();
}

std::shared_ptr<Node> Add::derivative(const std::string& variable) const {
    return std::make_shared<Add>(left->derivative(variable), right->derivative(variable));
}

void Add::print(std::ostream& os) const {
    os << "(" << *left << "+" << *right << ")";
}

std::ostream& operator<<(std::ostream& os, const Add& add) {
    os << "(" << *add.left << ") + (" << *add.right << ")";
    return os;
}

//Sub.h

class Sub : public Node {
public:
    Sub(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode);
    double evaluate() const override;
    std::shared_ptr<Node> derivative(const std::string& variable) const override;
    void print(std::ostream& os) const override;
    friend std::ostream& operator<<(std::ostream& os, const Sub& sub);
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};


//sub.cpp

Sub::Sub(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode)
    : left(leftNode), right(rightNode) {}

double Sub::evaluate() const {
    return left->evaluate() - right->evaluate();
}

std::shared_ptr<Node> Sub::derivative(const std::string& variable) const {
    return std::make_shared<Sub>(left->derivative(variable), right->derivative(variable));
}

void Sub::print(std::ostream& os) const {
    os << "(" << *left << "-" << *right << ")";
}

std::ostream& operator<<(std::ostream& os, const Sub& sub) {
    os << "(" << *sub.left << ") - (" << *sub.right << ")";
    return os;
}

//mul.cpp

class Mul : public Node {
public:
    Mul(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode);
    double evaluate() const override;
    std::shared_ptr<Node> derivative(const std::string& variable) const override;
    void print(std::ostream& os) const override;
    friend std::ostream& operator<<(std::ostream& os, const Mul& mul);
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

Mul::Mul(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode){
    left = leftNode;
    right = rightNode;
} 

double Mul::evaluate() const {
    return left->evaluate() * right->evaluate();
}

std::shared_ptr<Node> Mul::derivative(const std::string& variable) const {
    return std::make_shared<Add>(
        std::make_shared<Mul>(left, right->derivative(variable)),
        std::make_shared<Mul>(left->derivative(variable), right)
    );
}

void Mul::print(std::ostream& os) const {
    os << "(" << *left << "*" << *right << ")";
}

std::ostream& operator<<(std::ostream& os, const Mul& mul) {
    os << "(" << *mul.left << ") * (" << *mul.right << ")";
    return os;
}


//div.h

class Div : public Node {
public:
    Div(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode);
    double evaluate() const override;
    std::shared_ptr<Node> derivative(const std::string& variable) const override;
    void print(std::ostream& os) const override;
    friend std::ostream& operator<<(std::ostream& os, const Div& div);
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

//div.cpp
Div::Div(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode){
    left = leftNode;
    right = rightNode;
}

double Div::evaluate() const {
    return left->evaluate() / right->evaluate();
}

std::shared_ptr<Node> Div::derivative(const std::string& variable) const {
    return std::make_shared<Div>(
        std::make_shared<Sub>(
            std::make_shared<Mul>(left->derivative(variable), right),
            std::make_shared<Mul>(left, right->derivative(variable))
        ),
        std::make_shared<Mul>(right, right)
    );
}

void Div::print(std::ostream& os) const {
    os << "(" << *left << "/" << *right << ")";
}

std::ostream& operator<<(std::ostream& os, const Div& div) {
    os << "(" << *div.left << ") / (" << *div.right << ")";
    return os;
}

int main() {
    std::shared_ptr<Node> node = std::make_shared<Add>(
        std::make_shared<Mul>(std::make_shared<Constant>(2.3), std::make_shared<Variable>("Xray")),
        std::make_shared<Mul>(
            std::make_shared<Variable>("Yellow"),
            std::make_shared<Sub>(std::make_shared<Variable>("Zebra"), std::make_shared<Variable>("Xray"))
        )
    );
    std::cout << "Print out of the expression tree: " << std::endl;
    std::cout << *node << std::endl;
    std::cout << "Expression tree evalutation: " << std::endl;
    std::cout << node->evaluate() << std::endl;
    std::cout << "Derivative of the tree evalutation with respect to the variable Xray: " << std::endl;
    std::shared_ptr<Node> derivative = node->derivative("Xray");
    std::cout << *derivative << std::endl;

    Node* destructionTestNode = new Add(std::make_shared<Constant>(2.0), std::make_shared<Constant>(1.5));
    
    delete destructionTestNode;
    cout << endl;

    return 0;
}