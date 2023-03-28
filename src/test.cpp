#include <iostream>
#include <memory>

using namespace std;

template <typename T>
class TreeNode{
public:
    T data;
    shared_ptr<TreeNode<T>> left;
    shared_ptr<TreeNode<T>> right;

    TreeNode(T val){
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

template <typename T>
class BST{
public:
    shared_ptr<TreeNode<T>> root;

    BST(){
        root = nullptr;
    }

    void insert(T val){
        shared_ptr<TreeNode<T>> node(new TreeNode<T>(val));

        if(root == nullptr){
            root = node;
            return;
        }

        shared_ptr<TreeNode<T>> curr = root;

        while(true){
            if(val < curr->data){
                if(curr->left == nullptr){
                    curr->left = node;
                    return;
                }
                curr = curr->left;
            }
            else{
                if(curr->right == nullptr){
                    curr->right = node;
                    return;
                }
                curr = curr->right;
            }
        }
    }

    shared_ptr<TreeNode<T>> search(T val){
        shared_ptr<TreeNode<T>> curr = root;

        while(curr != nullptr){
            if(curr->data == val){
                return curr;
            }
            else if(val < curr->data){
                curr = curr->left;
            }
            else{
                curr = curr->right;
            }
        }

        return nullptr;
    }

    void inorderTraversal(shared_ptr<TreeNode<T>> node){
        if(node != nullptr){
            inorderTraversal(node->left);
            cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

    void print(){
        inorderTraversal(root);
        cout << endl;
    }
};

int main(){
    BST<int> bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(1);
    bst.insert(9);

    bst.print();

    shared_ptr<TreeNode<int>> node = bst.search(7);
    if(node != nullptr){
        cout << "Found: " << node->data << endl;
    }
    else{
        cout << "Not found." << endl;
    }

    return 0;
}
