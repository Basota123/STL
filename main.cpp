#include <iomanip>
#include <iostream>
#include <string>
#include "containers/vector.hpp"


 
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
    // C++11 initializer list syntax:
    vector<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
    std::cout << "1: " << words1;
 
    // words2 == words1
    vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "2: " << words2;
 
    // words3 == words1
    vector<std::string> words3(words1);
    std::cout << "3: " << words3;
 
    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    vector<std::string> words4(5, "Mo");
    std::cout << "4: " << words4;
 
    auto const rg = {"cat", "cow", "crow"};
#ifdef __cpp_lib_containers_ranges
    std::vector<std::string> words5(std::from_range, rg); // overload (11)
#else
    vector<std::string> words5(rg.begin(), rg.end()); // overload (5)
#endif
    std::cout << "5: " << words5;

	return 0;
}







//int main()
//{
//    std::fstream file("results.txt");
//
//    file << "std::set versus own_set\n";
//    file << "insertion time \n";
//
//    for (int i = 0; i < 10; i++)
//    {    
//        auto std_set_statr = std::chrono::high_resolution_clock::now();
//
//        std::set<int> std_set;
//        for (int i = 0; i < 1000000; i++)
//        {
//            int random_number = std::experimental::randint(0, 1000000);
//            std_set.insert(random_number);
//        }
//
//        auto std_set_stop = std::chrono::high_resolution_clock::now();
//
//        std::chrono::duration<double> dur1 = std_set_stop - std_set_statr;
//        file << "std_set: " << dur1.count() << "\n";
//
//        auto own_set_statr = std::chrono::high_resolution_clock::now();
//
//        set<int> own_set;
//        for (int i = 0; i < 1000000; i++)
//        {
//            int random_number = std::experimental::randint(0, 1000000);
//            own_set.insert(random_number);
//        }
//
//        auto own_set_stop = std::chrono::high_resolution_clock::now();
//
//        std::chrono::duration<double> dur2 = own_set_stop - own_set_statr;
//        file << "own_set: " << dur2.count() << "\n";
//
//        file << " \n";
//
//    }
//
//
// 
//    return 0;
//}
