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
    Constant(double value) : value_(value) {}
    double evaluate() const override { return value_; }
    std::shared_ptr<Node> derivative(const std::string& variable) const override { 
        return std::make_shared<Constant>(0.0);
    }
    void print(std::ostream& os) const override { os << value_; }
    friend std::ostream& operator<<(std::ostream& os, const Constant& cons) {
        os << cons.value_;
        return os;
    }
private:
    double value_;
};

class Variable : public Node {
public:
    Variable(const std::string& name){
        name_ = name;
    }
    static std::map<std::string, double> variables;
    double evaluate() const override { return variables[name_]; }
    std::shared_ptr<Node> derivative(const std::string& variable) const override {
        if (name_ == variable) {
            return std::make_shared<Constant>(1.0);
        } else {
            return std::make_shared<Constant>(0.0);
        }
    }
    void print(std::ostream& os) const override { os << name_; }
    friend std::ostream& operator<<(std::ostream& os, const Variable& var) {
        os << var.name_;
        return os;
    }
private:    
    std::string name_;
};

std::map<std::string, double> Variable::variables = {
    {"Xray", 1.5}, {"Yellow", 2.0}, {"Zebra", 3.0}
};

class Add : public Node {
public:
    Add(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : left_(left), right_(right) {}
    double evaluate() const override { return left_->evaluate() + right_->evaluate(); }
    std::shared_ptr<Node> derivative(const std::string& variable) const override {
        return std::make_shared<Add>(left_->derivative(variable), right_->derivative(variable));
    }
    void print(std::ostream& os) const override { os << "(" << *left_ << "+" << *right_ << ")"; }
    friend std::ostream& operator<<(std::ostream& os, const Add& add) {
        os << "(" << *add.left_ << ") + (" << *add.right_ << ")";
        return os;
    }
private:
    std::shared_ptr<Node> left_, right_;
};

class Sub : public Node {
public:
    Sub(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : left_(left), right_(right) {}
    double evaluate() const override { return left_->evaluate() - right_->evaluate(); }
    std::shared_ptr<Node> derivative(const std::string& variable) const override {
        return std::make_shared<Sub>(left_->derivative(variable), right_->derivative(variable));
    }
    void print(std::ostream& os) const override { os << "(" << *left_ << "-" << *right_ << ")"; }
    friend std::ostream& operator<<(std::ostream& os, const Sub& sub) {
        os << "(" << *sub.left_ << ") - (" << *sub.right_ << ")";
        return os;
    }
private:
    std::shared_ptr<Node> left_, right_;
};

class Mul : public Node {
public:
    Mul(std::shared_ptr<Node> left, std::shared_ptr<Node> right) : left_(left), right_(right) {}
    double evaluate() const override { return left_->evaluate() * right_->evaluate(); }
    std::shared_ptr<Node> derivative(const std::string& variable) const override {
        return std::make_shared<Add>(
            std::make_shared<Mul>(left_, right_->derivative(variable)),
            std::make_shared<Mul>(left_->derivative(variable), right_)
        );
    }
    void print(std::ostream& os) const override { os << "(" << *left_ << "*" << *right_ << ")"; }
    friend std::ostream& operator<<(std::ostream& os, const Mul& mul) {
        os << "(" << *mul.left_ << ") * (" << *mul.right_ << ")";
        return os;
    }
private:
    std::shared_ptr<Node> left_, right_;
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
    std::cout << *node << std::endl;

    std::shared_ptr<Node> derivative = node->derivative("Xray");
    std::cout << *derivative << std::endl;

    return 0;
}