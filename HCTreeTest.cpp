#include <iostream>
#include "HCTree.hpp"
#include <vector>
using namespace std;

int main( int argc, char* argv[]) {
    vector<int> v {3, 6, 9, 12};
    HCTree* t = new HCTree();
    t->build(v);
    ofstream s;
    BitOutputStream* out = new BitOutputStream(s);
    t->encode(6, out);
}
