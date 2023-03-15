#include <string>
#include <iostream>

#include "json/json.h"
#include "jsoncpp.cpp"

using namespace std;
 
// Class to represent the nodes of syntax tree
class node
{
public:
    string name;
    double constant;
    node *left = NULL, *right = NULL;
    node(string x)
    {
        name = x;
    }

    node(double d)
    {
        constant = d;
    }
};

class Add: public node
{
    public:
        Add(node lhs, node rhs) : node("+")
        {
            left = &lhs;
            right = &rhs;
        };
};

class Sub: public node
{
    public:
        Sub(node lhs, node rhs) : node("-")
        {
            left = &lhs;
            right = &rhs;
        };
};

class Div: public node
{
    public:
        Div(node lhs, node rhs) : node("/")
        {
            left = &lhs;
            right = &rhs;
        };
};

class Mul: public node
{
    public:
        Mul(node lhs, node rhs) : node("*")
        {
            left = &lhs;
            right = &rhs;
        };
};

class Constant: public node
{
    public:
        Constant(double d) : node(d)
        {

        }
};

class Variable: public node
{
    public:
        Variable(string x) : node(x)
        {

        }
};
 
// Utility function to return the integer value
// of a given string
int toInt(string s)
{
    int num = 0;
         
    // Check if the integral value is
    // negative or not
    // If it is not negative, generate the number
    // normally
    if(s[0]!='-')
        for (int i=0; i<s.length(); i++)
            num = num*10 + (int(s[i])-48);
    // If it is negative, calculate the +ve number
    // first ignoring the sign and invert the
    // sign at the end
    else
    {
      for (int i=1; i<s.length(); i++)
        num = num*10 + (int(s[i])-48);
      num = num*-1;
    }
     
    return num;
}
 
// This function receives a node of the syntax tree
// and recursively evaluates it
int eval(node* root)
{
    // empty tree
    if (!root)
        return 0;
 
    // leaf node i.e, an integer
    if (!root->left && !root->right)
        return toInt(root->name);
 
    // Evaluate left subtree
    int l_val = eval(root->left);
 
    // Evaluate right subtree
    int r_val = eval(root->right);
 
    // Check which operator to apply
    if (root->name=="+")
        return l_val+r_val;
 
    if (root->name=="-")
        return l_val-r_val;
 
    if (root->name=="*")
        return l_val*r_val;
 
    return l_val/r_val;
}
 
//driver function to check the above program
int main()
{
    // create a syntax tree
    node *root = new node("+");
    root->left = new node("*");
    root->left->left = new node("5");
    root->left->right = new node("-4");
    root->right = new node("-");
    root->right->left = new node("100");
    root->right->right = new node("20");
    cout << eval(root) << endl;
 
    delete(root);
 
    root = new Sub(Add(Add(Variable("5"), Variable("10")), Variable("5")), Variable("10"));
 
    cout << eval(root);
    return 0;
}