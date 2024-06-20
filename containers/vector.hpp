#ifndef OWN_VECTOR_H
#define OWN_VECTOR_H

//CXX20 

#include <initializer_list>
#include <memory>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <iostream>

template<
    class T, class Allocator = std::allocator<T>
> class vector 
{
public:
    // Type declarations
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::allocator_traits<allocator_type>::pointer;
    using const_pointer = std::allocator_traits<allocator_type>::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using alloc_traits = std::allocator_traits<allocator_type>;

    // Member functions
    constexpr vector() noexcept(noexcept(Allocator()));
    constexpr explicit vector( const Allocator& alloc ) noexcept;
    constexpr vector( size_type count,
        const T& value,
        const Allocator& alloc = Allocator() );        
    constexpr explicit vector( size_type count,
                 const Allocator& alloc = Allocator() );
    template< class InputIt >
    constexpr vector( InputIt first, InputIt last,
        const Allocator& alloc = Allocator() );
    constexpr vector( const vector& other );
    constexpr vector( const vector& other, const Allocator& alloc );
    constexpr vector( vector&& other );
    constexpr vector( vector&& other, const Allocator& alloc );
    constexpr vector( std::initializer_list<T> init,
        const Allocator& alloc = Allocator() );
    constexpr ~vector();

    constexpr vector& operator=( const vector& other );
    constexpr vector& operator=( vector&& other ) noexcept;
    constexpr vector& operator=( std::initializer_list<value_type> ilist );

    constexpr void assign( size_type count, const T& value );
    template< class InputIt >
    constexpr void assign( InputIt first, InputIt last );
    constexpr void assign( std::initializer_list<T> ilist );

    constexpr allocator_type get_allocator() const noexcept { return m_alloc; }

    // Element access
    constexpr reference at( size_type pos );
    constexpr const_reference at( size_type pos ) const;

    constexpr reference operator[]( size_type pos ) { return m_data[pos]; }
    constexpr const_reference operator[]( size_type pos ) const { return m_data[pos]; }

    constexpr reference front() { return m_data[0]; }
    constexpr const_reference front() const {return m_data[0]; }

    constexpr reference back() { return m_data[size() - 1]; }
    constexpr const_reference back() const {return m_data[size() - 1]; }

    constexpr pointer data() { return m_data; }
    constexpr const_pointer data() const { return m_data; }

    //Iterators
    constexpr iterator begin() { return m_data; }
    constexpr const_iterator begin() const { return m_data; }
    constexpr const_iterator cbegin() const noexcept { return m_data; }

    constexpr iterator end() { return m_data + size(); }
    constexpr const_iterator end() const { return m_data + size(); }
    constexpr const_iterator cend() const noexcept { return m_data + size(); }

    constexpr reverse_iterator rbegin() { return reverse_iterator( end() ); }
    constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator( end() ); }
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator( end() ); }

    constexpr reverse_iterator rend() { return reverse_iterator( begin() ); }
    constexpr const_reverse_iterator rend() const { return const_reverse_iterator( begin() ); }
    constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator( begin() ); }

    // Capacity
    constexpr bool empty() const { return size() == 0; }
    constexpr size_type size() const { return m_size; }
    constexpr size_type max_size() const noexcept { return alloc_traits::max_size( m_alloc ); }
    constexpr void reserve( size_type new_cap );
    constexpr size_type capacity() const noexcept { return m_capacity; }    
    constexpr void shrink_to_fit();

    // Modifiers
    constexpr void clear();
    constexpr iterator insert( const_iterator pos, const T& value );
    constexpr iterator insert( const_iterator pos, T&& value );
    constexpr iterator insert( const_iterator pos, size_type count, const T& value );
    template< class InputIt >
    constexpr iterator insert( const_iterator pos, InputIt first, InputIt last );
    constexpr iterator insert( const_iterator pos, std::initializer_list<T> ilist );


private:
    T* m_data = nullptr;
    size_type m_size = 0;
    size_type m_capacity = 0;
    allocator_type m_alloc;
};

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector() noexcept(noexcept(Allocator()))
{

}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const Allocator &alloc) noexcept
{
    m_alloc = alloc;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(size_type count, const T &value, const Allocator &alloc)
{
    m_size = count;
    m_capacity = count;

    m_data = alloc_traits::allocate(m_alloc, count);
    for (size_type i = 0; i < count; i++)
        alloc_traits::construct(m_alloc, m_data + i, value);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(size_type count, const Allocator &alloc)
{
    m_size = count;
    m_capacity = count;

    m_data = alloc_traits::allocate(m_alloc, count);
    for (size_type i = 0; i < count; i++)
        alloc_traits::construct(m_alloc, m_data + i, T());
}

template <class T, class Allocator>
template <class InputIt>
constexpr vector<T, Allocator>::vector(InputIt first, InputIt last, const Allocator &alloc)
{
    constexpr size_type count = std::distance(first, last);
    m_size = count;
    m_capacity = count;

    m_data = alloc_traits::allocate(m_alloc, count);

    size_type counter = 0;
    for (; first != last; ++first)
    {
        alloc_traits::construct(m_alloc, m_data + counter, *first);
        ++counter;
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector( const vector& other )
{
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_alloc = other.m_alloc;

    m_data = alloc_traits::allocate(m_alloc, m_size);
    for (size_type i = 0; i < m_size; ++i)
        alloc_traits::construct(m_alloc, m_data + i, other.m_data + i);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(const vector &other, const Allocator &alloc)
{
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_alloc = alloc;

    m_data = alloc_traits::allocate(m_alloc, m_size);
    for (size_type i = 0; i < m_size; ++i)
        alloc_traits::construct(m_alloc, m_data + i, other.m_data + i);
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector( vector&& other )
{
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_alloc = std::move(other.m_alloc);
    m_data = other.m_data;

    other.m_size = 0;
    other.m_capacity = 0;
    other.m_data = nullptr;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector( vector&& other, const Allocator& alloc )
{
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_alloc = alloc;
    m_data = other.m_data;

    other.m_size = 0;
    other.m_capacity = 0;
    other.m_data = nullptr;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::vector(std::initializer_list<T> init, const Allocator &alloc)
{
    m_size = init.size();
    m_capacity = init.size();

    m_data = alloc_traits::allocate(m_alloc, init.size());
    size_type counter = 0;
    for (auto it = init.begin(); it != init.end(); ++it)
    {
        alloc_traits::construct(m_alloc, m_data + counter, *it);
        ++counter;
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::~vector()
{
    for (size_type i = 0; i < m_size; i++)
    {
        alloc_traits::destroy(m_alloc, m_data + i);
    }

    alloc_traits::deallocate(m_alloc, m_data, m_capacity);

    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>& vector<T, Allocator>::operator=( const vector& other )
{
    if (this == *other) return *this;
    
    else
    {
        m_size = other.m_size;

        if (m_capacity >= other.m_capacity)
        {
            for (size_type i = 0; i < other.m_size; ++i)
                m_data[i] = other.m_data[i];
        }

        else if (m_capacity < other.m_capacity)
        {
            reserve(other.m_capacity);
            for (size_type i = 0; i < other.m_size; ++i)
                m_data[i] = other.m_data[i];
        }

        return *this;
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>& vector<T, Allocator>::operator=( vector&& other ) noexcept
{
    if (this == *other) return this;

    else
    {
        m_size = other.m_size;
        
        if (m_capacity >= other.m_capacity)
        {
            for (size_type i = 0; i < other.m_size; ++i)
                m_data[i] = std::forward<T>(other.m_data[i]);
        }

        else if (m_capacity < other.m_capacity)
        {
            reserve(other.m_capacity);
            for (size_type i = 0; i < other.m_size; ++i)
                m_data[i] = std::forward<T>(other.m_data[i]);
        }

        m_alloc = std::forward<allocator_type>(other.m_alloc);

        other.m_size = 0;
        other.m_capacity = 0;
        other.m_data = nullptr;

        return *this;
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>& vector<T, Allocator>::operator=( std::initializer_list<value_type> ilist )
{
    if (this == *ilist) return this;

    else
    {   
        m_size = ilist.size();

        if (m_capacity >= ilist.size())
        {
            size_type counter = 0;
            for (auto it = ilist.begin(); it != ilist.end(); ++it)
            {
                m_data[counter] = *it;
                ++counter;
            }
        }

        else if (m_capacity < ilist.size())
        {
            reserve(ilist.size());
            size_type counter = 0;
            for (auto it = ilist.begin(); it != ilist.end(); ++it)
            {
                m_data[counter] = *it;
                ++counter;
            }
        }
        return this;
    }
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::assign( size_type count, const T& value )
{
    m_size = count; 

    if (m_capacity >= count)
    {
        for (size_type i = 0; i < count; ++i)
            m_data[i] = value;
    }
    
    else if (m_capacity < count)
    {
        reserve(count);
        for (size_type i = 0; i < count; ++i)
            m_data[i] = value;
    }
}

template <class T, class Allocator>
template< class InputIt >
constexpr void vector<T, Allocator>::assign( InputIt first, InputIt last )
{
    constexpr size_type count = std::distance(first, last);
    m_size = count;

    if (m_capacity >= count)
    {
        size_type counter = 0;
        for (; first != last; ++first)
        {
            m_data[counter] = *first;
            ++counter;
        }
    }
    
    else if (m_capacity < count)
    {
        reserve(count);
        size_type counter = 0;
        for (; first != last; ++first)
        {
            m_data[counter] = *first;
            ++counter;
        }
    }
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::assign( std::initializer_list<T> ilist )
{
    m_size = ilist.size();

    if (m_capacity >= ilist.size())
    {
        size_type counter = 0;
        for (auto it = ilist.begin(); it != ilist.end(); ++it)
        {
            m_data[counter] = *it;
            ++counter;
        }
    }

    else if (m_capacity < ilist.size())
    {
        reserve(ilist.size());
        size_type counter = 0;
        for (auto it = ilist.begin(); it != ilist.end(); ++it)
        {
            m_data[counter] = *it;
            ++counter;
        }
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::reference vector<T, Allocator>::at( size_type pos )
{
    if (pos >= size())
    {
        throw std::out_of_range("Index out of range");
    }

    return m_data[pos];
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::const_reference vector<T, Allocator>::at( size_type pos ) const
{
    if (pos >= size())
    {
        throw std::out_of_range("Index out of range");
    }

    return m_data[pos];
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::reserve( size_type new_cap )
{
    if (new_cap > max_size()) throw throw std::length_error("Capacity overflow");
    
    if (capacity() >= new_cap) return;
    else 
    {   
        T* new_data = alloc_traits::allocate(m_alloc, new_cap);
        for (size_type i = 0; i < m_size; ++i)
        {
            alloc_traits::construct(m_alloc, new_data + i, m_data + i);
            alloc_traits::destroy(m_alloc, m_data + i);
        }
        alloc_traits::deallocate(m_alloc, m_data, m_capacity);

        m_capacity = new_cap;
        m_data = new_data;
    }

}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::shrink_to_fit()
{
    if (size() == capacity()) return;
    else
    {
        T* new_data = alloc_traits::allocate(m_alloc, size());
        for (size_type i = 0; i < m_size; ++i)
        {
            alloc_traits::construct(m_alloc, new_data + i, m_data + i);
            alloc_traits::destroy(m_alloc, m_data + i);
        }
        alloc_traits::deallocate(m_alloc, m_data, m_capacity);

        m_capacity = m_size;
        m_data = new_data;
    }
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::clear()
{
    for (size_type i = 0; i < m_size; i++)
    {
        m_allocator.destroy(m_data + i);
    }
    m_data = nullptr;
    m_size = 0;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, const T& value)
{
    if (size() == 0)
    {
        m_data = alloc_traits::allocate(m_alloc, 1);
        alloc_traits::construct(m_alloc, m_data, value);
        m_size = 1;
        m_capacity = 1;

        return m_data;
    }
    
    else
    {
        size_type index = pos - cbegin();

        if (m_size == m_capacity)
        {
            reserve(m_capacity * 2);
        }

        for (size_type i = m_size; i > index; i--)
        {
            m_data[i] = m_data[i - 1];
        }

        m_data[index] = value;
        m_size++;

        return m_data + index;
    }
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, T&& value)
{
    insert(pos, std::move(value));
}

template <class T, class Allocator>
constexpr vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, size_type count, const T& value)
{
    if (size() == 0)
    {
        m_data = alloc_traits::allocate(m_alloc, count);
        m_size = count;
        m_capacity = count;

        for (size_type i = 0; i < count; i++)
        {
            alloc_traits::construct(m_alloc, m_data + i, value);
        }

        return m_data;
    }

    else
    {
        size_type index = pos - cbegin();
        
        if (m_size == m_capacity)
        {
            reserve(m_capacity * 2);
        }
        
        for (size_type i = m_size; i > index; i--)
        {
            m_data[i] = m_data[i - 1];
        }

        for (size_type i = 0; i < count; i++)
        {
            alloc_traits::construct(m_alloc, m_data + index + i, value);
        }

        m_size += count;

        return m_data + index;
    }
}




#endif //!OWN_VECTOR_H