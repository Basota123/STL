#include <iostream>
#include "list.hpp"

template< class Os, class Container >
Os& operator << (Os& os, Container container)
{
    os << "{";
    for (auto it = container.begin(); it!= container.end(); ++it)
    {
        os << " " << *it;
    }
    os << " }";
    return os;
}


int main()
{

    list<int> l1 = {1,2,3,4,5};
    std::cout << l1;
    

    return 0;
}