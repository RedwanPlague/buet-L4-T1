#include "Child.hpp"
#include "Parent.hpp"
#include <iostream>

using namespace std;

int main() {
    Parent a(5);
    cout << a.getA() << endl;

    Child b(3);
    cout << b.getA() << endl;
    cout << b.getB() << endl;

    return 0;
}
