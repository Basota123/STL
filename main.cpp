#include <iostream>
#include "containers/set.hpp"
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <set>
#include <experimental/random>

int main()
{
    std::fstream file("results.txt");

    file << "std::set versus own_set\n";
    file << "insertion time \n";

    for (int i = 0; i < 10; i++)
    {    
        auto std_set_statr = std::chrono::high_resolution_clock::now();

        std::set<int> std_set;
        for (int i = 0; i < 1000000; i++)
        {
            int random_number = std::experimental::randint(0, 1000000);
            std_set.insert(random_number);
        }

        auto std_set_stop = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> dur1 = std_set_stop - std_set_statr;
        file << "std_set: " << dur1.count() << "\n";

        auto own_set_statr = std::chrono::high_resolution_clock::now();

        set<int> own_set;
        for (int i = 0; i < 1000000; i++)
        {
            int random_number = std::experimental::randint(0, 1000000);
            own_set.insert(random_number);
        }

        auto own_set_stop = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> dur2 = own_set_stop - own_set_statr;
        file << "own_set: " << dur2.count() << "\n";

        file << " \n";

    }


 
    return 0;
}