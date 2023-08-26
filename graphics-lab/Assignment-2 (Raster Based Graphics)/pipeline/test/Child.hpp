#ifndef B
#define B

#include "Parent.hpp"

class Child : public Parent {
    int b;

  public:
    Child() = default;
    Child(int b) : b(b) {}

    int getB() { return b; }
};

#endif // B
