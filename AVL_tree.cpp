#include <iostream>
#include <list>
#include <vector> 
using namespace std;


struct Node{
    int val;
    Node* p, *left, *right;
    Node() 
     : p(nullptr)
     , left(nullptr)
     , right(nullptr){}
    Node(int x) 
     : p(nullptr)
     , left(nullptr)
     , right(nullptr)
     , val(x){}
    ~Node(){}
};
//find, erase, insert


class Binary_tree{
    void RotateRight(Node* u, Node* v);
    void RotateLeft(Node* u, Node* v);
    Node* head;
    vector<Node*> diff;
public:
    Binary_tree() : head(new Node){}
    ~Binary_tree(){
        //we need to delete all nodes
    }

    bool find(int x, Node* ptr){
        if(ptr){
            if(x == ptr->val) return true;
            if(x < ptr->val) return find(x, ptr->left);
            else return find(x, ptr->right);
        }
        else return false; 
        
    }

    void insert(int x){
        Node* tmp = new Node(x);
        Node* curr = head;
        while(curr->right || curr->left){
            if(curr->val <= x) curr = curr->left;
            else curr = curr->right;
        }
        if(curr->val <= x) curr->left = tmp;
        else curr->right = tmp;
    }
private:
    //combination of this two => big rotations
    void RotateRight(Node* u, Node* v){
        u->left = v->right;
        v->right = u;
        //change diff
    }
    void RotateLeft(Node* u, Node* v){
        u->right = v->left;
        v->left = u;
        //change diff
    }

};

int main(){
    return 0;
}