#include <string>
#include <iostream>
#include "json/json.h"
#include "jsoncpp.cpp"

using namespace std;
 
// Class to represent the nodes of syntax tree
class Node
{
public:
    string name;
    double constant;
    Node *left = NULL, *right = NULL;
    Node(string x)
    {
        name = x;
    }

    Node(double d)
    {
        constant = d;
    }
};

class Add: public Node
{
    public:
        Add(Node lhs, Node rhs) : Node("+")
        {
            left = &lhs;
            right = &rhs;
        };
};

class Sub: public Node
{
    public:
        Sub(Node lhs, Node rhs) : Node("-")
        {
            left = &lhs;
            right = &rhs;
        };
};

class Div: public Node
{
    public:
        Div(Node lhs, Node rhs) : Node("/")
        {
            left = &lhs;
            right = &rhs;
        };
};

class Mul: public Node
{
    public:
        Mul(Node lhs, Node rhs) : Node("*")
        {
            left = &lhs;
            right = &rhs;
        };
};

class Constant: public Node
{
    public:
        Constant(double d) : Node(d)
        {

        }
};

class Variable: public Node
{
    public:
        Variable(string x) : Node(x)
        {

        }
};

class ExpressionTree
{
    public:
        void printTree(Node* node){
            printTreeRecursive(node);
            cout << endl;
        }        
    private: 
        //Attempt at making the parentheses clever. Doesn't really work. May revisit, but it isn't a requirement so prolly not... TODO.
        // void printTreeRecursive(node* node){
        //     if(node == NULL){
        //         return;
        //     }
        //     //if there are child nodes, check the node types to determine
        //     //whether parentheses are required
        //     if(node->left != NULL && node->right != NULL){
        //         bool needParentheses = false;
        //         //if current node is + or -, need parentheses for * or / children
        //         if(node->name == "+" || node->name == "-"){
        //             if(node->left->name == "*" 
        //                 || node->left->name == "/"
        //                 || node->right->name == "*"
        //                 || node->right->name == "/"){
        //                 needParentheses = true;                        
        //             }
        //         }            
        //         //if current node is * or /, need parentheses for + or - children
        //         else if (node->name == "*" || node->name == "/") {            
        //             if (node->left->name == "+" 
        //                 || node->left->name == "-"
        //                 || node->right->name == "+"
        //                 || node->right->name == "-") {
        //                 needParentheses = true;
        //             }                
        //         }            
        //         if(needParentheses){
        //             cout << "(";
        //         }
        //         printTreeRecursive(node->left);
        //         cout << node->name << " ";
        //         printTreeRecursive(node->right);
        //         if(needParentheses){
        //             cout << ")";
        //         }       
        //     }   
        //     //otherwise just print the name
        //     else{
        //         cout << node->name << " ";
        //     }              
        // }    
        void printTreeRecursive(Node* node){
            if(node == NULL){
                return;
            }            
            cout << "(";
            printTreeRecursive(node->left);
            cout << node->name;
            printTreeRecursive(node->right);                
            cout << ")";                                               
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
int eval(Node* root)
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
    Node *root = new Node("+");
    root->left = new Node("*");
    root->left->left = new Node("5");
    root->left->right = new Node("-4");
    root->right = new Node("-");
    root->right->left = new Node("100");
    root->right->right = new Node("20");
    cout << eval(root) << endl;

    ExpressionTree tree;
    tree.printTree(root);    
 
    delete(root);
 
    root = new Sub(Add(Add(Variable("5"), Variable("10")), Variable("5")), Variable("10"));
 
    cout << eval(root);
    return 0;
}