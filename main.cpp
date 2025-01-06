#include <iostream>
#include "containers/vector.hpp"

 
template<typename T>
std::ostream& operator<<(std::ostream& s, const vector<T>& v)
{
    s.put('{');
    for (char comma[]{'\0', ' ', '\0'}; const auto& e : v)
        s << comma << e, comma[0] = ',';
    return s << "}";
}
 
int main()
{
    vector<int> v(10, 1);
    v.reserve(20);
    std::cout << v.capacity() << std::endl;
    v.shrink_to_fit();
    std::cout << v.capacity() << std::endl;

    std::cout << v << std::endl;
    
    
    


	return 0;
}
