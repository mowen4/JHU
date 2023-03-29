#include <iostream>
#include <memory>
#include <string>

class Node {
public:
    virtual ~Node() {}
    virtual double evaluate() const = 0;
    virtual std::string toString() const = 0;
};

class Constant : public Node {
public:
    Constant(double value) : value(value) {}
    virtual double evaluate() const override { return value; }
    virtual std::string toString() const override { return std::to_string(value); }
private:
    double value;
};

class Variable : public Node {
public:
    Variable(const std::string& name) : name(name) {}
    virtual double evaluate() const override { /* TODO: Implement variable evaluation */ }
    virtual std::string toString() const override { return name; }
private:
    std::string name;
};

class Add : public Node {
public:
    Add(const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right) : left(left), right(right) {}
    virtual double evaluate() const override { return left->evaluate() + right->evaluate(); }
    virtual std::string toString() const override { return "(" + left->toString() + "+" + right->toString() + ")"; }
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

class Sub : public Node {
public:
    Sub(const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right) : left(left), right(right) {}
    virtual double evaluate() const override { return left->evaluate() - right->evaluate(); }
    virtual std::string toString() const override { return "(" + left->toString() + "-" + right->toString() + ")"; }
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

class Mul : public Node {
public:
    Mul(const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right) : left(left), right(right) {}
    virtual double evaluate() const override { return left->evaluate() * right->evaluate(); }
    virtual std::string toString() const override { return "(" + left->toString() + "*" + right->toString() + ")"; }
private:
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

// Helper function to create shared_ptr from raw pointer
template<typename T>
std::shared_ptr<T> node_ptr(T* ptr) {
    return std::shared_ptr<T>(ptr);
}

int main() {
    std::shared_ptr<Node> node = node_ptr<Node>(new Add(
        node_ptr<Node>(new Mul(node_ptr<Node>(new Constant(2.3)), node_ptr<Node>(new Variable("Xray")))),
        node_ptr<Node>(new Mul(node_ptr<Node>(new Variable("Yellow")), 
            node_ptr<Node>(new Sub(node_ptr<Node>(new Variable("Zebra")), node_ptr<Node>(new Variable("Xray"))))
        )
    )));

    // std::shared_ptr<Node> node2 = new Add(new Mul(new Constant(2.3), new Variable("Xray")), 
    //                 new Mul( new Variable("Yellow"), new Sub(new Variable("Zebra"), new Variable("Xray"))));

    std::cout << node->toString() << std::endl;
    std::cout << "Result: " << node->evaluate() << std::endl;
    return 0;
}