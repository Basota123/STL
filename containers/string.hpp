#ifndef BASIC_STRING_H
#define BASIC_STRING_H


#include <memory>
#include <iterator>


template <
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
> class basic_string 
{
private:
	class bs_iter;
	
public:
	// Member types
	using traits_type = Traits;
	using value_type = CharT;
	using allocator_type = Allocator;
	using size_type = std::allocator_traits<Allocator>::size_type;
	using difference_type = std::allocator_traits<Allocator>::difference_type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = std::allocator_traits<Allocator>::pointer;
	using const_pointer = std::allocator_traits<Allocator>::const_pointer;
	using iterator = bs_iter;
	using const_iterator = const bs_iter;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	
	
	
};

using string = basic_string<char>;

#endif //!BASIC_STRING_H
