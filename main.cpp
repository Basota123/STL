#include <iostream>
#include "containers/vector.hpp"
#include "containers/array.hpp"
 
template<typename T>
std::ostream& operator<<(std::ostream& s, const vector<T>& v)
{
    s.put('{');
    for (char comma[]{'\0', ' ', '\0'}; const auto& e : v)
        s << comma << e, comma[0] = ',';
    return s << "}\n";
}
 
int main()
{

	return 0;
}
