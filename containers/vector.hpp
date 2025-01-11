#ifndef OWN_VECTOR_H
#define OWN_VECTOR_H

// CXX20

#include <memory>
#include <limits>
#include <concepts>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <type_traits>
#include <initializer_list>

template <
    class T, class Allocator = std::allocator<T>>
class vector
{
private:
    class random_access_iterator;

public:
    // Type declaration
    using value_type               =    T;
    using allocator_type           =    Allocator;
    using size_type                =    std::size_t;
    using difference_type          =    std::ptrdiff_t;
    using reference                =    value_type&;
    using const_reference          =    const value_type&;
    using pointer                  =    typename std::allocator_traits<Allocator>::pointer;
    using const_pointer            =    typename std::allocator_traits<Allocator>::const_pointer;
    using iterator                 =    random_access_iterator;
    using const_iterator           =    const random_access_iterator;
    using reverse_iterator         =    std::reverse_iterator<iterator>;
    using const_reverse_iterator   =    std::reverse_iterator<const_iterator>;

    // Constructors and Destructor
    constexpr vector() noexcept(noexcept(Allocator()));
    constexpr explicit vector(const Allocator& alloc) noexcept;
    constexpr vector(size_type count,
                     const T& value,
                     const Allocator& alloc = Allocator());
    constexpr explicit vector(size_type count,
                    const Allocator& alloc = Allocator());
    template <std::input_iterator InputIt>
    constexpr vector(InputIt first, InputIt last,
                     const Allocator& alloc = Allocator());
    constexpr vector(const vector& other);
    constexpr vector(const vector& other, const Allocator& alloc);
    constexpr vector(vector&& other) noexcept;
    constexpr vector(vector&& other, const Allocator& alloc);
    constexpr vector(std::initializer_list<T> init,
                     const Allocator& alloc = Allocator());
    constexpr ~vector();

    // Operator = 
    constexpr vector& operator=( const vector& other );
    constexpr vector& operator=( vector&& other ) 
        noexcept(std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value == true);
    constexpr vector& operator=( std::initializer_list<value_type> ilist );

    // Assign methods
    constexpr void assign(size_type count, const T& value);
    template <std::input_iterator InputIt>
    constexpr void assign(InputIt first, InputIt last);
    constexpr void assign(std::initializer_list<T> ilist);

    // Get allocator
    constexpr allocator_type get_allocator() const { return m_alloc; }

    // Element access
    constexpr reference at( size_type pos );
    constexpr const_reference at( size_type pos ) const;

    constexpr reference operator[]( size_type pos ) { return *(m_arr + pos); }
    constexpr const_reference operator[]( size_type pos ) const { return *(m_arr + pos); }

    constexpr reference front() { return *m_arr; }
    constexpr const_reference front() const { return *m_arr; }

    constexpr reference back() { return *(m_arr + m_size - 1ul); }
    constexpr const_reference back() const { return *(m_arr + m_size - 1ul); }

    constexpr T* data() noexcept { return m_arr; }
    constexpr const T* data() const noexcept { return m_arr; }

    // Iterators
    constexpr iterator begin() noexcept { return iterator(m_arr); }
    constexpr const_iterator begin() const noexcept { return const_iterator(m_arr); }
    constexpr const_iterator cbegin() const noexcept { return const_iterator(m_arr); }

    constexpr iterator end() noexcept { return iterator(m_arr + m_size); }
    constexpr const_iterator end() const noexcept { return const_iterator(m_arr + m_size); }
    constexpr const_iterator cend() const noexcept { return const_iterator(m_arr + m_size); }

    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    // Capacity
    constexpr bool empty() const noexcept { return m_size == 0ul; }
    constexpr size_type size() const noexcept { return m_size; }
    constexpr size_type max_size() const noexcept { return std::numeric_limits<difference_type>::max(); }
    constexpr void reserve( size_type new_cap );
    constexpr size_type capacity() const noexcept { return m_capacity; }
    constexpr void shrink_to_fit();

    // Modifiers
    constexpr void clear() noexcept;
    
    constexpr iterator insert( const_iterator pos, const T& value );
    constexpr iterator insert( const_iterator pos, T&& value );
    constexpr iterator insert( const_iterator pos, size_type count, const T& value );
    template< std::input_iterator InputIt >
    constexpr iterator insert( const_iterator pos, InputIt first, InputIt last );
    constexpr iterator insert( const_iterator pos, std::initializer_list<T> ilist );

    template< class... Args >
    constexpr iterator emplace( const_iterator pos, Args&&... args );

    constexpr iterator erase( const_iterator pos );
    constexpr iterator erase( const_iterator first, const_iterator last );

    constexpr void push_back( const T& value );
    constexpr void push_back( T&& value );

    template< class... Args >
    constexpr reference emplace_back( Args&&... args );

    constexpr void pop_back();

    constexpr void resize( size_type count );
    constexpr void resize( size_type count, const value_type& value );

    constexpr void swap( vector& other ) noexcept(
        std::allocator_traits<Allocator>::propagate_on_container_swap::value
            || std::allocator_traits<Allocator>::is_always_equal::value);

private:
    class random_access_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
        using iterator_concept = std::contiguous_iterator_tag;
        using iterator_type = T*;
        using pointer = T*;
        using reference = T&;
        using value_type = T;

        random_access_iterator(const random_access_iterator &other) : m_ptr(other.m_ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }

        random_access_iterator& operator++()
        {
            m_ptr++;
            return *this;
        }
        random_access_iterator operator++(int)
        {
            random_access_iterator temp = *this;
            ++(*this);
            return temp;
        }
        random_access_iterator& operator--()
        {
            m_ptr--;
            return *this;
        }
        random_access_iterator operator--(int)
        {
            random_access_iterator temp = *this;
            --(*this);
            return temp;
        }

        random_access_iterator& operator+=(difference_type n)
        {
            difference_type m = n;
            if (m >= 0)
                while (m--)
                    ++m_ptr;
            else
                while (m++)
                    --m_ptr;

            return *this;
        }
        random_access_iterator operator+(difference_type n)
        {
            random_access_iterator temp = *this;
            return temp += n;
        }
        random_access_iterator& operator-=(difference_type n) { return *this += -n; }
        random_access_iterator operator-(difference_type n)
        {
            random_access_iterator temp = *this;
            return temp -= n;
        }

        difference_type operator-(const random_access_iterator &other) const { return m_ptr - other.m_ptr; }

        reference operator[](difference_type n) { return *(this + n); }

        friend bool operator==(const random_access_iterator &rhs, const random_access_iterator &lhs) { return rhs.m_ptr == lhs.m_ptr; }
        friend bool operator!=(const random_access_iterator &rhs, const random_access_iterator &lhs) { return rhs.m_ptr != lhs.m_ptr; }
        friend bool operator>=(const random_access_iterator &rhs, const random_access_iterator &lhs) { return rhs.m_ptr >= lhs.m_ptr; }
        friend bool operator>(const random_access_iterator &rhs, const random_access_iterator &lhs) { return rhs.m_ptr > lhs.m_ptr; }
        friend bool operator<=(const random_access_iterator &rhs, const random_access_iterator &lhs) { return rhs.m_ptr <= lhs.m_ptr; }
        friend bool operator<(const random_access_iterator &rhs, const random_access_iterator &lhs) { return rhs.m_ptr < lhs.m_ptr; }

    private:
        friend class vector;

        random_access_iterator() = default;
        random_access_iterator(pointer ptr) : m_ptr(ptr) {}

        pointer m_ptr;
    };

    void allocate_and_construct(size_type count, const T& value)
    {
        m_arr = std::allocator_traits<Allocator>::allocate(m_alloc, count);
        for (std::size_t i = 0ul; i < count; i++)
            std::allocator_traits<Allocator>::construct(m_alloc, m_arr + i, value);
    }

    pointer m_arr;
    size_type m_size;
    size_type m_capacity;
    Allocator m_alloc;
};

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector() noexcept(noexcept(Allocator()))
    : m_arr(nullptr), m_size(0ul), m_capacity(0ul), m_alloc() {}

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector(const Allocator& alloc) noexcept
    : m_arr(nullptr), m_size(0ul), m_capacity(0ul), m_alloc(alloc) {}

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector(size_type count, const T& value, const Allocator& alloc)
    : m_arr(nullptr), m_size(count), m_capacity(count), m_alloc(alloc)
{
    allocate_and_construct(count, value);
}

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector(size_type count, const Allocator& alloc)
    : m_arr(nullptr), m_size(count), m_capacity(count), m_alloc(alloc)
{ 
    allocate_and_construct(count, T()); 
}


template< class T, class Allocator >
template< std::input_iterator InputIt >
constexpr vector<T, Allocator>::vector(InputIt first, InputIt last, const Allocator &alloc)
    : m_arr(nullptr), 
      m_size(std::distance(first, last)), 
      m_capacity((std::distance(first, last))), 
      m_alloc(alloc)
{
    m_arr = std::allocator_traits<Allocator>::allocate(m_alloc, m_capacity);
    std::size_t counter = 0ul;
    for (; first != last; ++first)
    {
        std::allocator_traits<Allocator>::construct(m_alloc, m_arr + counter, *first);
        ++counter;
    }
        
}

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector(const vector& other)
    : m_arr(nullptr), 
      m_size(other.m_capacity), 
      m_capacity(other.m_capacity),
      m_alloc(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator()))
{
    if (m_capacity > 0ul)
    {
        m_arr = std::allocator_traits<Allocator>::allocate(m_alloc, m_capacity);
        for (std::size_t i = 0; i < m_size; i++)
            std::allocator_traits<Allocator>::construct(m_alloc, m_arr + i, *(other.m_arr + i));
    }
}

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector(const vector& other, const Allocator& alloc)
    : m_arr(other.m_arr), m_size(other.m_size), m_capacity(other.m_capacity), m_alloc(alloc) 
{
    if (m_capacity > 0ul)
    {
        m_arr = std::allocator_traits<Allocator>::allocate(m_alloc, m_capacity);
        for (std::size_t i = 0ul; i < m_size; i++)
            std::allocator_traits<Allocator>::construct(m_alloc, m_arr + i, *(other.m_arr + i));
    }
}

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector(vector&& other) noexcept
    : m_arr(other.m_arr), m_size(other.m_size), m_capacity(other.m_capacity),
      m_alloc(std::move(other.get_allocator()))
{
    other.m_arr = nullptr;
    other.m_size = 0ul;
    other.m_capacity = 0ul;
}

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector(vector&& other, const Allocator& alloc)
    : m_arr(other.m_arr), m_size(other.m_size), m_capacity(other.m_capacity),
      m_alloc(alloc)
{
    other.m_arr = nullptr;
    other.m_size = 0ul;
    other.m_capacity = 0ul;
}

template< class T, class Allocator >
constexpr vector<T, Allocator>::vector(std::initializer_list<T> init, const Allocator& alloc) 
    : m_size(init.size()), m_capacity(init.size()), m_alloc(alloc)
{
    m_arr = std::allocator_traits<Allocator>::allocate(m_alloc, m_capacity);
    std::size_t counter = 0ul;
    for (auto it = init.begin(); it != init.end(); ++it)
    {
        std::allocator_traits<Allocator>::construct(m_alloc, m_arr + counter, *it);
        ++counter;
    }   
}

template< class T, class Allocator >
constexpr vector<T, Allocator>::~vector()
{
    for (size_type i = 0ul; i < m_size; i++)
        std::allocator_traits<Allocator>::destroy(m_alloc, m_arr + i);

    std::allocator_traits<Allocator>::deallocate(m_alloc, m_arr, m_capacity);
    
    m_arr = nullptr;
    m_size = 0ul;
    m_capacity = 0ul;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>& vector<T, Allocator>::operator=(const vector& other)
{
    if (this != &other)
    {
        if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value)
        {
            allocator_type new_allocator = other.get_allocator();
            if (new_allocator != m_alloc)
            {
                for (std::size_t i = 0ul; i < m_size; ++i)
                    std::allocator_traits<allocator_type>::destroy(m_alloc, m_arr + i);
                std::allocator_traits<allocator_type>::deallocate(m_alloc, m_arr, m_capacity);

                m_arr = nullptr;
                m_size = 0ul;
                m_capacity = 0ul;

                m_alloc = new_allocator;
            }
        }

        m_size = other.m_size;
        m_capacity = other.m_capacity;

        m_arr = std::allocator_traits<allocator_type>::allocate(m_alloc, m_capacity);
        for (std::size_t i = 0ul; i < m_size; ++i)
            std::allocator_traits<allocator_type>::construct(m_alloc, m_arr + i, *(other.m_arr + i));
            
    }

    return *this;

}

template <class T, class Allocator>
constexpr vector<T, Allocator>& vector<T, Allocator>::operator=(vector&& other) 
    noexcept(std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value == true)
{
    if (this != &other)
    {
        if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value)
            m_alloc = other.get_allocator();
        
        for (std::size_t i = 0ul; i < m_size; ++i)
            std::allocator_traits<allocator_type>::destroy(m_alloc, m_arr + i);
        std::allocator_traits<allocator_type>::deallocate(m_alloc, m_arr, m_capacity);
        
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_arr = other.m_arr;

        other.m_size = 0ul;
        other.m_capacity = 0ul;
        other.m_arr = nullptr;
    }

    return *this;
}

template <class T, class Allocator>
constexpr vector<T, Allocator>& vector<T, Allocator>::operator=(std::initializer_list<value_type> ilist)
{
    for (std::size_t i = 0ul; i < m_size; ++i)
        std::allocator_traits<allocator_type>::destroy(m_alloc, m_arr + i);
    std::allocator_traits<allocator_type>::deallocate(m_alloc, m_arr, m_capacity);

    m_size = ilist.size();
    m_capacity = ilist.size();

    m_arr = std::allocator_traits<allocator_type>::allocate(m_alloc, m_capacity);
    size_type counter = 0ul;
    for (auto it = ilist.begin(); it != ilist.end(); ++it)
    {
        std::allocator_traits<allocator_type>::construct(m_alloc, m_arr + counter, *it);
        ++counter;
    }

    return *this;
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::assign( size_type count, const T& value )
{
    m_size = count; 
    if (m_capacity >= count)
    {
        for (size_type i = 0; i < count; ++i)
            m_arr[i] = value;
    }
    
    else if (m_capacity < count)
    {
        reserve(count);
        for (size_type i = 0; i < count; ++i)
            m_arr[i] = value;
    }
}

template <class T, class Allocator>
template <std::input_iterator InputIt>
constexpr void vector<T, Allocator>::assign(InputIt first, InputIt last)
{
    constexpr size_type count = std::distance(first, last);

    m_size = count;
    size_type counter = 0;

    if (m_capacity >= count)
    {
        for (; first != last; ++first)
        {
            m_arr[counter] = *first;
            ++counter;
        }
    } 

    else if (m_capacity < count)
    {
        reserve(count);
        for (; first != last; ++first)
        {
            m_arr[counter] = *first;
            ++counter;
        }
    }
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::assign(std::initializer_list<T> ilist)
{
    constexpr size_type count = std::distance(ilist.begin(), ilist.end());

    m_size = count;
    size_type counter = 0;

    if (m_capacity >= count)
    {
        for (auto it = ilist.begin(); it != ilist.end(); ++it)
        {
            m_arr[counter] = *it;
            ++counter;
        }
    } 

    else if (m_capacity < count)
    {
        reserve(count);
        for (auto it = ilist.begin(); it != ilist.end(); ++it)
        {
            m_arr[counter] = *it;
            ++counter;
        }
    }
}

template <class T, class Allocator>
constexpr typename vector<T, Allocator>::reference vector<T, Allocator>::at( size_type pos )
{
    if (pos >= size()) throw std::out_of_range("Index out of bounds");
    return *(m_arr + pos);
}

template <class T, class Allocator>
constexpr typename vector<T, Allocator>::const_reference vector<T, Allocator>::at( size_type pos ) const
{
    if (pos >= size()) throw std::out_of_range("Index out of bounds");
    return *(m_arr + pos);
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::reserve( size_type new_cap )
{
    if (new_cap > max_size()) throw std::length_error("New capacity is to much");
    if (new_cap <= m_capacity) return;

    pointer new_arr = std::allocator_traits<allocator_type>::allocate(m_alloc, new_cap);
    for (size_type i = 0ul; i < m_size; ++i)
    {
        std::allocator_traits<allocator_type>::construct(m_alloc, new_arr + i, *(m_arr + i));
        std::allocator_traits<allocator_type>::destroy(m_alloc, m_arr + i);
    }

    std::allocator_traits<allocator_type>::deallocate(m_alloc, m_arr, m_capacity);

    m_arr = new_arr;
    m_capacity = new_cap;

    new_arr = nullptr;
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::shrink_to_fit()
{
    if (m_size == m_capacity) return;

    pointer new_arr = std::allocator_traits<allocator_type>::allocate(m_alloc, m_size);
    for (size_type i = 0ul; i < m_size; ++i)
    {
        std::allocator_traits<allocator_type>::construct(m_alloc, new_arr + i, std::move(*(m_arr + i)));
        std::allocator_traits<allocator_type>::destroy(m_alloc, m_arr + i);
    }

    std::allocator_traits<allocator_type>::deallocate(m_alloc, m_arr, m_capacity);

    m_arr = new_arr;
    m_capacity = m_size;

    new_arr = nullptr;
}

template <class T, class Allocator>
constexpr void vector<T, Allocator>::clear() noexcept
{
    for (size_type i = 0ul; i < m_size; i++)
        std::allocator_traits<allocator_type>::destroy(m_alloc, m_arr + i);

    m_size = 0ul;
}

template <class T, class Allocator>
constexpr typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, const T& value)
{
    const size_type index = pos - cbegin();
    if (m_size >= m_capacity) reserve(2 * m_capacity);

    for (size_type i = m_size; i > index; i--)
        m_arr[i] = m_arr[i - 1];
    
    m_arr[index] = value;

    return iterator(m_arr + index);
}

template <class T, class Allocator>
constexpr typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, T&& value)
{
    const size_type index = pos - cbegin();
    if (m_size >= m_capacity) reserve(2 * m_capacity);

    for (size_type i = m_size; i > index; i--)
        m_arr[i] = m_arr[i - 1];
    
    m_arr[index] = std::move(value);
    m_size++;

    return iterator(m_arr + index);
}

template <class T, class Allocator>
constexpr typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos,
    size_type count, const T& value)
{
    const size_type index = pos - cbegin();
    if (m_size >= m_capacity) reserve(2 * m_capacity);

    for (size_type i = m_size; i > index; i--)
        m_arr[i + count - 1] = m_arr[i - 1];
    
    for (size_type i = index; i < index + count; ++i)
        m_arr[i] = value;

    m_size += count;
    return iterator(m_arr + index);
}

template <class T, class Allocator>
template <std::input_iterator InputIt>
constexpr typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, 
    InputIt first, InputIt last)
{
    const size_type index = pos - cbegin();
    if (m_size >= m_capacity) reserve(2 * m_capacity);

    const difference_type count = std::distance(first, last);

    for (size_type i = m_size; i > index; i--)
        m_arr[i + count - 1] = m_arr[i - 1];

    for (size_type i = 0; i < count; ++i)
        m_arr[index + i] = *first++;
    
    m_size += count;
    return iterator(m_arr + index);    
}

template <class T, class Allocator>
constexpr typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos,
    std::initializer_list<T> ilist)
{
    const size_type index = pos - cbegin();
    if (m_size >= m_capacity) reserve(2 * m_capacity);

    auto it = ilist.begin();
    const difference_type count = std::distance(it, ilist.end());

    for (size_type i = m_size; i > index; i--)
        m_arr[i + count - 1] = m_arr[i - 1];

    for (size_type i = 0; i < count; ++i)
        m_arr[index + i] = *it++;
    
    m_size += count;
    return iterator(m_arr + index);
}






#endif //! OWN_VECTOR_H

