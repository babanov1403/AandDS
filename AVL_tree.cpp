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

//every operation is O(logn) or O(1)(but with big constant);
class Binary_tree{
    void RotateRight(Node* u, Node* v);
    void RotateLeft(Node* u, Node* v);
    void updateBalance(Node* u);
    void reBalance(Node* u);
    Node* head;
    size_t sz;
public:
    Binary_tree() : head(new Node), sz(0){}
    ~Binary_tree(){
        //we need to delete all nodes
    }
    size_t size(){
        return this->sz;
    }
    void erase(int x){
        if(this->sz < 1){
            cout << "Nothing to erase\n";
            return;
        }
        (this->sz)--;
        Node* ptr = this->head;
        while(ptr && ptr->val != x){
            if(x > ptr->val){
                if(ptr->right) ptr = ptr->right;
                else{
                    cout << "Nothing to erase\n";
                    (this->sz)++;
                    return;
                }
            }
            else if(x < ptr->val){
                if(ptr->left) ptr = ptr->left;
                else{
                    cout << "Nothing to erase\n";
                    (this->sz)++;
                    return;
                }
            } 
        }
        if(!ptr){
            cout << "Nothing to erase\n";
            (this->sz)++;
            return;
        }
        //Now we need to check either node is leaf ,or has only one child, or has two children 
        //ZDES ZAKON4IL
        if(!ptr->left && !ptr->right)
            delete ptr;
        else if(!ptr->left ^ !ptr->right){

        }
        else if(ptr->left && ptr->right)
    }
    bool find(int x, Node* ptr = this->head){//NULL
        if(ptr){
            if(x == ptr->val) return true;
            if(x < ptr->val) return find(x, ptr->left);
            else return find(x, ptr->right);
        }
        else return false; 
    }

    void insert(int x){
        (this->sz)++;
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
        updateBalance(tmp);
    }
private:
    //combination of this two => big rotations
    void RotateRight(Node* u, Node* v){
        u->left = v->right;
        v->right = u;  
    }
    void RotateLeft(Node* u, Node* v){
        u->right = v->left;
        v->left = u;
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
        //i know how to do this in a pretty way, maybe later something will be changed
        //I HOPE THAT WORKS WELL
        if(u->diff == -2){//diff == -2 => right tree deeper than left tree => we will do left turn
            if(u->right){
                if(u->right->diff == -1){
                    RotateLeft(u, u->right);
                    u->diff = 0;
                    u->left->diff = 0;
                }
                else if(u->right->diff == 0){
                    RotateLeft(u, u->right);
                    u->diff = 1;
                    u->left->diff = -1;
                }
                else if(u->right->diff == 1){//далее большие повороты
                    if(u->right->left){
                        if(u->right->left->diff == 1){
                            RotateRight(u->right, u->right->left);
                            RotateLeft(u, u->right);
                            u->diff = 0;
                            u->left->diff = 0;
                            u->right->diff = -1; 
                        }
                        else if(u->right->left->diff == -1){
                            RotateRight(u->right, u->right->left);
                            RotateLeft(u, u->right);
                            u->diff = 0;
                            u->left->diff = 1;
                            u->right->diff = 0;
                        }
                        else if(u->right->left->diff == 0){
                            RotateRight(u->right, u->right->left);
                            RotateLeft(u, u->right);
                            u->diff = 0;
                            u->left->diff = 0;
                            u->right->diff = 0;
                        }
                    }
                }
            }
        }
        else if(u->diff == 2){
            if(u->left){
                if(u->left->diff == 1){//true
                    RotateRight(u, u->left);
                    u->diff = 0;
                    u->right->diff = 0;
                }
                else if(u->left->diff == 0){//true
                    RotateRight(u, u->left);
                    u->diff = -1;
                    u->right->diff = 1;
                }
                else if(u->left->diff == -1){
                    if(u->left->right){
                        if(u->left->right->diff == 1){
                            RotateLeft(u->left, u->left->right);
                            RotateRight(u, u->left);
                            u->diff = 0;
                            u->left->diff = 0;
                            u->right->diff = -1;
                        }
                        else if(u->left->right->diff == -1){
                            RotateLeft(u->left, u->left->right);
                            RotateRight(u, u->left);
                            u->diff = 0;
                            u->left->diff = 1;
                            u->right->diff = 0;
                        }
                        else if(u->left->right->diff == 0){
                            RotateLeft(u->left, u->left->right);
                            RotateRight(u, u->left);
                            u->diff = 0;
                            u->left->diff = 0;
                            u->right->diff = 0;
                        }
                    }   
                }
            }
        }
    } 

};

int main(){
    return 0;
}