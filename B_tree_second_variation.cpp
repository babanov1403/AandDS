#include <vector>
#include <iostream>

using namespace std;




/*
*every time when we do smth like -> or * 
* we dowloading data from server(that's costs a lot for us)
*/
struct Node{
        vector<int> keys;
        vector<Node*> children;

        Node(int key){
            keys.push_back(key);
            children.push_back(nullptr);
            children.push_back(nullptr);
        }

        Node(vector<int> keys, vector<Node*> children) : keys(keys), children(children) {}
};
template<int T>
class btree{
public:
    

    Node* root;

    btree() : root(nullptr) {}
    //each time we have only one node, and recursive approach doesnt work at all(because it is a B-tree)
    //we want to store only one item each time
    bool find(int key){
        Node* curr = root;
        while(curr){
            //make here somthing like data-getter
            //but for now it will cost O(1) by *ptr
            //[6, 9]
            int i = lower_bound(curr->keys.begin(), curr->keys.end(), key) - curr->keys.begin();
            if(i<curr->keys.size() && curr->keys[i] == key) return true;
            else curr = (key < curr->keys[i]) ? curr->children[i] : curr->children[i+1]; // i think there's a bug
        }
        return false;
    }

    //return value maybe Node*?? or void
    void insert(int key){
        if(!root){
            root = new Node(key);
            return;
            //return loadNewNode(root);
        }
        Node* v = root; 
        if(root->keys.size() == 2*T - 1){
            root = nullptr;//delete current root(we will deal with it a bit lately)
            //T-1 key wont be in *Child
            Node* leftChild = new Node(vector<int>(v->keys.begin(), v->keys.begin() + T - 1), vector<Node*>(v->children.begin(), v->children.begin() + T));
            Node* rightChild = new Node(vector<int>(v->keys.begin()+ T, v->keys.end()), vector<Node*>(v->children.begin(), v->children.begin() + T));
            //and we need to upload this to our server/database
            root = new Node({v->keys[T-1]}, {leftChild, rightChild});
            
            //our key will be inserted in leftChild or rightChild depends of root
            delete v;
            v = (key < root->keys[0]) ? leftChild : rightChild;
        }
        while(true){
            int pos = lower_bound(v->keys.begin(), v->keys.end(), key) - v->keys.begin();
            Node* posIt = v->children[pos];
            if(!posIt){//or make bool isLeaf in node and check it
                v->keys.insert(v->keys.begin() + pos, key);
                v->children.push_back(nullptr);
                //then upload the node to our server
                return;
            } 
            if(posIt->keys.size() < 2*T - 1){
                v = posIt;
                continue;
            }
            //if we are here, then we need to split our Node :(
            Node* leftChild = new Node(vector<int>(posIt->keys.begin(), posIt->keys.begin() + T - 1), vector<Node*>(posIt->children.begin(), posIt->children.begin() + T));
            Node* rightChild = new Node(vector<int>(posIt->keys.begin()+ T, posIt->keys.end()), vector<Node*>(posIt->children.begin(), posIt->children.begin() + T));
            // now delete posIt
            
            // and upload to database left/right child
            //then insert out "mean" node in v
            v->keys.insert(v->keys.begin() + pos, posIt->keys[T-1]);
            //now we need to deal with children
            v->children[pos] = leftChild;
            v->children.insert(v->children.begin()+pos+1, rightChild);
            //save the result to database
            v = (key < posIt->keys[T-1]) ? leftChild : rightChild;
            delete posIt;
        }
    }

    void erase(int key){
        /*
        actually we must consider case when user wants to erase the root(or root's key) but idk maybe 
        one day i will fix it
        */
        Node* prev = root;
        int i = lower_bound(prev->keys.begin(), prev->keys.end(), key) - prev->keys.begin();
        if(i<prev->keys.size() && prev->keys[i] == key){
            eraseIfFound(prev, key);
            return;
        }
        Node* next  = (key < prev->keys[i]) ? prev->children[i] : prev->children[i+1];
        while(next){
            i = lower_bound(next->keys.begin(), next->keys.end(), key) - next->keys.begin();
            if(next->keys.size() >= T){
                if(i<next->keys.size() && next->keys[i] == key){
                    eraseIfFound(next, key);
                    return;
                }
                else{ 
                    prev = next;
                    next = (key < prev->keys[i]) ? prev->children[i] : prev->children[i+1]; 

                }// i think there's a bug
            }

            else{
                /*
                * if we are here, we facing a problem : we need to downgrade to 
                * node (next), but unfortunately it has T-1 keys => we need to add 
                * one key to our next node, we can do this in 3 ways:
                * 1) borrow if exists from the left neighbour(>=T)
                * 2) borrow if exists from the right (>=T)
                * 3) if we have all two neighbours T-1 then we need to merge them and do it
                * recursively XDDDD
                */
               // [][]        [prev]   [][]
               //[][][nextLeft]    [next][nextRight][]

               // []        []    [prev]         [][]
               //[][nextLeft][next]    [nextRight][][]
               Node* nextLeft, *nextRight;
               int pos = (prev->keys[i] < key) ? i+1 : i;
               nextLeft = (pos - 1 >= 0) ? prev->children[pos-1] : nullptr;
               nextRight = (pos + 1 < prev->children.size()) ? prev->children[pos + 1] : nullptr;
                

               if(nextLeft){
                    if(nextLeft->keys.size() >= T){
                        //lets take key from nextLeft
                        next->keys.insert(next->keys.begin(), nextLeft->keys.back());
                        nextLeft->keys.pop_back();
                        next->children.insert(next->children.begin(), nextLeft->children.back());
                        nextLeft->children.pop_back();
                        swap(next->keys.front(), prev->keys.back());
                    }
                    else{
                        next = mergeTwoChild(prev, nextLeft, next, i);

                    }
               }
               else if(nextRight){
                    if(nextRight->keys.size() >= T){
                        next->keys.push_back(nextRight->keys.front());
                        nextRight->keys.erase(nextRight->keys.begin());
                        next->children.push_back(next->children.front());
                        nextRight->children.erase(nextRight->children.begin());
                        swap(next->keys.back(), prev->keys.front());
                    }
                    else{
                        next = mergeTwoChild(prev, next, nextRight, i);
                    }
               }
            }
        }
        // make here smth for a leaf variant XDDD
    }

    ~btree(){
        //there is must be memory deletion :)))
    }

private:
    //do the best stuff in the world
    Node* mergeTwoChild(Node* curr, Node* left, Node* right, int pos){
        Node* merged = new Node(vector<int>(left->keys.begin(), left->keys.end()), vector<Node*>(left->children.begin(), left->children.end()));
        merged->keys.push_back(curr->keys[pos]);
        for(int i = 0; i<T-1; i++){
            merged->keys.push_back(right->keys[i]);
            merged->children.push_back(right->children[i]);
        }
        cout << pos << "<-----\n";
        merged->children.push_back(right->children.back());
        curr->keys.erase(curr->keys.begin()+pos);
        curr->children.erase(curr->children.begin()+pos); //??
        curr->children[pos] = merged;
        delete left;
        delete right;
        return merged;
    }


    void eraseIfFound(Node* curr, int key){
        int pos = lower_bound(curr->keys.begin(), curr->keys.end(), key) - curr->keys.begin();
        Node* posLeftChild = curr->children[pos];
        Node* posRightChild = curr->children[pos+1];
        if(!posLeftChild && !posRightChild){
            //we operate with a leaf now(which has more than T keys)
            curr->keys.erase(curr->keys.begin()+pos);
            curr->children.erase(curr->children.begin()+pos);
            return;
        }
        if(posRightChild->keys.size() == T-1 && posLeftChild->keys.size() == T-1){
            //merging
            Node* merged = mergeTwoChild(curr, posLeftChild, posRightChild, pos);
            eraseIfFound(merged, key); // bad practice
        }

        //consider right child

        if(posRightChild->keys.size() >= T){
            /*
            1) we're traversing down to the left-est child of this, but always checking if keys.size() >=T
            2) if not, we need to "make a path" for us and borrow a key from neighbour node
            */
           Node* prev = posRightChild;
           Node* nextLeft = posRightChild->children.front();
           Node* nextRight = posRightChild->children[1];
           while(nextLeft){
                if(nextLeft->keys.size() >= T){
                    prev = nextLeft;
                    nextRight = prev->children[1];
                    nextLeft = nextLeft->children.front(); 
                }
                else if(nextRight->keys.size() >= T){
                    //borrow from right child
                    nextLeft->keys.push_back(nextRight->keys.front());
                    nextRight->keys.erase(nextRight->keys.begin());
                    nextLeft->children.push_back(nextRight->children.front());
                    nextLeft->children.erase(nextLeft->children.begin());
                    swap(nextLeft->keys.back(), prev->keys.front());

                    //and do some other stuff
                    prev = nextLeft;
                    nextRight = prev->children[1];
                    nextLeft = prev->children[0];
                }
                else{
                    Node* prev = mergeTwoChild(prev, nextLeft, nextRight, 0);
                    nextRight = prev->children[1];
                    nextLeft = prev->children[0];
                }
           }
        }

        if(posLeftChild->keys.size() >= T){
             /*
            1) we're traversing down to the left-est child of this, but always checking if keys.size() >=T
            2) if not, we need to "make a path" for us and borrow a key from neighbour node
            */
           Node* prev = posLeftChild;
           Node* nextLeft = posLeftChild->children[posLeftChild->children.size()-2];
           Node* nextRight = posLeftChild->children.back();
           while(nextRight){
                if(nextRight->keys.size() >= T){
                    prev = nextRight;
                    nextLeft = prev->children[posLeftChild->children.size()-2];
                    nextRight = prev->children.back();
                }
                else if(nextLeft->keys.size() >= T){
                    //borrow from left child
                    nextRight->keys.insert(nextRight->keys.begin(), nextLeft->keys.back());
                    nextLeft->keys.pop_back();
                    nextRight->children.insert(nextRight->children.begin(), nextLeft->children.back());
                    nextLeft->children.pop_back();
                    swap(nextRight->keys.front(), prev->keys.back());

                    //and do some other stuff 
                    prev = nextRight;
                    nextRight = prev->children.back();
                    nextLeft = prev->children[prev->children.size() - 2];
                }
                else{
                    Node* prev = mergeTwoChild(prev, nextLeft, nextRight, T-2);
                    nextRight = prev->children.back();
                    nextLeft = prev->children[prev->children.size() - 2];
                }
           }
        }    
    }
};



int main(){
    btree<3> tree;
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(8);
    tree.insert(9);
    tree.insert(10);
    tree.insert(11);
    tree.insert(12);
    tree.insert(13);
    tree.insert(14);
    if(tree.find(7)) cout << "Found 7!\n";
    else cout << "Not found 7!\n";
    if(tree.find(100)) cout << "Found 101!\n";
    else cout << "Not found 101!\n";
    tree.erase(7);
    if(tree.find(7)) cout << "Found 7!\n";
    else cout << "Not found 7!\n";
    tree.insert(20);
    tree.insert(19);
    tree.insert(18);
    tree.insert(17);
    tree.erase(12);
    return 0;
}