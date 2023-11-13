#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

//rewrite with templates???
//maybe i should use shared ptr???
//maybe implement disk writing w/ fstream???

class BTree{
    struct Node{
        bool isLeaf;
        list<int> keys; // our elements in a node
        list<Node*> child; // size if keys.size()+1

        Node() : isLeaf(true) {}

        Node(int val) 
        : isLeaf(true){
            keys.push_back(val);
        }

        ~Node(){}
    };
    int t;
    Node* root;

    void insertInList(list<int>& v, int x){
        auto it = v.begin();
        while(it!=v.end() && *it < x) ++it;
        v.insert(it, x);
    }

    bool find(int x, Node* curr){
        if(curr){
            //search in a list
            auto it = curr->keys.begin();
            auto puller = curr->child.begin();
            for(;it!=curr->keys.end();++it, ++puller){
                if(*it == x) return true;
                if(*it > x){
                    return find(x, *puller);
                } 
            }
            return find(x, *puller);
           
        }
        return false;
    }

    void splitChild(Node* x, int i, list<Node*>::iterator it_c){
        Node* z = new Node;
        Node* y = *it_c;
        z->isLeaf = y->isLeaf;
        // z->keys.size() will be t-1
        for(int _ = 0; _<t; _++){
            z->keys.push_front(y->keys.back());
            y->keys.pop_back();
        }
        if(!y->isLeaf){
            for(int _ = 0; _<t; _++){
                z->child.push_front(y->child.back());
                y->child.pop_back();
            }
        }
        x->child.insert(it_c, z);

        insertInList(x->keys, y->keys.back());
        //write all on disk(y, z, x);


    }

    void insertNotFull(Node* x, int k){
        if(x->isLeaf){
            //inserting a k in a [keys]
            insertInList(x->keys, k);
            //write to disk
        }
        else{
            auto it = x->keys.begin();
            auto it_c = x->child.begin();
            while(it != x->keys.end() && *it < k){
                ++it;   
                ++it_c;
            }
            //reading from disk(accessing to *it_c)
            if((*it_c)->keys.size() == 2*t - 1){
                splitChild(x, t, it_c);

            }
            insertNotFull(*it_c, k);
        }
    }
public:
    BTree(): t(5), root(nullptr){}
    BTree(int t) : t(t) {
        root->child = {};
        root->keys = {};
        root->isLeaf = true;
    }
    ~BTree(){}

    //Find, O(h) obrascheniy k disku, and O(h*logt)
    bool find(int x){
        Node* curr = root;
        if(curr){
            //search in a list
            auto it = curr->keys.begin();
            auto puller = curr->child.begin();
            for(;it!=curr->keys.end();++it, ++puller){
                if(*it == x) return true;
                if(*it > x){
                    return find(x, *puller);
                } 
            }
            return find(x, *puller);
           
        }
        return false;
    }
    //if leaf we just need to insert
    void insert(int x){
        Node* r = root;
        if(!r->keys.empty() && r->keys.size() == 2*t - 1){//check if tree full or not
            Node* s = new Node;
            root = s;
            s->isLeaf = false;
            s->child.push_back(r);
            splitChild(s, t, r->child.begin());
            insertNotFull(s, x);
        }
        else insertNotFull(r, x);
    }
};


int main(){
    BTree b(2);
    b.insert(1);
    b.insert(2);
    b.insert(3);
    b.insert(4);
    b.insert(5);
}