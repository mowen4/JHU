#include <memory>
#include <string>
#include <ostream>
#include <iostream>
#include <stdlib.h>

class ExpressionNode {
public:
    virtual double evaluate() const = 0;
};

class ValueNode : public ExpressionNode {
public:
    ValueNode(double value) : m_value(value) {}
    double evaluate() const override { return m_value; }
private:
    double m_value;
};

class BinaryOperatorNode : public ExpressionNode {
public:
    BinaryOperatorNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right)
        : m_left(left), m_right(right) {}
protected:
    std::shared_ptr<ExpressionNode> m_left, m_right;
};

class AddNode : public BinaryOperatorNode {
public:
    AddNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right)
        : BinaryOperatorNode(left, right) {}
    double evaluate() const override { return m_left->evaluate() + m_right->evaluate(); }
};

class SubtractNode : public BinaryOperatorNode {
public:
    SubtractNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right)
        : BinaryOperatorNode(left, right) {}
    double evaluate() const override { return m_left->evaluate() - m_right->evaluate(); }
};

class MultiplyNode : public BinaryOperatorNode {
public:
    MultiplyNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right)
        : BinaryOperatorNode(left, right) {}
    double evaluate() const override { return m_left->evaluate() * m_right->evaluate(); }
};

class DivideNode : public BinaryOperatorNode {
public:
    DivideNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right)
        : BinaryOperatorNode(left, right) {}
    double evaluate() const override { return m_left->evaluate() / m_right->evaluate(); }
};

// std::shared_ptr<ExpressionNode> createExpressionTree(const std::string& expression) {
//     // TODO: parse the expression and create the expression tree
// }

int main() {
    //std::string expression = "3 + 4 * 5";
    //std::shared_ptr<ExpressionNode> tree = createExpressionTree(expression);
    //std::cout << tree->evaluate() << std::endl; // should print "23"
    return 0;
}