#include <iostream>
#include <memory>
#include <string>
#include <map>

using namespace std;


/// @brief Node abstract class that is the parent to the other derived classes
/// used to create expression trees
class Node {
public:
    virtual ~Node() {};
    virtual double evaluate() const = 0;
    virtual std::shared_ptr<Node> derivative(const std::string& variable) const = 0;
    virtual void print(std::ostream& os) const = 0;    
    friend std::ostream& operator<<(std::ostream& os, const Node& node);        
};

/// @brief Overload to the << operator 
/// @param os 
/// @param node 
/// Calls the node's print function which will call the overloaded
/// print function in the derived class. 
/// @return 
std::ostream& operator<<(std::ostream& os, const Node& node) {
    node.print(os);
    return os;
}

/// @brief Constant class. Inherits from the Node class. Constant nodes
/// hold a double value. 
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


/// @brief Constructor for the Constant node. Assigns the value passed in to the
/// private double variable held in the Constant class.
/// @param val 
Constant::Constant(double val) {
    value = val;
}

/// @brief Implementation of the evaluate function. Returns the value held
/// by this Constant node.
/// @return 
double Constant::evaluate() const {
    return value;
}

/// @brief Derivative of Constants always return 0.0.
/// @param variable 
/// @return 
std::shared_ptr<Node> Constant::derivative(const std::string& variable) const {
    return std::make_shared<Constant>(0.0);
}

/// @brief Print function writes the private double value to cout. 
/// @param os 
void Constant::print(std::ostream& os) const {
    os << value;
}

/// @brief overload for the << operator for Constant nodes
/// @param os 
/// @param cons 
/// @return 
std::ostream& operator<<(std::ostream& os, const Constant& cons) {
    os << cons.value;
    return os;
}


/// @brief Variable class. This class inherits from Node and holds a string
/// variable name. 
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

/// @brief map that holds the association between variables and their values
std::map<std::string, double> Variable::variables = {
    {"Xray", 2.0}, {"Yellow", 3.0}, {"Zebra", 5.0}
};

/// @brief Variable constructor. Sets the string passed in to the variable name.
/// @param varName 
Variable::Variable(const std::string& varName) {
    name = varName;
}


/// @brief Variable evaluate implementation returns the double stored in the 
/// <string, double> map for the given name.
/// @return 
double Variable::evaluate() const {
    return variables[name];
}

/// @brief Derivative of a Variable node returns 1.0 if the derivative is taken
/// with respect to the current variable, otherwise 0.0.
/// @param variable 
/// @return 
std::shared_ptr<Node> Variable::derivative(const std::string& variable) const {
    if (name == variable) {
        return std::make_shared<Constant>(1.0);
    } else {
        return std::make_shared<Constant>(0.0);
    }
}

/// @brief Print function implementation for the Variable node. Prints out the 
/// name.
/// @param os 
void Variable::print(std::ostream& os) const {
    os << name;
}

/// @brief  overload for the << operator for Variable nodes 
/// @param os 
/// @param var 
/// @return 
std::ostream& operator<<(std::ostream& os, const Variable& var) {
    os << var.name;
    return os;
}


/// @brief Add class inherits from the Node class. It has a left and right value
/// that each can be another node so they are of the type std::shared_ptr<Node>.
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

/// @brief Constructor for Add. Sets the values for the left and right nodes.
/// @param leftNode 
/// @param rightNode 
Add::Add(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode)
    : left(leftNode), right(rightNode) {}

/// @brief Evalutation of the Add node. Adds the recursive evaluation of
/// the left node to the recursive evlauation of the right node. 
/// @return 
double Add::evaluate() const {
    return left->evaluate() + right->evaluate();
}

/// @brief Derivative for an Add node returns a std::shared_ptr<Add> where the 
/// new left node is the derivative of the current left Node with respective
/// to the given variable and the new right node is the derivative of the 
/// current right node with respect to the given variable. 
/// @param variable 
/// @return 
std::shared_ptr<Node> Add::derivative(const std::string& variable) const {
    return std::make_shared<Add>(left->derivative(variable), right->derivative(variable));
}

/// @brief Add print function implementation. Prints the left and right node with
/// a + in between. Whole expression is wrapped in parentheses. 
/// @param os 
void Add::print(std::ostream& os) const {
    os << "(" << *left << "+" << *right << ")";
}

/// @brief overload for the << operator for Add nodes
/// @param os 
/// @param add 
/// @return 
std::ostream& operator<<(std::ostream& os, const Add& add) {
    os << "(" << *add.left << ") + (" << *add.right << ")";
    return os;
}

/// @brief Add class inherits from the Node class. It has a left and right value
/// that each can be another node so they are of the type std::shared_ptr<Node>.
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


/// @brief Constructor for Sub node. Sets the values for the left and right nodes.
/// @param leftNode 
/// @param rightNode 
Sub::Sub(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode)
    : left(leftNode), right(rightNode) {}

/// @brief Evalutation of the Sub node. Subtracts the recursive evaluation of
/// the left node from the recursive evlauation of the right node. 
/// @return 
double Sub::evaluate() const {
    return left->evaluate() - right->evaluate();
}

/// @brief Derivative for a Sub node returns a std::shared_ptr<Sub> where the 
/// new left node is the derivative of the current left Node with respective
/// to the given variable and the new right node is the derivative of the 
/// current right node with respect to the given variable. 
std::shared_ptr<Node> Sub::derivative(const std::string& variable) const {
    return std::make_shared<Sub>(left->derivative(variable), right->derivative(variable));
}

/// @brief Sub print function implementation. Prints the left and right node with
/// a - in between. Whole expression is wrapped in parentheses. 
void Sub::print(std::ostream& os) const {
    os << "(" << *left << "-" << *right << ")";
}


/// @brief overload for the << operator for Sub nodes
/// @param os 
/// @param sub 
/// @return 
std::ostream& operator<<(std::ostream& os, const Sub& sub) {
    os << "(" << *sub.left << ") - (" << *sub.right << ")";
    return os;
}

/// @brief Mul class inherits from the Node class. It has a left and right value
/// that each can be another node so they are of the type std::shared_ptr<Node>.
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

/// @brief Mul Node constructor. Assigns the left and right node values. 
/// @param leftNode 
/// @param rightNode 
Mul::Mul(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode){
    left = leftNode;
    right = rightNode;
} 

/// @brief Evalutation of the Mul node. Multiplies the recursive evaluation of
/// the left node to the recursive evlauation of the right node. 
double Mul::evaluate() const {
    return left->evaluate() * right->evaluate();
}

/// @brief Derivative for a Mul node returns a std::shared_ptr<Add> where the 
/// new left node is a std::shared_ptr<mul>. This new left node has a left Node
/// that is the same as the current left node and the right node is the recursive
/// derivative of current right node with respect to the given variable. The right
/// node of the returned std::shared_ptr<Add> is also a new std::shared_ptr<Mul>. 
/// This one has a left node that is the recursive derivative of the current
/// left node and the right node is the same as the current right node.
std::shared_ptr<Node> Mul::derivative(const std::string& variable) const {
    return std::make_shared<Add>(
        std::make_shared<Mul>(left, right->derivative(variable)),
        std::make_shared<Mul>(left->derivative(variable), right)
    );
}

/// @brief Mul print function implementation. Prints the left and right node with
/// a * in between. Whole expression is wrapped in parentheses. 
void Mul::print(std::ostream& os) const {
    os << "(" << *left << "*" << *right << ")";
}

/// @brief overload for the << operator for Mul nodes
/// @param os 
/// @param mul 
/// @return 
std::ostream& operator<<(std::ostream& os, const Mul& mul) {
    os << "(" << *mul.left << ") * (" << *mul.right << ")";
    return os;
}


/// @brief Div class inherits from the Node class. It has a left and right value
/// that each can be another node so they are of the type std::shared_ptr<Node>.
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


/// @brief Div Node constructor. Assigns the left and right node values. 
/// @param leftNode 
/// @param rightNode 
Div::Div(std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode){
    left = leftNode;
    right = rightNode;
}

/// @brief Evalutation of the Div node. Divides the recursive evaluation of
/// the left node from the recursive evlauation of the right node. 
double Div::evaluate() const {
    return left->evaluate() / right->evaluate();
}

/// @brief Derivative of the Div node returns a std::shared_ptr<Div>. The left node
/// of this returned value is a std::shared_ptr<Sub> and the right node is a 
/// std::shared_ptr<Mul>. In the <Sub> node, the left node is a std::shared_ptr<Mul>
/// where the left node is a recursive derivative of the current left node with
/// respect to the given variable and the right node is the same as the current
/// right node. The right node in the <Sub> node is a second std:shared_ptr<Mul>
/// which holds the current left node as its left value and the recursive derivative
/// of the current right node with respect to the given variable as its right node. 
/// The right node of returned <Div> node is a std::shared_ptr<Mul> whose left
/// and right nodes are the current right node. 
/// @param variable 
/// @return 
std::shared_ptr<Node> Div::derivative(const std::string& variable) const {
    return std::make_shared<Div>(
        std::make_shared<Sub>(
            std::make_shared<Mul>(left->derivative(variable), right),
            std::make_shared<Mul>(left, right->derivative(variable))
        ),
        std::make_shared<Mul>(right, right)
    );
}


/// @brief Div print function implementation. Prints the left and right node with
/// a / in between. Whole expression is wrapped in parentheses. 
void Div::print(std::ostream& os) const {
    os << "(" << *left << "/" << *right << ")";
}

/// @brief overload for the << operator for Div nodes
/// @param os 
/// @param div 
/// @return 
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
    std::cout << "Derivative of the tree with respect to the variable Xray: " << std::endl;
    std::shared_ptr<Node> derivative = node->derivative("Xray");
    std::cout << *derivative << std::endl;

    std::cout << "Derivative evaluation result:" << std::endl;
    std::cout << derivative->evaluate() << std::endl;


    

    return 0;
}