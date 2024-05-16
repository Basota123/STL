#ifndef _OWN_SET_H_
#define _OWN_SET_H_

#include <functional>
#include <memory>
#include <cstddef>
#include <concepts>
#include <iterator>
#include <utility>

template< class Key > 
class set
{
private:
    class bidiriter;
    struct base_node;
    struct node;


public:
    using Compare = std::less<Key>;
    using Allocator = std::allocator<Key>;
    using key_type = Key;
    using value_type = Key;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::allocator_traits<Allocator>::pointer;
    using const_pointer = std::allocator_traits<Allocator>::const_pointer;
    using iterator = bidiriter;
    using const_iterator = const bidiriter;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = node;

public:
    set();
    template< std::input_iterator InputIt >
    set( InputIt first, InputIt last);
    set( const set& other );
    set( set&& other );
    set( std::initializer_list<value_type> init );
    ~set();

    set& operator=( const set& other );
    set& operator=( set&& other ) noexcept;
    set& operator=( std::initializer_list<value_type> ilist );

    allocator_type get_allocator() const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const noexcept;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const noexcept;

    iterator rbegin();
    const_iterator rbegin() const;
    const_iterator crbegin() const noexcept;

    iterator rend();
    const_iterator rend() const;
    const_iterator crend() const noexcept;

    bool empty() const noexcept;
    size_type size() const;
    size_type max_size() const;

    void clear();

    std::pair<iterator, bool> insert( const value_type& value );
    std::pair<iterator, bool> insert( value_type&& value );
    iterator insert( const_iterator pos, const value_type& value );
    iterator insert( const_iterator pos, value_type&& value );
    template< std::input_iterator InputIt >
    void insert( InputIt first, InputIt last );
    void insert( std::initializer_list<value_type> ilist );

    template< class... Args >
    std::pair<iterator, bool> emplace( Args&&... args );

    iterator erase( const_iterator pos );
    iterator erase( const_iterator first, const_iterator last );
    size_type erase( const Key& key );

    void swap( set& other ) noexcept

    size_type count( const Key& key ) const;

    iterator find( const Key& key );
    const_iterator find( const Key& key ) const;

};


#endif //!_OWN_SET_H_