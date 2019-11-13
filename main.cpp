#include <iostream>

#include "sub1.hpp"

int main(int argc, char const* argv[])
{
    std::cout << "make test" << std::endl;
    sub1();
#ifdef NDEBUG
    std::cout << "NO DEBUG" << std::endl;
#else
    std::cout << "DEBUG" << std::endl;
#endif
    return 0;
}
