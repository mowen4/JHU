#include <iostream>
#include <memory>
#include <string>
#include <map>

class Node {
public:
    virtual ~Node() {}
    virtual double evaluate() const = 0;
    virtual std::shared_ptr<Node> derivative(const std::string& variable) const = 0;
    virtual void print(std::ostream& os) const = 0;    
    // friend std::ostream& operator<<(std::ostream& os, const Node& node);
    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        node.print(os);
        return os;
    }
};

class Constant : public Node {
public:
    Constant(double val){
         value = val;
    } 
    double evaluate() const override { return value; }
    std::shared_ptr<Node> derivative(const std::string& variable) const override { 
        return std::make_shared<Constant>(0.0);
    }
    void print(std::ostream& os) const override { os << value; }
    friend std::ostream& operator<<(std::ostream& os, const Constant& cons) {
        os << cons.value;
        return os;
    }
private:
    double value;
};

class Variable : public Node {
public:
    Variable(const std::string& varName){
        name = varName;
    }
    static std::map<std::string, double> variables;
    double evaluate() const override { return variables[name]; }
    std::shared_ptr<Node> derivative(const std::string& variable) const override {
        if (name == variable) {
            return std::make_shared<Constant>(1.0);
        } else {
            return std::make_shared<Constant>(0.0);
        }
    }
    void print(std::ostream& os) const override { os << name; }
    friend std::ostream& operator<<(std::ostream& os, const Variable& var) {
        os << var.name;
        return os;
    }
private:    
    std::string name;
};

std::map<std::string, double> Variable::variables = {
    {"Xray", 2.0}, {"Yellow", 3.0}, {"Zebra", 5.0}
};

class Add : public Node {
public:
    Add(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode){
     left = leftNode;
     right = rightNode;
    }
    double evaluate() const override { return left->evaluate() + right->evaluate(); }
    std::shared_ptr<Node> derivative(const std::string& variable) const override {
        return std::make_shared<Add>(left->derivative(variable), right->derivative(variable));
    }
    void print(std::ostream& os) const override { os << "(" << *left << "+" << *right << ")"; }
    friend std::ostream& operator<<(std::ostream& os, const Add& add) {
        os << "(" << *add.left << ") + (" << *add.right << ")";
        return os;
    }
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

class Sub : public Node {
public:
    Sub(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode){
        left = leftNode;
        right = rightNode;
    } 
    double evaluate() const override { return left->evaluate() - right->evaluate(); }
    std::shared_ptr<Node> derivative(const std::string& variable) const override {
        return std::make_shared<Sub>(left->derivative(variable), right->derivative(variable));
    }
    void print(std::ostream& os) const override { os << "(" << *left << "-" << *right << ")"; }
    friend std::ostream& operator<<(std::ostream& os, const Sub& sub) {
        os << "(" << *sub.left << ") - (" << *sub.right << ")";
        return os;
    }
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

class Mul : public Node {
public:
    Mul(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode){
        left = leftNode;
        right = rightNode;
    } 
    double evaluate() const override { return left->evaluate() * right->evaluate(); }
    std::shared_ptr<Node> derivative(const std::string& variable) const override {
        return std::make_shared<Add>(
            std::make_shared<Mul>(left, right->derivative(variable)),
            std::make_shared<Mul>(left->derivative(variable), right)
        );
    }
    void print(std::ostream& os) const override { os << "(" << *left << "*" << *right << ")"; }
    friend std::ostream& operator<<(std::ostream& os, const Mul& mul) {
        os << "(" << *mul.left << ") * (" << *mul.right << ")";
        return os;
    }
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

class Div : public Node {
public:
    Div(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : left_(left), right_(right) {}
    double evaluate() const override { return left_->evaluate() / right_->evaluate(); }
    std::shared_ptr<Node> derivative(const std::string& variable) const override {
        return std::make_shared<Div>(
            std::make_shared<Sub>(
                std::make_shared<Mul>(left_->derivative(variable), right_),
                std::make_shared<Mul>(left_, right_->derivative(variable))
            ),
            std::make_shared<Mul>(right_, right_)
        );
    }
    void print(std::ostream& os) const override { os << "(" << *left_ << "/" << *right_ << ")"; }
    friend std::ostream& operator<<(std::ostream& os, const Div& div) {
        os << "(" << *div.left_ << ") / (" << *div.right_ << ")";
        return os;
    }

private:
    std::shared_ptr<Node> left_, right_;
};

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

    return 0;
}