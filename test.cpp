#include <iostream>
using namespace std;

class A{
public:
    int x;
    A(){
        cout << "A created\n";
    }
    A(int x) : x(x){
        cout << "A created with " << x <<'\n';
    }

    ~A(){
        cout << "~A\n";
    }
};

int main(){
    A* a = new A(5);
    delete a;
}