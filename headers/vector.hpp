#ifndef OWN_VECTOR_H
#define OWN_VECTOR_H

#include <initializer_list>
#include <memory>
#include <iterator>
#include <stdexcept>
#include <type_traits>

template<typename T>
class vector 
{
public:
    //Type declarations
    using value_type = T;
    using allocator_type = std::allocator<T>;
    using size_type = typename allocator_type::size_type;
    using difference_type = typename allocator_type::difference_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    

    //constuctors and destructor
    vector();
    explicit vector( size_type count );
    vector( size_type count, const T& value );
    vector( size_type count, T&& value);
    template< class InputIt >
    vector( InputIt first, InputIt last );
    vector( const vector& other );
    vector( vector&& other );
    vector( std::initializer_list<T> init );
    ~vector();

    //operator =
    vector& operator=( const vector& other ); //copy assignment
    vector& operator=( vector&& other ); //move assignment
    vector& operator=( std::initializer_list<T> ilist ); //initializer list assignment

    //assign methods
    void assign( size_type count, const T& value );
    template< class InputIt >
    void assign( InputIt first, InputIt last );
    void assign( std::initializer_list<T> init );

    //get_allocator
    allocator_type get_allocator() const { return m_allocator; }

    //element access
    reference at( size_type pos );
    const_reference at( size_type pos ) const;

    reference operator[]( size_type pos ) { return m_data[pos]; }
    const_reference operator[]( size_type pos ) const { return m_data[pos]; }

    reference front() { return m_data[0]; }
    const_reference front() const { return m_data[0]; }

    reference back() { return m_data[m_size - 1]; }
    const_reference back() const { return m_data[m_size - 1]; }

    pointer data() { return m_data; }
    const_pointer data() const { return m_data; }

    //iterators
    iterator begin() noexcept { return m_data; }
    const_iterator begin() const noexcept { return m_data; }
    const_iterator cbegin() const noexcept { return m_data; }

    iterator end() noexcept { return m_data + m_size; }
    const_iterator end() const noexcept { return m_data + m_size; }
    const_iterator cend() const noexcept { return m_data + m_size; }

    reverse_iterator rbegin() noexcept { return reverse_iterator( end() ); }
    const_reverse_iterator rbegin() const noexcept { return reverse_iterator( end() ); }
    const_reverse_iterator crbegin() const noexcept { return reverse_iterator( end() ); }

    reverse_iterator rend() noexcept {return reverse_iterator( begin() ); }
    const_reverse_iterator rend() const noexcept {return reverse_iterator( begin() ); }
    const_reverse_iterator crend() const noexcept {return reverse_iterator( begin() ); }

    //capacity
    bool empty() const noexcept { return m_size == 0; }

    size_type size() const noexcept { return m_size; }
    size_type max_size() const noexcept { return std::allocator_traits<allocator_type>::max_size( m_allocator ); }

    void reserve( size_type new_cap );

    size_type capacity() const noexcept { return m_capacity; }

    //modifiers
    void clear() noexcept;

    iterator insert( const_iterator pos, const T& value );
    iterator insert( const_iterator pos, T&& value );

    iterator insert( const_iterator pos, size_type count, const T& value );

    template< class InputIt >
    iterator insert( const_iterator pos, InputIt first, InputIt last );

    iterator insert( const_iterator pos, std::initializer_list<T> ilist );

    template< class... Args >
    iterator emplace( const_iterator pos, Args&&... args );

    iterator erase( const_iterator pos );
    iterator erase( const_iterator first, const_iterator last );

    void push_back( const T& value );
    void push_back( T&& value );

    void pop_back();

    void resize( size_type count );
    void resize( size_type count, const value_type& value );

    void swap( vector& other );

private:
    T* m_data = nullptr;
    size_type m_size = 0;
    size_type m_capacity = 0;
    allocator_type m_allocator;

};

//constuctors and destructor
template<typename T>
inline vector<T>::vector() : m_data( nullptr ), m_size( 0 ), m_capacity( 0 )
{

}

template<typename T>
inline vector<T>::vector( size_type count )
{
    m_data = m_allocator.allocate(count * sizeof(T));
    m_size = count;
    m_capacity = count;

    for (size_type i = 0; i < count; i++)
    {
        m_allocator.construct(m_data + i, T());
    }
}

template<typename T>
inline vector<T>::vector( size_type count, const T& value )
{
    m_data = m_allocator.allocate(count * sizeof(T));
    m_size = count;
    m_capacity = count;

    for (size_type i = 0; i < count; i++)
    {
        m_allocator.construct(m_data + i, value);
    }
}

template<typename T>
inline vector<T>::vector( size_type count, T&& value )
{
    m_data = m_allocator.allocate(count * sizeof(T));
    m_size = count;
    m_capacity = count;
    
    for (size_type i = 0; i < count; i++)
    {
        m_allocator.construct(m_data + i, std::forward<T>(value));
    }
}

template<typename T>
template< class InputIt >
inline vector<T>::vector( InputIt first, InputIt last )
{
    m_data = m_allocator.allocate(std::distance(first, last) * sizeof(T));
    m_size = std::distance(first, last);
    m_capacity = std::distance(first, last);

    size_type i = 0;

    for (; first!= last; ++first)
    {
        m_allocator.construct(m_data + i, *first);
        ++i;
    }
}

template<typename T>
inline vector<T>::vector( const vector& other )
{
    m_data = m_allocator.allocate(other.m_size * sizeof(T));
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    size_type i = 0;

    for (auto it = other.begin(); it != other.end(); ++it)
    {
        m_allocator.construct(m_data + i, *it);
        ++i;
    }
}

template<typename T>
inline vector<T>::vector( vector&& other )
{
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_data = other.m_data;

    for (size_type i = 0; i < other.m_size; ++i)
    {
        other.m_allocator.destroy(other.m_data + i);
    }

    other.m_allocator.deallocate(other.m_data, other.m_capacity * sizeof(T));

    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template<typename T>
inline vector<T>::vector( std::initializer_list<T> ilist )
{
    m_data = m_allocator.allocate(ilist.size() * sizeof(T));
    m_size = ilist.size();
    m_capacity = ilist.size();

    size_type i = 0;

    for (auto it = ilist.begin(); it!= ilist.end(); ++it)
    {
        m_allocator.construct(m_data + i, *it);
        ++i;
    }
}

template<typename T>
inline vector<T>::~vector()
{
    for (size_type i = 0; i < m_size; i++)
    {
        m_allocator.destroy(m_data + i);
    }

    m_allocator.deallocate(m_data, m_capacity);

    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
}

//assignment operator
template<typename T>
inline vector<T>& vector<T>::operator=( const vector& other ) //copy assignment 
{
    if (this == &other)
    {
        return *this;
    }

    clear();

    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_data = m_allocator.allocate(other.m_size * sizeof(T));

    size_type i = 0;

    for (auto it = other.begin(); it!= other.end(); ++it)
    {
        m_allocator.construct(m_data + i, *it);
        ++i;
    }

    return *this;
}

template<typename T>
inline vector<T>& vector<T>::operator=( vector&& other ) //move assignment
{
    if (this == &other)
    {
        return *this;
    }
    
    clear();
    
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_data = other.m_data;

    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;

    return *this;
}

template<typename T>
inline vector<T>& vector<T>::operator=( std::initializer_list<T> ilist ) //initializer list assignment  
{

    if(this == &ilist)
    {
        return *this;
    }

    clear();

    m_size = ilist.size();
    m_capacity = ilist.size();
    m_data = m_allocator.allocate(ilist.size() * sizeof(T));

    size_type i = 0;

    for (auto it = ilist.begin(); it!= ilist.end(); ++it)
    {
        m_allocator.construct(m_data + i, *it);
        ++i;
    }

    return *this;
}

//assign methods
template<typename T>
inline void vector<T>::assign( size_type count, const T& value ) 
{
    clear();

    m_data = m_allocator.allocate(count * sizeof(T));
    m_size = count;
    m_capacity = count;

    for (size_type i = 0; i < count; i++)
    {
        m_allocator.construct(m_data + i, value);
    }
}

template<typename T>
template<class InputIt >
inline void vector<T>::assign( InputIt first, InputIt last )
{
    clear();

    m_data = m_allocator.allocate(std::distance(first, last) * sizeof(T));
    m_size = std::distance(first, last);
    m_capacity = std::distance(first, last);

    size_type i = 0;

    for (;first != last; ++first)    
    {
        m_allocator.construct(m_data + i, *first);
        ++i;
    }
}

template<typename T>
inline void vector<T>::assign( std::initializer_list<T> ilist )
{
    clear();

    m_data = m_allocator.allocate(ilist.size() * sizeof(T));
    m_size = ilist.size();
    m_capacity = ilist.size();

    size_type i = 0;

    for (auto it = ilist.begin(); it!= ilist.end(); ++it)
    {
        m_allocator.construct(m_data + i, *it);
        ++i;
    }
}

//element access
template<typename T>
inline typename vector<T>::reference vector<T>::at( size_type pos )
{
    if (pos >= m_size)
    {
        throw std::out_of_range("Index out of range");
    }

    return m_data[pos];
}

template<typename T>
inline typename vector<T>::const_reference vector<T>::at( size_type pos ) const
{
    if (pos >= m_size)
    {
        throw std::out_of_range("Index out of range");
    }
    
    return m_data[pos];
}

//capacity
template<typename T>
inline void vector<T>::reserve( size_type new_cap )
{
    if (new_cap > max_size())
    {
        throw std::length_error("Capacity overflow");
    }

    if (new_cap <= m_capacity)
    {
        return;
    }

    T* new_data = m_allocator.allocate(new_cap * sizeof(T));
    
    for (size_type i = 0; i < m_size; i++)
    {
        m_allocator.construct(new_data + i, m_data[i]);
        m_allocator.destroy(m_data + i);
    }

    m_allocator.deallocate(m_data, m_capacity * sizeof(T));
    m_data = new_data;
    m_capacity = new_cap;
}

//modifiers
template<typename T>
inline void vector<T>::clear() noexcept
{
    for (size_type i = 0; i < m_size; i++)
    {
        m_allocator.destroy(m_data + i);
    }
    m_data = nullptr;
    m_size = 0;
}

template<typename T>
inline typename vector<T>::iterator vector<T>::insert( const_iterator pos, const T& value ) 
{
    if (size() == 0)
    {
        m_data = m_allocator.allocate(1 * sizeof(T));
        m_allocator.construct(m_data, value);
        m_size = 1;

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

template<typename T>
inline typename vector<T>::iterator vector<T>::insert( const_iterator pos, T&& value ) 
{
    if (size() == 0)
    {
        m_data = m_allocator.allocate(1 * sizeof(T));
        m_allocator.construct(m_data, std::move(value));
        m_size = 1;

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

        m_data[index] = std::move(value);
        m_size++;

        return m_data + index;
    }
}

template<typename T>
inline typename vector<T>::iterator vector<T>::insert( const_iterator pos, size_type count, const T& value )
{
    if (size() == 0)
    {
        m_data = m_allocator.allocate(count * sizeof(T));
        m_size = count;
        m_capacity = count;

        for (size_type i = 0; i < count; i++)
        {
            m_allocator.construct(m_data + i, value);
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
            m_allocator.construct(m_data + index + i, value);
        }

        m_size += count;

        return m_data + index;
    }
}

template< typename T >
template< class InputIt >
inline typename vector<T>::iterator vector<T>::insert( const_iterator pos, InputIt first, InputIt last )
{
    if (first == last)
    {
        return const_cast<iterator>(pos);
    }

    if (size() == 0)
    {
        m_data = m_allocator.allocate(std::distance(first, last) * sizeof(T));
        m_size = std::distance(first, last);
        m_capacity = std::distance(first, last);

        size_type i = 0;

        for (; first!= last; ++first)
        {
            m_allocator.construct(m_data + i, *first);
            ++i;
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

        size_type i = 0;

        for (; first!= last; ++first)
        {
            m_allocator.construct(m_data + index + i, *first);
            ++i;
        }

        m_size += std::distance(first, last);

        return m_data + index;
    }
} 

template<typename T>
inline typename vector<T>::iterator vector<T>::insert( const_iterator pos, std::initializer_list<T> ilist )
{
    if (ilist.size() == 0)
    {
        return const_cast<iterator>(pos);
    }

    if (size() == 0)
    {
        m_data = m_allocator.allocate(ilist.size() * sizeof(T));
        m_size = ilist.size();
        m_capacity = ilist.size();

        size_type i = 0;

        for (auto it = ilist.begin(); it!= ilist.end(); ++it)
        {
            m_allocator.construct(m_data + i, *it);
            ++i;
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

        size_type i = 0;
        
        for (auto it = ilist.begin(); it!= ilist.end(); ++it)
        {
            m_allocator.construct(m_data + index + i, *it);
            ++i;
        }
        
        m_size += ilist.size();

        return m_data + index;
    }
}

template<typename T>
template< class... Args >
inline typename vector<T>::iterator vector<T>::emplace( const_iterator pos, Args&&... args )
{
    if (size() == 0)
    {
        m_data = m_allocator.allocate(1 * sizeof(T));
        m_allocator.construct(m_data, std::forward<Args>(args)...);
        m_size = 1;

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

        m_allocator.construct(m_data + index, std::forward<Args>(args)...);
        m_size++;

        return m_data + index;
    }
}

template<typename T>
inline typename vector<T>::iterator vector<T>::erase( const_iterator pos )
{
    size_type index = pos - cbegin();

    m_allocator.destroy(pos);

    for (size_type i = index; i < m_size - 1; i++)
    {
        m_data[i] = m_data[i + 1];
    }

    m_size--;

    return m_data + index;
}

template<typename T>
inline typename vector<T>::iterator vector<T>::erase( const_iterator first, const_iterator last )   
{
    size_type first_index = first - cbegin();
    size_type last_index = last - cbegin();

    size_type range = last_index - first_index;

    for (size_type i = first_index; i < last_index; i++)
    {
        m_allocator.destroy(m_data + i);
    }

    for (size_type i = last_index; i < m_size; i++)
    {
        m_data[i - range] = m_data[i];
    }

    m_size -= range;

    return m_data + first_index;
}

template<typename T>
inline void vector<T>::push_back( const T& value )
{
    if (size() == 0)
    {
        m_data = m_allocator.allocate(1 * sizeof(T));
        m_allocator.construct(m_data, value);
        m_size = 1;
        m_capacity = 1;
    }

    else 
    {
        if (m_size == m_capacity)
        {
            reserve(m_capacity * 2);
        }

        m_allocator.construct(m_data + m_size, value);
        m_size++;
        m_capacity++;
    }
}

template< typename T >
inline void vector<T>::push_back( T&& value )
{
    if (size() == 0)
    {
        m_data = m_allocator.allocate(1 * sizeof(T));
        m_allocator.construct(m_data, std::move(value));
        m_size = 1;
        m_capacity = 1;
    }

    else 
    {
        if (m_size == m_capacity)
        {
            reserve(m_capacity * 2);
        }

        m_allocator.construct(m_data + m_size, std::move(value));
        m_size++;
        m_capacity++;
    }
}

template< typename T >
inline void vector<T>::pop_back()
{
    if (size() == 0)
    {
        return;
    }

    erase(cend() - 1);
}

template< typename T >
inline void vector<T>::resize( size_type count )
{
    if (count == size())
    {
        return;
    }

    else if (count < m_size)
    {
        m_size = count;
        return;
    }

    else if (count > m_size)
    {
        if (count > m_capacity*2)
        {
            reserve(count);
        }

        else 
        {
            reserve(m_capacity*2);
        }

        for (size_type i = count; i < m_size; i++)
        {
            m_allocator.construct(m_data + i, T());
        }

        m_size = count;
    }
}

template< typename T >
inline void vector<T>::resize( size_type count, const T& value )
{
    if (count == m_size)
    {
        return;
    }

    else if (count <= m_size)
    {
        m_size = count;
        return;
    }

    else if(count > m_size)
    {
        if (count > m_capacity*2)
        {
            reserve(count);
        }

        else 
        {
            reserve(m_capacity*2);
        }
        
        for (size_type i = m_size; i < count; i++)
        {
            m_allocator.construct(m_data + i, value);
        }
        m_size = count;
    }
}

template<typename T>
void vector<T>::swap( vector& other ) 
{
    std::swap(m_data, other.m_data);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
}

#endif //!OWN_VECTOR_H