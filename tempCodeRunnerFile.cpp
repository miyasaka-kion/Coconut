#include <iostream>


struct  A{
    int x;
    A(int x = 1) : x(x) {}  
};

struct D : A  {
    D(int y) : A(y) {}
};

struct Entity {
    int x;
    // Entity() = default;
};


auto main() -> int {
    Entity e;
    std::cout << e.x << std::endl;
}