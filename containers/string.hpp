#ifndef BASIC_STRING_H
#define BASIC_STRING_H


#include <memory>
#include <iterator>
#include <iostream>
#include <algorithm>

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
	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	
	// Member functions
	basic_string() noexcept(noexcept(Allocator())) : basic_string(Allocator()) {}
	explicit basic_string( const Allocator& alloc ) : m_alloc(alloc) {}
	basic_string( size_type count, CharT ch, const Allocator& alloc = Allocator() )
	{
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size * sizeof(CharT));
		m_size = count;
		m_capacity = count;
		
		for (int i = 0; i < count; i++)
			std::allocator_traits<allocator_type>::construct(m_alloc, m_data + i, ch);
	}
	
	basic_string( const basic_string& other, size_type pos, const Allocator& alloc = Allocator() )
	{
		m_size = other.size() - pos;
		m_capacity = other.size() - pos;
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size * sizeof(CharT));
		
		for (int i = 0; i < m_size; i++)
			std::allocator_traits<allocator_type>::construct(m_alloc, m_data + i, other[i]);
	}
	
	basic_string( const CharT* s, size_type count, const Allocator& alloc = Allocator() )
	{
		m_size = count;
		m_capacity = count;
		
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size * sizeof(CharT));
		
		for (int i = 0; i < count; i++)
			std::allocator_traits<allocator_type>::construct(m_alloc, m_data + i, s[i]);
	}
	
	basic_string( const CharT* s, const Allocator& alloc = Allocator() )
	{
		m_data = s;
		m_size = std::char_traits<CharT>::length(s);
		m_capacity = std::char_traits<CharT>::length(s);
	}
	
	template< class InputIt >
	basic_string( InputIt first, InputIt last, const Allocator& alloc = Allocator() )
	{
		m_size = std::distance(first, last);
		m_capacity = std::distance(first, last);
		
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size * sizeof(CharT));
		
		int counter = 0;
		for (; first != last; ++first)
		{
			std::allocator_traits<allocator_type>::construct(m_alloc, m_data + counter, *first);
			++counter;
		}
			
	}
	
	basic_string( const basic_string& other )
	{
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, other.m_size * sizeof(CharT));
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		
		int counter = 0;
		for (auto it = other.begin(); it != other.end(); ++it)
		{
			m_data = std::allocator_traits<allocator_type>::construct(m_alloc, m_data + counter, *it);
			++counter;
		}
		
	}
	
	basic_string( const basic_string& other, const Allocator& alloc ) : m_alloc(alloc)
	{
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, other.m_size * sizeof(CharT));
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		
		int counter = 0;
		for (auto it = other.begin(); it != other.end(); ++it)
		{
			m_data = std::allocator_traits<allocator_type>::construct(m_alloc, m_data + counter, *it);
			++counter;
		}
	}
	
	basic_string( basic_string&& other ) noexcept
	{
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		m_data = other.m_data;
		
		other.clear();
		
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
	}
	
	basic_string( basic_string&& other, const Allocator& alloc )
	{
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		m_data = other.m_data;
		m_alloc = std::move(other.m_alloc);
		
		other.clear();
		
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
	}
	
	basic_string( std::initializer_list<CharT> ilist, const Allocator& alloc = Allocator() ) : m_alloc(alloc)
	{
		m_size = ilist.size();
		m_capacity = ilist.size();
		m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size * sizeof(CharT));
		
		int counter = 0;
		for (auto it = ilist.begin(); it != ilist.end(); ++it)
		{
			std::allocator_traits<allocator_type>::construct(m_alloc, m_data+counter, *it);
			++counter;
		}
	}
	
	~basic_string()
	{
		clear();
		std::allocator_traits<allocator_type>::deallocate(m_alloc, const_cast<CharT*>(m_data), m_size * sizeof(CharT));
	}
	
	basic_string& operator=( const basic_string& str )
	{
		if (this == &str) return *this;
		
		if (capacity() > str.size()) 
			std::allocator_traits<allocator_type>::deallocate(m_alloc, m_data, capacity());
		if (str.size() > capacity())
			m_data = std::allocator_traits<allocator_type>::allocate(m_alloc, str.size() * sizeof(CharT));
		
		std::copy(str.begin(), str.end(), m_data);
		
		m_size = str.m_size;
		m_capacity = str.m_capacity;
		return *this;
	}
	
	basic_string& operator=( basic_string&& str ) noexcept 
	{
		if(this == &str) return *this;
		
		clear();
		
		m_data = str.m_data;
		m_size = str.m_size;
		m_capacity = str.m_capacity;
		m_alloc = std::move(str.m_alloc);
		
		str.m_data = nullptr;
		str.m_size = 0;
		str.m_capacity = 0;
	}
	
	basic_string& operator=( const CharT* s )
	{
		if (this == &s) return *this;
		
		m_data = s;
		m_size = std::char_traits<CharT>::length(s);
		m_capacity = std::char_traits<CharT>::length(s);
		
		return *this;
	}
	
	allocator_type get_allocator() const { return m_alloc; }
	
	// Element access
	CharT& at( size_type pos ) { return (const_cast<char*>(m_data)[pos]); }
	const CharT& at( size_type pos ) const { return m_data[pos]; }
	
	CharT& operator[]( size_type pos ) { return (const_cast<char*>(m_data)[pos]); }
	const CharT& operator[]( size_type pos ) const { return m_data[pos]; } 
	
	CharT& front() { return this->operator[](0); }
	const CharT& front() const { return this->operator[](0); }
	
	CharT& back() { return this->operator[](size() - 1); }
	const CharT& back() const { return this->operator[](size() - 1); }
	
	const CharT* data() const { return m_data; }
	CharT* data() noexcept { return const_cast<char*>(m_data); }
	
	// Iterators
	iterator begin() { return m_data; }
	const_iterator begin() const { return m_data; }
	const_iterator cbegin() const noexcept { return m_data; }
	
	iterator end() { return m_data + m_size; }
	const_iterator end() const { return m_data + m_size; }
	const_iterator cend() const noexcept { return m_data + m_size; }
	
	reverse_iterator rbegin() noexcept { return reverse_iterator( end() ); }
	const_reverse_iterator rbegin() const noexcept { return reverse_iterator( end() ); }
	const_reverse_iterator crbegin() const noexcept { return reverse_iterator( end() ); }
	
	reverse_iterator rend() noexcept {return reverse_iterator( begin() ); }
	const_reverse_iterator rend() const noexcept {return reverse_iterator( begin() ); }
	const_reverse_iterator crend() const noexcept {return reverse_iterator( begin() ); }
	
	// Capacity
	bool empty() const { return m_size == 0; }
	size_type size() const { return m_size; }
	size_type length() const { return m_size; }
	size_type max_size() const { return std::allocator_traits<allocator_type>::max_size(m_alloc); }
	
	void reserve(size_type new_cap)
	{
		if (new_cap > max_size())
		{
			throw std::length_error("Capacity overflow");
		}
		
		if (new_cap <= m_capacity)
		{
			return;
		}
		
		CharT* new_data = std::allocator_traits<allocator_type>::allocate(m_alloc, new_cap * sizeof(CharT));
		
		for (size_type i = 0; i < m_size; i++)
		{
			std::allocator_traits<allocator_type>::construct(m_alloc, new_data + i, m_data[i]);
			std::allocator_traits<allocator_type>::destroy(m_alloc, m_data + i);
		}
		
		std::allocator_traits<allocator_type>::allocate(m_alloc, m_data, m_capacity * sizeof(CharT));
		m_data = new_data;
		m_capacity = new_cap;
	}
	
	size_type capacity() const { return m_capacity; }
	
	void shrink_to_fit()
	{
		if (m_capacity == m_size) return;
		
		allocator_type new_allocator = get_allocator();
		const CharT* new_data = std::allocator_traits<allocator_type>::allocate(new_allocator, m_size);
		
		std::copy(begin(), end(), new_data);
		std::allocator_traits<allocator_type>::deallocate(m_alloc, m_data, m_capacity);
		
		m_data = new_data;
		m_capacity = m_size;
	}
	
	
	// Modifiers
	void clear()
	{
		for(int i = 0; i < m_size; ++i)
			std::allocator_traits<allocator_type>::destroy(m_alloc, m_data + i);
		
		m_data = nullptr;
		m_size = 0;
	}
	
	
	
	
	
	
	
	
	// for tests
	static void print(const basic_string& string)
	{
		for (int i = 0; i < string.m_size; i++)
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
using wstring = basic_string<wchar_t>;
using u8string = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;



#endif //!BASIC_STRING_H
