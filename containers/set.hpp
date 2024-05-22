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
    class tree_iter;
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
    using iterator = tree_iter;
    using const_iterator = const tree_iter;
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

    void swap( set& other ) noexcept;

    size_type count( const Key& key ) const;

    iterator find( const Key& key );
    const_iterator find( const Key& key ) const;


private:

    class tree_iter
    {
    private:
        friend class set;

    public:
        using iterator_type = set::value_type;
        using value_type = iterator_type;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;

    private:
        tree_iter() = delete;
        tree_iter( const base_node& node ) : m_node( node ) {}

        base_node* m_node;

    public:
        tree_iter( const tree_iter& other ) : m_node( other.m_node ) {}






    };

    struct base_node
    {
        char height;
        base_node* left;
        base_node* right;
        base_node* parent;

        friend class set;
    };

    struct node : base_node 
    {
        Key key;

        node() = default;
        node( const node& other ) = delete;
        node( const Key& value ) : key(value) {}

        template< class... Args >
        node( Args&& ...args ) : key( std::forward<Args>( args )... ) {}
    };

    //members of class 
    base_node fake_node{ '0', &fake_node, &fake_node, &fake_node };
    node* left = nullptr;
    node* right = nullptr;
    node* parent = nullptr;

    // вспомогательные методы для итерации по дереву
    static base_node* next( base_node* node );
    static base_node* prev( base_node* node );

    // методы для работы с АВЛ-деревом
    bool is_balanced();
    void balance( base_node* node );

    // вспомогательные методы для работы с АВЛ-деревом
    char get_height( const base_node* p ) const { return p?p->height:0; }
    int balance_factor( const base_node* p ) const { return get_height(p->right)-get_height(p->left); }
    
    void fix_height( base_node* p )
    {
        char hl = get_height(p->right);
        char hr = get_height(p->left);

        p->height = (hl > hr? hl : hr) + 1;
    }

    // балансировка узлов
    base_node* right_rotate( base_node* p )
    {
        base_node* temp = p->left;
        p->left = q->right;
    }
};


#endif //!_OWN_SET_H_