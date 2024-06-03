#include <iostream>

template< class... Args >
void print(Args&&... args) { }

template< class Head, class... Args >
void print(const Head head, Args&&... args)
{
	std::cout << head << std::endl;
	print(args...);
}
