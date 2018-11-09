
#include <iostream>
#include "CommSharedMemory.hpp"


void Box::print()
{
    std::cout << "length    = " << length << std::endl;
    std::cout << "breadth   = " << breadth << std::endl;
    std::cout << "height    = " << height << std::endl;
}