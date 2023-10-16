#include <vector>
#include <iostream>
using namespace std;
class Heap {
public:
	std::vector<int> getElement;//)) new int[100000]
	std::size_t size = getElement.size();

	//default constructor
	Heap() {}
	//via vector
	Heap(std::vector<int> v) : getElement(v) {
		heapify(getElement);
	}
	// write all numbers
	Heap(std::initializer_list<int> lst) : getElement{ lst } {
		heapify(getElement);
	}
	//destructor
	~Heap() {
		this->getElement.clear();
	}
	//insert x, getmin, extract min, decrease some key by delta

	void insert(int x) {
		this->getElement.push_back(x);
		siftup(this->getElement, (this->getElement).size() - 1);
	}
	void extractMin() {
		std::vector<int>& v = this->getElement;
		std::swap(v[0], v[v.size() - 1]);
		v.pop_back();
		siftdown(v, 0);
	}
	int getMin() {
		return this->getElement[0];
	}
	void decrease(int i, int delta) {
		this->getElement[i] -= delta;
		siftup(this->getElement, i);
	}
	/*void show() {
		int k = 0;
		for (auto i : this->getElement)
			cout << k++ << ": " << i << '\n';
		cout << '\n';
	}*/
	int operator[](int i) {
		int n = this->getElement.size();
		//std::cerr << "index out of range\n";
		return this->getElement[(i % n + n) % n];
		
	}


private:
	//heapify, siftup, siftdown, ???
	void siftup(std::vector<int>& v, int i) {
		//just let lighter vertice go up( O(logn) )
		while (i > 0 && v[i] < v[(i - 1) / 2]) {
			std::swap(v[i], v[(i - 1) / 2]);
			i = (i - 1) / 2;
		}
	}
	void siftdown(std::vector<int>& v, int i) {
		//heavy down(children 2i+1, 2i+2) ( O(logn)
		int j = -1;//s kem swap
		while (2 * i + 1 < v.size()) {
			if (v[i] > v[2 * i + 1]) j = 2 * i + 1;
			//std::cout << j << " ";
			if (2 * i + 2 < v.size() && v[i] > v[2 * i + 2]) {
				if (j != -1 && v[j] > v[2 * i + 2]) j = 2 * i + 2;
				else if (j == -1 && v[i] > v[2 * i + 2]) j = 2 * i + 2;
			}
			if (j == -1) break;
			std::swap(v[i], v[j]);
			i = j;
			j = -1;
		}
	}
	void heapify(std::vector<int>& v) {
		//O(n)
		for (int i = v.size() - 1; i >= 0; i--) {
			siftup(v, i);
		}
	}
};

int main() {
	vector<int> v = { 7, 6, 5, 4, 3, 2, 1 };
}

