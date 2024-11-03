#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cstddef>
#include <iterator>
#include <stdexcept>

template< class T, std::size_t N > 
struct array
{
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    
    reference at( size_type pos );
    const_reference at( size_type pos ) const;

    reference operator[]( size_type pos );
    const_reference operator[]( size_type pos ) const;

    reference back();
    const_reference back() const;

    pointer data() noexcept;
    const_pointer data() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept; 

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const  noexcept;
    const_reverse_iterator crbegin() const noexcept;

    constexpr bool empty() const noexcept;
    constexpr size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;

    void fill( const T& value );
    void swap( array& ohter ) noexcept;
    
private:
    value_type m_data[N];
    size_type m_size;
};


template< class T, std::size_t N > 
inline array<T, N>::reference array<T, N>::at( size_type pos )
{
    if (pos >= size())
        throw std::out_of_range("out of range");

    return m_data[pos];
}

template< class T, std::size_t N > 
inline array<T, N>::const_reference array<T, N>::at( size_type pos ) const
{
    if (pos >= size())
        throw std::out_of_range("out of range");

    return m_data[pos];
}

template< class T, std::size_t N > 
inline array<T, N>::reference array<T, N>::operator[]( size_type pos )
{
    return m_data[pos];
}

template< class T, std::size_t N > 
inline array<T, N>::const_reference array<T, N>::operator[]( size_type pos ) const
{
    return m_data[pos];
}

template< class T, std::size_t N > 
inline array<T, N>::reference front()
{
    return m_data[0];
}


template< class T, std::size_t N > 
inline array<T, N>::const_reference front() const
{
    return m_data[0];
}


template< class T, std::size_t N > 
inline array<T, N>::reference back()
{
    return m_data[size() - 1];
}

template< class T, std::size_t N > 
inline array<T, N>::const_reference back() const
{
    return m_data[size() - 1];
}

template< class T, std::size_t N > 
inline array<T, N>::pointer array<T, N>::data() noexcept
{
    return &m_data;
}

template< class T, std::size_t N > 
inline array<T, N>::const_pointer array<T, N>::data() const noexcept
{
    return &m_data;
}

template< class T, std::size_t N >
inline array<T, N>::iterator array<T, N>::begin() noexcept
{
    return &m_data[0];
}

template< class T, std::size_t N >
inline array<T, N>::const_iterator array<T, N>::begin() const noexcept
{
    return &m_data[0];
}

template< class T, std::size_t N >
inline array<T, N>::const_iterator array<T, N>::cbegin() const noexcept
{
    return &m_data[0];
}

template< class T, std::size_t N >
inline array<T, N>::iterator array<T, N>::end() noexcept
{
    return &m_data[N];
}

template< class T, std::size_t N >
inline array<T, N>::const_iterator array<T, N>::end() const noexcept
{
    return &m_data[N];
}

template< class T, std::size_t N >
inline array<T, N>::const_iterator array<T, N>::cend() const noexcept
{
    return &m_data[N];
}

template< class T, std::size_t N >
constexpr bool array<T, N>::empty() const noexcept
{
    return size() == 0;
}

template< class T, std::size_t N >
constexpr array<T, N>::size_type array<T, N>::size() const noexcept
{
    return N;
}

#endif // ARRAY_HPP
