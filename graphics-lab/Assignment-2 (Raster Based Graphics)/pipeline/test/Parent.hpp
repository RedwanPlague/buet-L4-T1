#ifndef A
#define A

#include "Child.hpp"

class Parent {
    int a;

  public:
    Parent() = default;
    Parent(int a) : a(a) {}

    int getA() { return a; }

    void use(Child b) {}
};

#endif // A
