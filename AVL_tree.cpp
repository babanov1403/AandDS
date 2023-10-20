#include <iostream>
#include <list>
#include <vector> 
using namespace std;


struct Node{
    int val, diff;
    Node* p, *left, *right;
    Node() 
     : p(nullptr)
     , left(nullptr)
     , right(nullptr)
     , diff(0){}
    Node(int x) 
     : p(nullptr)
     , left(nullptr)
     , right(nullptr)
     , val(x)
     , diff(0){}
    ~Node(){}
};
//find, erase, insert


class Binary_tree{
    void RotateRight(Node* u, Node* v);
    void RotateLeft(Node* u, Node* v);
    void updateBalance(Node* u);
    void reBalance(Node* u);
    Node* head;
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
        while(curr){
            if(curr->val < x){
                if(curr->right) curr = curr->right;
                else{
                    curr->right = tmp;
                    tmp->p = curr;
                }
            }
            else{
                if(curr->left) curr = curr->left;
                else{
                    curr->left = tmp;
                    tmp->p = curr;
                }
            }
        }
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
    void updateBalance(Node* u){//helper func for reBalance
        if(u->diff < -1 || u->diff > 1){
            reBalance(u);
            return;
        }
        if(u->p){
            if((u->p)->left == u) (u->p)->diff++;
            else if((u->p)->right == u) (u->p)->diff--;
            if((u->p)->diff != 0) updateBalance(u->p);
        }
    }
    void reBalance(Node* u){
        if(u->diff == -2){//diff == -2 => right tree deeper than left tree => we will do left turn
            if(u->right){
                if(u->right->diff == -1){
                    RotateLeft(u, u->right);
                    u->diff = 0;
                    u->right->diff = 0;
                    return;
                }
                else if(u->right->diff == 0){
                    RotateLeft(u, u->left);
                    u->diff = -1;
                    u->right->diff = 1;
                }
                else if()//далее большие повороты
            }
        }
        else if(u->diff == 2){
            //ПОСЧТИТАТЬ ВРАЩЕНИЯ
            if(u->left){
                if(u->left->diff == -1){
                    RotateLeft(u, u->right);
                    u->diff = 0;
                    u->right->diff = 0;
                    return;
                }
                else if(u->left->diff == 0){
                    RotateRight(u, u->left);
                    u->diff = -1;
                    u->right->diff = 1;
                }
            }
        }
    } 

};

int main(){
    return 0;
}