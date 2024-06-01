#ifndef BASIC_STRING_H
#define BASIC_STRING_H


#include <memory>
#include <iterator>
#include <iostream>

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
	
	// Member functions
	basic_string() noexcept(noexcept(Allocator())) : basic_string(Allocator()) {}
	explicit basic_string( const Allocator& alloc ) : m_alloc(alloc) {}
	basic_string( size_type count, CharT ch, const Allocator& alloc = Allocator() )
	{
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, count);
		m_size = count;
		m_capacity = count;
		
		for (int i = 0; i < count; i++)
			std::allocator_traits<allocator_type>::construct(m_alloc, m_data + i, ch);
	}
	
	basic_string( const basic_string& other, size_type pos, const Allocator& alloc = Allocator() )
	{
		m_size = other.size() - pos;
		m_capacity = other.size() - pos;
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size);
		
		for (int i = 0; i < m_size; i++)
			std::allocator_traits<allocator_type>::construct(m_alloc, m_data + i, other[i]);
	}
	
	basic_string( const CharT* s, size_type count, const Allocator& alloc = Allocator() )
	{
		m_size = count;
		m_capacity = count;
		
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size);
		
		for (int i = 0; i < count; i++)
			std::allocator_traits<allocator_type>::construct(m_alloc, m_data + i, s[i]);
	}
	
	
	
	
	~basic_string(){}
	
	
	
	
	
	
	
	
	
	
	
	void print(const basic_string& string)
	{
		for (int i = 0; i < m_size; i++)
			std::cout << string.m_data[i];
	}
	
private:
	// Members of the class 
	allocator_type m_alloc;
	size_type m_size;
	size_type m_capacity;
	const CharT* m_data;
	
};



using string = basic_string<char>;

#endif //!BASIC_STRING_H
