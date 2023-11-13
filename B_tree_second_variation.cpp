#include <vector>
#include <iostream>

using namespace std;




/*
*every time when we do smth like -> or * 
* we dowloading data from server(that's costs a lot for us)
*/
template<int T>
class btree{
public:
    struct Node{
        vector<int> keys;
        vector<Node*> children;

        Node(int key){
            keys.push_back(key);
            children.push_back(nullptr);
            children.push_back(nullptr);
        }

        Node(vector<int> keys, vector<int> children){
            
        }
    };
    Node* root;
    //each time we have only one node, and recursive approach doesnt work at all(because it is a B-tree)
    //we want to store only one item each time
    bool find(int key){
        Node* curr = root;
        while(curr){
            //make here somthing like data-getter
            //but for now it will cost O(1) by *ptr
            auto num = lower_bound(curr->keys.begin(), curr->keys.end(), key);
            int i = curr->keys.begin() - num;
            if(i<curr->keys.size() && curr->keys[i] == key) return true;
            else curr = (curr->keys[i] < key) ? curr->children[i] : curr->children[i+1]; // i think there's a bug
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
                v->keys.insert(v.begin() + pos, key);
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
            delete posIt;
            // and upload to database left/right child
            //then insert out "mean" node in v
            v->keys.insert(v->keys.begin() + pos, posIt->keys[T-1]);
            //now we need to deal with children
            v->children[pos] = leftChild;
            v->children.insert(v->children.begin()+pos+1, rightChild);
            //save the result to database
            v = (key < posIt->keys[T-1]) ? leftChild : rightChild;
        }
    }

    void erase(int key){
        /*
        * 0) we need to check whether our key in a root or not
        * 1) if our key in a leaf
        *
        * 
        */
        Node* curr = root;
        int pos;
        while(curr){
            if(curr != root && curr->keys.size() == T-1) //now we need make here
            pos = lower_bound(curr->keys.begin(), curr->keys.end(), key) - curr->keys.begin();
            if(i<curr->keys.size() && curr->keys[pos] == key) break;
            else curr = (curr->keys[pos] < key) ? curr->children[pos] : curr->children[pos+1]; // i think there's a bug
        }
        if(!curr){
            cout << "nothing to erase\n";
            return;
        }
        Node* currIt = curr->children[pos];
        if(!currIt){//(1)
            if(curr->keys.size() >= T){
                //just erase
                curr->keys.erase(v.begin() + pos);
                return;
            }


        }
    }
};



int main(){
    return 0;
}