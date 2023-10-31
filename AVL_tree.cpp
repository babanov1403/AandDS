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

void print(Node* curr);

//every operation is O(logn) or O(1)(but with big constant);
class Binary_tree{
    void doSomeShit(Node* u, Node* v){
        v->p = u->p;
        if(u->p){
            if(u->p->left == u) u->p->left = v;
            else if(u->p->right == u) u->p->right = v;
        }
        u->p = v;
    }
    void correctHead(){
        Node* tmp = head;
        while(tmp->p)
            tmp = tmp->p;
        head = tmp;
    }
    //combination of this two => big rotations
    void RotateRight(Node* u, Node* v){
        u->left = v->right;
        v->right = u;  
        doSomeShit(u, v);
    }
    void RotateLeft(Node* u, Node* v){
        u->right = v->left;
        v->left = u;
        doSomeShit(u, v);
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
        else return;
    }
    void reBalance(Node* u){
        //i know how to do this in a pretty way, maybe later something will be changed
        //I HOPE THAT WORKS WELL
        if(u->diff == -2){//diff == -2 => right tree deeper than left tree => we will do left turn
            if(u->right){
                if(u->right->diff == -1){
                    Node* v = u->right;
                    RotateLeft(u, v);
                    v->diff = 0;
                    v->left->diff = 0;
                }
                else if(u->right->diff == 0){    
                    Node* v = u->right;
                    RotateLeft(u, v);  
                    v->diff = 1;
                    v->left->diff = -1;
                }
                else if(u->right->diff == 1){//далее большие повороты
                    if(u->right->left){
                        Node* a = u, *b = u->right, *c = u->right->left;
                        if(u->right->left->diff == 1){
                            RotateRight(b, c);
                            RotateLeft(a, c);
                            c->diff = 0;
                            a->diff = 0;
                            b->diff = -1; 
                        }
                        else if(u->right->left->diff == -1){
                            RotateRight(b, c);
                            RotateLeft(a, c);
                            c->diff = 0;
                            a->diff = 1;
                            b->diff = 0;
                        }
                        else if(u->right->left->diff == 0){
                            RotateRight(b, c);
                            RotateLeft(a, c);
                            c->diff = 0;
                            a->diff = 0;
                            b->diff = 0;
                        }
                    }
                }
            }
        }
        else if(u->diff == 2){
            if(u->left){
                if(u->left->diff == 1){
                    Node* v = u->left;     
                    RotateRight(u, v);
                    v->diff = 0;
                    v->right->diff = 0;
                }
                else if(u->left->diff == 0){
                    Node* v = u->left;     
                    RotateRight(u, v);
                    v->diff = -1;
                    v->right->diff = 1;
                }
                else if(u->left->diff == -1){
                    if(u->left->right){
                        Node* a = u, *b = u->left, *c = u->left->right; 
                        if(u->left->right->diff == 1){
                            RotateLeft(b, c);
                            RotateRight(a, c);
                            c->diff = 0;
                            b->diff = 0;
                            a->diff = -1;
                        }
                        else if(u->left->right->diff == -1){
                            RotateLeft(b, c);
                            RotateRight(a, c);
                            c->diff = 0;
                            b->diff = 1;
                            a->diff = 0;
                        }
                        else if(u->left->right->diff == 0){
                            RotateLeft(b, c);
                            RotateRight(a, c);
                            c->diff = 0;
                            b->diff = 0;
                            a->diff = 0;
                        }
                    }   
                }
            }
        }
        //now we need to check is our head true root of the tree
        correctHead();
    }
    size_t sz;
    void destrHelper(Node* u){
        if(!u) return;
        if(!u->left && !u->right){
            delete u;
            return;
        }
        destrHelper(u->left);
        destrHelper(u->right);
        delete u;
        return;
    }
public:
    Node* head;
    Binary_tree() : head(nullptr){}
    ~Binary_tree(){
        destrHelper(head);
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
        if(sz == 0){
            delete head;
            return;
        }
        Node* parent = ptr->p;
        int what_child  = 0;
        if(ptr == parent->left) what_child = -1;
        else if(ptr == parent->right) what_child = 1;
        
        if(!ptr->left && !ptr->right){
            parent->diff += what_child;
            delete ptr;
        }
        else if(!ptr->left ^ !ptr->right){
            if(ptr->left){
                swap(ptr, ptr->left);
            } 
            else{
                swap(ptr, ptr->right);
            }
            parent->diff += what_child;
            delete ptr;
        }
        else if(ptr->left && ptr->right){
            Node* helper = ptr->right;
            
            while(helper->left) helper = helper->left;
            swap(ptr, helper);
            parent = ptr->p;
            if(ptr == parent->left) (parent->diff)--;
            else if(ptr == parent->right) (parent->diff)++;
            delete ptr; 
        }
        reBalance(parent);
    }
    bool find(int x){
        Node* ptr = head;
        if(ptr){
            if(x == ptr->val) return true;
            if(x < ptr->val) return find(x, ptr->left);
            else return find(x, ptr->right);
        }
        else return false; 
    }
    bool find(int x, Node* ptr){//NULL
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
        //rude code
        if(!curr){
            head = tmp;
            head->diff = 0;
            head->val = x;
        }else{
            while(curr){
                if(curr->val < x){
                    if(curr->right) curr = curr->right;
                    else{
                        curr->right = tmp;
                        tmp->p = curr;
                        break;
                    }
                }
                else{
                    if(curr->left) curr = curr->left;
                    else{
                        curr->left = tmp;
                        tmp->p = curr;
                        break;
                    }
                }
            }
        }
        updateBalance(tmp);
    }
};

void print(Node* curr){
        if(!curr) return;
        cout << '(' << curr->val << " , " << curr->diff << ')';
        cout << '\n';
        if(curr->left) cout << '(' << curr->left->val << " , " << curr->left->diff << ") ";
        else cout << "null ";
        if(curr->right) cout << '(' << curr->right->val << " , " << curr->right->diff << ") ";
        else cout << "null ";
        cout << '\n';
        print(curr->left);
        print(curr->right);
}

int main(){
    Binary_tree tree;
    int a;
    int k = 0;
    while(k++<10){
        cin >> a;
        tree.insert(a);
        print(tree.head);
        cout << "====================\n";
    }
    while(k-->5){
        cin >> a;
        tree.erase(a);
        print(tree.head);
        cout << "=================\n";
    }
    return 0;
}